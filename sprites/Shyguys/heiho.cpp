#include <heiho.h>

CREATE_STATE(daEnHeiho_c, Sleep);
CREATE_STATE(daEnHeiho_c, Jump);
CREATE_STATE(daEnHeiho_c, Dizzy);
CREATE_STATE(daEnHeiho_c, Idle);

dActor_c *daEnHeiho_c::build() {
	void *buffer = AllocFromGameHeap1(sizeof(daEnHeiho_c));
	return new(buffer) daEnHeiho_c;
}

const char* heihoArcList [] = {
	"heiho",
	NULL
};

const SpriteData heihoSpriteData = {ProfileId::EN_HEIHO, 8, -16, 0, 8, 8, 8, 0, 0, 0, 0, 0};
Profile heihoProfile(&daEnHeiho_c::build, SpriteId::EN_HEIHO, &heihoSpriteData, ProfileId::EN_KURIBO, ProfileId::EN_HEIHO, "EN_HEIHO", heihoArcList, 0x12);

void daEnHeiho_c::dieFall_Begin() {
    playChrAnim("diefall", 0, 0.0, 1.0);
    return dEn_c::dieFall_Begin();
}

void daEnHeiho_c::dieFall_Execute() {
    updateModel();
    return dEn_c::dieFall_Execute();
}


void daEnHeiho_c::dieOther_Begin() {
    removeMyActivePhysics();

    playChrAnim("die", 0, 0.0, 1.15);

	rot.y = 0;
	rot.x = 0;

    //set Y speed for gravity
    y_speed_inc = -0.1075;
    max_speed.y = -4.0;
    speed.x = 0.0;
    speed.y = 0.0;
}

void daEnHeiho_c::dieOther_Execute() {
    updateModel();
    HandleYSpeed();
    doSpriteMovement();
}

extern "C" void sub_80024C20(void);
extern "C" void __destroy_arr(void*, void(*)(void), int, int);
//extern "C" __destroy_arr(struct DoSomethingCool, void(*)(void), int cnt, int bar);

bool daEnHeiho_c::CreateIceActors() {
	struct DoSomethingCool my_struct = { 0, {pos.x, pos.y-3.8, pos.z}, {1.3, 1.5, 1.5}, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
    frzMgr.Create_ICEACTORs( (void*)&my_struct, 1 );
    __destroy_arr( (void*)&my_struct, sub_80024C20, 0x3C, 1 );
    return true;
}

void daEnHeiho_c::reactFumiProc(dStageActor_c* killingActor) {
    if (health == 1) {
        health = 0;
        doStateChange(&StateID_Dizzy);
    } else {
        Vec2 killSpeed;
        killSpeed.y = speed.y;
        killSpeed.x = speed.x;
        dEn_c::killWithSpecifiedState(killingActor, &killSpeed, &dEn_c::StateID_DieOther);
    }
}

void daEnHeiho_c::loadModel() {
    resFile.data = getResource("heiho", "g3d/heiho.brres");
    nw4r::g3d::ResMdl bmdl = resFile.GetResMdl("heiho");
	model.setup(bmdl, &allocator, 0x227, 1, 0);
	SetupTextures_Enemy(&model, 0);

	nw4r::g3d::ResAnmChr resAnmChr = resFile.GetResAnmChr("idle");
	anmChr.setup(bmdl, resAnmChr, &allocator, 0);
    playChrAnim("idle", 0, 0.0, 2.0);

    resPat = resFile.GetResAnmTexPat("color");
	anmTexPat.setup(bmdl, resPat, &allocator, 0, 1);
	anmTexPat.bindEntry(&model, &resPat, 0, 1);
	anmTexPat.setFrameForEntry(color, 0);
	anmTexPat.setUpdateRateForEntry(0.0f, 0);
	model.bindAnim(&anmTexPat);

    return;
}

void daEnHeiho_c::initialize() {
    type = (heyhoTypes)(settings >> 28 & 0xF);
    color = settings >> 24 & 0xF;
    health = settings >> 17 & 1;
    distance = settings >> 12 & 0xF;
    spawnDir = (settings >> 8 & 1)^1;
    
    scale.x = 1.0;
    scale.y = 1.0;
    scale.z = 1.0;

    //tile collider
	static const lineSensor_s below(-4<<12, 4<<12, 0<<12);
	static const pointSensor_s above(0<<12, 20<<12);
	static const lineSensor_s adjacent(5<<12, 10<<12, 8<<12);

	collMgr.init(this, &below, &above, &adjacent);

    //these are used to set the "center" of the sprite
    //used by yoshi tongue and dieFall
    pos_delta2.x = 0.0;
    pos_delta2.y = 12.0;
    pos_delta2.z = 0.0;

    spriteSomeRectX = 28.0;
	spriteSomeRectY = 32.0;
	_320 = 0.0;
	_324 = 12.0;

    _36D = 2;

    float zPositions[2] = {1500.0, -2500.0};
    pos.z = zPositions[appearsOnBackFence];

    hhAngles[0] = 0x2000;
    hhAngles[1] = 0xE000;

    aPhysics.info.xDistToCenter = 0.0;
	aPhysics.info.yDistToCenter = 10.0;
	aPhysics.info.xDistToEdge = 8.0;
	aPhysics.info.yDistToEdge = 10.0;

    int directionToUse;
    if (type < Jumper || type > Pacer) {
        directionToUse = dSprite_c__getXDirectionOfFurthestPlayerRelativeToVEC3(this, pos);
    } else {
        directionToUse = spawnDir;
    }

    direction = directionToUse;
	rot.y = hhAngles[directionToUse];

    //set walking distances for pacer
    if (type == Pacer) {
        float finalOffset = 16.0 * distance - 8.0;
        finalPos[0] = pos.x + finalOffset;
        finalPos[1] = pos.x - finalOffset;
    }

    switch (type) {
        case Walker:
        case WalkerLedge:
        case Pacer:
            doStateChange(&StateID_Walk);
            break;
        case Sleeper:
            doStateChange(&StateID_Sleep);
            break;
        case Jumper:
            doStateChange(&StateID_Jump);
            break;
        default:
            doStateChange(&StateID_Idle);
            break;
    }
    return;
}

void daEnHeiho_c::setTurnState() {
    dStateBase_c *state = acState.getCurrentState();
    if (state == &StateID_Walk || state == &StateID_TrplnJump) {    
        doStateChange(&StateID_Turn);
    }
    return;
}

extern "C" bool CallXCompareFuncLT(float, float);

void daEnHeiho_c::vf2D0(dStageActor_c* collidingActor) {
    bool directionThing = CallXCompareFuncLT(collidingActor->pos.x + collidingActor->pos_delta2.x, pos.x + pos_delta2.x);
    dStateBase_c *state = acState.getCurrentState();
    if (state != &StateID_Sleep && state != &StateID_Dizzy) {
        direction = directionThing;
        rot.y = hhAngles[direction];
    }
    if (state == &StateID_Turn) {
        doStateChange(&StateID_Walk);
    }
    if (state == &StateID_Walk || state == &StateID_Turn || state == &StateID_TrplnJump) {
        setWalkSpeed();
    }
    return;
}

//can't use daEnKuribo_c::setWalkSpeed()
float heihoSpeeds[2] = {0.6, -0.6};
void daEnHeiho_c::setWalkSpeed() {
    speed.x = heihoSpeeds[direction];
    return;
}

void daEnHeiho_c::playChrAnim(const char* name, int playsOnce, float unk, float rate) {
	nw4r::g3d::ResAnmChr resAnmChr = resFile.GetResAnmChr(name);
	anmChr.bind(&model, resAnmChr, playsOnce);
	model.bindAnim(&anmChr, unk);
	anmChr.setUpdateRate(rate);
}

bool daEnHeiho_c::checkForLedge(float xOffset) {
    float xOffs[] = {xOffset, -xOffset};

    VEC3 tileToCheck;
	tileToCheck.y = 4.0 + pos.y;
    tileToCheck.z = pos.z;
    tileToCheck.x = pos.x + xOffs[direction];

	u32 unit = collMgr.getTileBehaviour2At(tileToCheck.x, pos.y - 2.0, currentLayerID);

	if (((unit >> 0x10) & 0xFF) == 8) {
		return false;
    } else {
        float zeroFloat = 0.0;
        bool result = collMgr.sub_800757B0(&tileToCheck, &zeroFloat, currentLayerID, 1, -1);
	    if (((!result) || (tileToCheck.y <= zeroFloat)) || (zeroFloat <= pos.y - 5.0)) {
            return false;
        } else {
            return true;
        }
    }

	return false;
}

extern "C" u8 dEn_c_EnBgCheck(dEn_c *self); //checks if we're on top of a tile
extern "C" bool CheckDanceValues_Bahps(u32 param); //checks for bahs
extern "C" void DoStuffAndMarkDead(dStageActor_c *actor, Vec vector, float unk); //kills if touching lava
extern "C" int SmoothRotation(short* rot, u16 amt, int unk2); //rotates sprite until amt, returns true when finished

void daEnHeiho_c::initializeState_Walk() {
    if (acState.getPreviousState() != &StateID_Turn) {
        playChrAnim("walk", 0, 0.0, 0.85);
    }
    setWalkSpeed();
    //set Y speed for gravity
    y_speed_inc = -0.1875;
    max_speed.x = 0.0;
    max_speed.y = -4.0;
    max_speed.z = 0.0;
}

void daEnHeiho_c::executeState_Walk() {
    updateModel();
    HandleYSpeed();
    doSpriteMovement();
    SmoothRotation(&rot.y, hhAngles[direction], 0x800);
    walkEffect();

    if (!(dEn_c_EnBgCheck(this))) { //if we aren't standing on a tile
        if (collMgr.isOnTopOfTile() && (_45D == false) && (speed.y <= 0.0f)) {
            velocity2.x = velocity2.x + _310;
        }
    } else {
        velocity2.x = 0.0;
        speed.y = 0.0; //if we are standing on a tile, stop applying gravity
        if (isOnEnLiftRemoconTrpln()) {
            doStateChange(&StateID_TrplnJump);
            return;
        }
        if (checkForLedge(2.5f) == false) {
            if (type == WalkerLedge) {
                doStateChange(&StateID_Turn);
                return;
            }
        }
    }
    //turn at final position if we're a pacer
    if (type == Pacer) {
        if ((direction == RIGHT && pos.x >= finalPos[0]) || (direction == LEFT && pos.x <= finalPos[1]))
            doStateChange(&StateID_Turn);
    }
    //turn if touching a tile
    if (collMgr.outputMaybe & 0x15 << direction & 0x3f) {
        doStateChange(&StateID_Turn);
    }
    DoStuffAndMarkDead(this, pos, 1.0);
    return;
}

//hack to fix a bug when being stunned while turning
void daEnHeiho_c::initializeState_Turn() {
    if (acState.getPreviousState() == &StateID_Dizzy) {
        playChrAnim("walk", 0, 0.0, 0.85);
    }
    return daEnKuriboBase_c::initializeState_Turn();
}

void daEnHeiho_c::executeState_Turn() {
    updateModel();
    HandleYSpeed();
    doSpriteMovement();

    if (!(dEn_c_EnBgCheck(this))) {
        if (collMgr.isOnTopOfTile() && _45D == false && speed.y <= 0.0f) {
            velocity2.x = velocity2.x + _310;
        }
    } else {
        speed.y = 0.0;
        if (isOnEnLiftRemoconTrpln()) {
            doStateChange(&StateID_TrplnJump);
            return;
        }
    }

    DoStuffAndMarkDead(this, pos, 1.0);

    bool doneTurning = SmoothRotation(&rot.y, hhAngles[direction], 0x500);

    if (doneTurning) {
        doStateChange(&StateID_Walk);
    }
    return;
}

//another hack to fix a bug when being stunned while trplnjump
void daEnHeiho_c::initializeState_TrplnJump() {
    if (acState.getPreviousState() == &StateID_Dizzy && !hasSetWalkAnim) {
        playChrAnim("walk", 0, 0.0, 0.85);
        hasSetWalkAnim = true; //dumb stupid fix
    }
    return daEnKuriboBase_c::initializeState_TrplnJump();
}

void daEnHeiho_c::executeState_TrplnJump() {
    updateModel();
    HandleYSpeed();
    doSpriteMovement();
    bool doneTurning = SmoothRotation(&rot.y, hhAngles[direction], 0x800);

    u8 BgCheck = dEn_c_EnBgCheck(this);
    //hit a wall?
    if (BgCheck & 4) {
        direction ^= 1;
        speed.x = -speed.x;
    }
    //touching ground?
    if (BgCheck & 1) {
        speed.y = 0.0;
        if (!isOnEnLiftRemoconTrpln()) {
            if (doneTurning) {
                doStateChange(&StateID_Walk);
            } else {
                doStateChange(&StateID_Turn);
            }
        } else {
            initializeState_TrplnJump();
        }
    }
    //turn at final position if we're a pacer
    if (type == Pacer) {
        if ((direction == RIGHT && pos.x >= finalPos[0]) || (direction == LEFT && pos.x <= finalPos[1]))
            doStateChange(&StateID_Turn);
    }
    DoStuffAndMarkDead(this, pos, 1.0);
}

void daEnHeiho_c::beginState_Sleep() {
	playChrAnim("sleep", 0, 0.0, 0.5);
    y_speed_inc = -0.1875;
    max_speed.x = 0.0;
    max_speed.y = -4.0;
    max_speed.z = 0.0;
    rot.y = 0;
}

void daEnHeiho_c::executeState_Sleep() {
    updateModel();
    HandleYSpeed();
    doSpriteMovement();

	if (dEn_c_EnBgCheck(this)) {
        if (isOnEnLiftRemoconTrpln()) {
            //bounce shyguy if on bouncy stuff
            speed.y = 5.5;
        }
    }

    DoStuffAndMarkDead(this, pos, 1.0);
}

void daEnHeiho_c::endState_Sleep() { }

extern "C" void startEnemySound(void*,SFX,Vec2*,int);
extern "C" void cvtSndObjctPos(Vec2 *out, Vec *stage_pos);
extern void *SoundClassRelated;

void daEnHeiho_c::beginState_Jump() {
	max_speed.x = 0.0;
	speed.x = 0.0;
	x_speed_inc = 0.0;

	jumpCounter = 0;

    y_speed_inc = -0.1875;
    max_speed.y = -4.0;
}

void daEnHeiho_c::executeState_Jump() {
    updateModel();
    HandleYSpeed();
    doSpriteMovement();

    u8 BgCheck = dEn_c_EnBgCheck(this);
    // touching ceiling
    if (BgCheck & 2) {
        speed.y = 0.0;
    }

    // Shy Guy is on ground
	if (BgCheck & 1) {
        Vec2 soundPos;
        cvtSndObjctPos(&soundPos, &pos);
        
		if (jumpCounter == 3) { jumpCounter = 0; }

		jumpCounter = jumpCounter + 1;

        nw4r::snd::SoundHandle handle;
		if (jumpCounter == 3) {
			playChrAnim("jump2", 1, 0.0, 0.6);
			speed.y = 6.0;
			startEnemySound(SoundClassRelated, SE_PLY_JUMPDAI_HIGH, &soundPos, 0);
		} else {
            playChrAnim("jump", 1, 0.0, 0.45);
            speed.y = 4.5;
            startEnemySound(SoundClassRelated, SE_PLY_JUMPDAI, &soundPos, 0);
	    }
	}

    DoStuffAndMarkDead(this, pos, 1.0);
}

void daEnHeiho_c::endState_Jump() { }

void daEnHeiho_c::beginState_Dizzy() {
    stateForRecover = acState.getPreviousState();

    if (stateForRecover == &StateID_Sleep) {
        playChrAnim("dizzy_sleep", 0, 0.0, 1.0);
    } else {
        playChrAnim("dizzy", 0, 0.0, 1.0);
    }

	max_speed.x = 0;
	speed.x = 0;
	x_speed_inc = 0;

	max_speed.y = -4.0;
	speed.y = -4.0;
	y_speed_inc = -0.1875;

	timer = 0;
	jumpCounter = 0;
    hasSetWalkAnim = false;
}

void daEnHeiho_c::executeState_Dizzy() {
    updateModel();
    HandleYSpeed();
    doSpriteMovement();

	if (dEn_c_EnBgCheck(this)) {
        if (isOnEnLiftRemoconTrpln()) {
            //bounce shyguy if on bouncy stuff
            speed.y = 5.5;
        }
    }

    DoStuffAndMarkDead(this, pos, 1.0);

	effect.spawn("Wm_en_spindamage", 0, &(Vec){pos.x, pos.y + 24.0, 0}, (S16Vec *)0, &(Vec){1.0, 1.0, 1.0});

	if (timer > 600) {
		doStateChange(stateForRecover);
		health = 1;
	}
    timer += 1;
}

void daEnHeiho_c::endState_Dizzy() { }

void daEnHeiho_c::beginState_Idle() {
    speed.x = 0.0;

    y_speed_inc = -0.1875;
    max_speed.x = 0.0;
    max_speed.y = -4.0;
    max_speed.z = 0.0;
}

void daEnHeiho_c::executeState_Idle() {
    updateModel();
    HandleYSpeed();
    doSpriteMovement();

	dEn_c_EnBgCheck(this);

    DoStuffAndMarkDead(this, pos, 1.0);
}

void daEnHeiho_c::endState_Idle() { }