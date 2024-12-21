#include <game.h>
#include <profile.h>
#include <daFreezer_c.h>

class daEnPuchiPakkun_c : public dEn_c {
    public:
        int onCreate();
        int onExecute();
        int onDraw();

        void _vf5C();

        void spriteCollision(ActivePhysics *apThis, ActivePhysics *apOther);

        void ice_Begin();

	    void ice_End();

        bool CreateIceActors();

        mHeapAllocator_c allocator;

		nw4r::g3d::ResFile resFile;
		m3d::mdl_c model;
		m3d::anmChr_c anmChr;

        u16 nipperAngles[2];
        u16 nipperTurnAngles[2];
        
        bool walks;
        int jumpHeight;

        float storedPosForIce;
        bool isBahJump;

        lineSensor_s adjacent;

        void loadModel();
        void updateModel();

        void playChrAnim(const char* name, int playsOnce, float unk2, float rate, bool syncedAnim);

        void setWalkSpeed();

        bool checkForLedge(float xOffset);
        bool isPlayerAbove();

        static dActor_c *build();

        USING_STATES(daEnPuchiPakkun_c);
        DECLARE_STATE(Idle);
        DECLARE_STATE(Walk);
        DECLARE_STATE(Turn);
        DECLARE_STATE(Jump);
        DECLARE_STATE(IceWait);
};
CREATE_STATE(daEnPuchiPakkun_c, Idle);
CREATE_STATE(daEnPuchiPakkun_c, Walk);
CREATE_STATE(daEnPuchiPakkun_c, Turn);
CREATE_STATE(daEnPuchiPakkun_c, Jump);
CREATE_STATE(daEnPuchiPakkun_c, IceWait);

dActor_c *daEnPuchiPakkun_c::build() {
	void *buffer = AllocFromGameHeap1(sizeof(daEnPuchiPakkun_c));
	return new(buffer) daEnPuchiPakkun_c;
}

const char* NipperArcList [] = {
	"pakkun_puchi",
	NULL
};

const SpriteData NipperSpriteData = {ProfileId::EN_PUCHI_PAKKUN, 8, 0xFFFFFFF0, 0, 8, 8, 8, 0, 0, 0, 0, 0};
Profile NipperProfile(&daEnPuchiPakkun_c::build, SpriteId::EN_PUCHI_PAKKUN, &NipperSpriteData, ProfileId::EN_PUCHI_PAKKUN, ProfileId::EN_PUCHI_PAKKUN, "EN_PUCHI_PAKKUN", NipperArcList, 0x12);

int daEnPuchiPakkun_c::onCreate() {
    //set angles for turning and walking
    nipperAngles[0] = 0x4000;
    nipperAngles[1] = -0x4000;

    nipperTurnAngles[0] = 0x3999;
    nipperTurnAngles[1] = -0x3999;

    //sprite settings
    walks = (settings >> 17) & 1;
    jumpHeight = (settings >> 12) & 0xF;

    loadModel();

    //setup collider
    ActivePhysics::Info nipper_cc;

    nipper_cc.xDistToCenter = 0.0;
	nipper_cc.yDistToCenter = 8.0;
	nipper_cc.xDistToEdge = 8.0;
	nipper_cc.yDistToEdge = 8.0;
	nipper_cc.category1 = 0x3;
	nipper_cc.category2 = 0x0;
	nipper_cc.bitfield1 = 0x4F;
	nipper_cc.bitfield2 = 0xFFBAD35E;
	nipper_cc.unkShort1C = 0;
	nipper_cc.callback = &dEn_c::collisionCallback;

    aPhysics.initWithStruct(this, &nipper_cc);

    aPhysics.addToList();
    
    max_speed.y = -4.0;

    direction = dSprite_c__getXDirectionOfFurthestPlayerRelativeToVEC3(this, pos);
	rot.y = nipperAngles[direction];

    _518 = 0;

    scale.x = 1.0;
    scale.y = 1.0;
    scale.z = 1.0;

    //tile collider
    int tileWidth = 8;
    if (walks) {
        //walking nippers have a wider tile collider, so they turn before hitting a wall
        tileWidth = 16;
    }
	static const lineSensor_s below(-4<<12, 4<<12, 0<<12);
	static const pointSensor_s above(0<<12, 16<<12);
    adjacent = lineSensor_s(3<<12, 8<<12, tileWidth<<12);

	collMgr.init(this, &below, &above, &adjacent);

    //these are used to set the "center" of the sprite
    //used by yoshi tongue and dieFall
    pos_delta2.x = 0.0;
    pos_delta2.y = 8.0;
    pos_delta2.z = 0.0;

    spriteSomeRectX = 16.0;
	spriteSomeRectY = 16.0;
	_320 = 0.0;
	_324 = 8.0;

    _36D = 2;

    if (settings & 1) { //spawn frozen
        doStateChange(&StateID_IceWait);
        //create an AC_FREEZER at our position and layer
        createChild(AC_FREEZER, this, 0, &pos, (S16Vec *)0, currentLayerID);
        aPhysics.removeFromList();
    } else {
        if (walks) {
            doStateChange(&StateID_Walk);
        } else {
            doStateChange(&StateID_Idle);
        }
    }
    
    return true;
}

int daEnPuchiPakkun_c::onExecute() {
    acState.execute();
    checkZoneBoundaries(0);
    return true;
}

int daEnPuchiPakkun_c::onDraw() {
    model.scheduleForDrawing();
    return true;
}

extern "C" void changePosAngle(VEC3 *, S16Vec *, int);
void daEnPuchiPakkun_c::_vf5C() {
    // this is something EVERYONE gets wrong
    // newer does a crappy version of this in a "updateModelMatricies" function
    // you're supposed to do it like this in finalUpdate/_vf5C
    Mtx someMatrix;
    Mtx thirdMatrix;

    changePosAngle(&pos, &rot, 1);
    PSMTXTrans(*matrix, pos.x, pos.y, pos.z);
    matrix.applyRotationY(&rot.y);
    
    PSMTXTrans(someMatrix, 0.0, pos_delta2.y, 0.0);
    PSMTXConcat(*matrix, someMatrix, *matrix);
    matrix.applyRotationX(&rot.x);

    PSMTXTrans(thirdMatrix, 0.0, -pos_delta2.y, 0.0);
    PSMTXConcat(*matrix, thirdMatrix, *matrix);
    matrix.applyRotationZ(&rot.z);

    model.setDrawMatrix(matrix);
    model.setScale(initialScale.x, initialScale.y, initialScale.z);
    model.calcWorld(false);
    return;
}

void daEnPuchiPakkun_c::spriteCollision(ActivePhysics *apThis, ActivePhysics *apOther) {
    if ((direction != LEFT) || (apThis->firstFloatArray[3] <= 0.0)) {
        if (direction != RIGHT) {
            return;
        }
        if (apThis->firstFloatArray[3] >= 0.0) {
            return;
        }
    }
    if (acState.getCurrentState() == &StateID_Walk) {
        doStateChange(&StateID_Turn);
    }
    return;
}

void daEnPuchiPakkun_c::ice_Begin() {
    if (!walks) {
        //munchers are slightly smaller when encased in ice, so we replicate that here
        scale = (Vec3){0.89, 0.89, 0.89};
        //store x pos
        storedPosForIce = pos.x;
        //set animation to open mouth
        anmChr.setCurrentFrame(0.0);
    }
    return dEn_c::ice_Begin();
}

void daEnPuchiPakkun_c::ice_End() {
    //restore everything
    if (!walks) {
        scale = (Vec3){1.0, 1.0, 1.0};
        pos.x = storedPosForIce;
        playChrAnim("attack", 0, 0.0, 1.0, true);
    }
    return dEn_c::ice_End();
}

extern "C" void sub_80024C20(void);
extern "C" void __destroy_arr(void*, void(*)(void), int, int);
//extern "C" __destroy_arr(struct DoSomethingCool, void(*)(void), int cnt, int bar);

//ice actor gets set differently if we're walking
//flag 0x1000 forces the ice to not be carryable
bool daEnPuchiPakkun_c::CreateIceActors() {
	struct DoSomethingCool ice_struct = { 0x1000, {pos.x, pos.y, pos.z+5.0}, {0.80, 0.80, 0.80}, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
    if (walks) {
        ice_struct.unk_01 = 0;
        ice_struct.pos = (Vec3){pos.x, pos.y-1.0, pos.z};
        ice_struct.scale = (Vec){1.0, 1.0, 1.0};
    }
    frzMgr.Create_ICEACTORs( (void*)&ice_struct, 1 );
    __destroy_arr( (void*)&ice_struct, sub_80024C20, 0x3C, 1 );
    return true;
}

void daEnPuchiPakkun_c::loadModel() {
    allocator.link(-1, GameHeaps[0], 0, 0x20);

	resFile.data = getResource("pakkun_puchi", "g3d/pakkun_puchi.brres");
    nw4r::g3d::ResMdl bmdl = resFile.GetResMdl("pakkun_puchi");
	model.setup(bmdl, &allocator, 0x227, 1, 0);
	SetupTextures_Enemy(&model, 0);

	nw4r::g3d::ResAnmChr resAnmChr = resFile.GetResAnmChr("attack");
	anmChr.setup(bmdl, resAnmChr, &allocator, 0);
    playChrAnim("attack", 0, 0.0, 1.0, true);

	allocator.unlink();
}

void daEnPuchiPakkun_c::updateModel() {
    model._vf1C();
    return;
}

extern u32 Stage_exeFrame;

void daEnPuchiPakkun_c::playChrAnim(const char* name, int playsOnce, float unk, float rate, bool syncedAnim) {
	nw4r::g3d::ResAnmChr resAnmChr = resFile.GetResAnmChr(name);
	anmChr.bind(&model, resAnmChr, playsOnce);
	model.bindAnim(&anmChr, unk);
	anmChr.setUpdateRate(rate);

    //sync our animations with other nippers
    if (syncedAnim) {
		float frame = Stage_exeFrame - (Stage_exeFrame / int(anmChr._28)) * int(anmChr._28);
		anmChr.setCurrentFrame(frame);
	}
}

void daEnPuchiPakkun_c::setWalkSpeed() {
    speed.x = (direction == 0) ? 0.5 : -0.5;
    return;
}

bool daEnPuchiPakkun_c::checkForLedge(float xOffset) {
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

bool daEnPuchiPakkun_c::isPlayerAbove() {
    for (int i = 0; i < 4; i++) {
        dAcPy_c *player = dAcPy_c::findByID(i);
        //grab any active players
        if (player) {
            //make sure they're not bubbled
            if (strcmp(player->states2.getCurrentState()->getName(), "dAcPy_c::StateID_Balloon")) {
                //are we in the same x range as the nipper?
                if (16.0 >= abs(player->pos.x - pos.x)) {
                    //are we in the y range?
                    if (pos.y + 8.0 <= player->pos.y && player->pos.y <= pos.y + 104.0) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

extern "C" u8 dEn_c_EnBgCheck(dEn_c *self);
extern "C" bool CheckDanceValues_Bahps(u32 param);
extern "C" void DoStuffAndMarkDead(dStageActor_c *actor, Vec vector, float unk);
extern "C" int SmoothRotation(short* rot, u16 amt, int unk2);
extern "C" s8 getNearestPlayerYDirection(dStageActor_c *, Vec3 *);

void daEnPuchiPakkun_c::beginState_Idle() {
    playChrAnim("attack", 0, 0.0, 1.0, true);
    //set Y speed for gravity
    y_speed_inc = -0.1875;
    max_speed.x = 0.0;
    max_speed.y = -4.0;
    max_speed.z = 0.0;
}

void daEnPuchiPakkun_c::executeState_Idle() {
    updateModel();
    HandleYSpeed();
    doSpriteMovement();

    if ((dEn_c_EnBgCheck(this)) & 1) {
        if (CheckDanceValues_Bahps(1)) {
            isBahJump = true;
            doStateChange(&StateID_Jump);
        }
        if (isPlayerAbove()) {
            doStateChange(&StateID_Jump);
        }
    }

    DoStuffAndMarkDead(this, pos, 1.0);
}

void daEnPuchiPakkun_c::endState_Idle() { }

void daEnPuchiPakkun_c::beginState_Walk() {
    if (acState.getPreviousState() != &StateID_Turn) {
        playChrAnim("walk", 0, 0.0, 1.0, true);
    }
    setWalkSpeed();
    //set Y speed for gravity
    y_speed_inc = -0.1875;
    max_speed.x = 0.0;
    max_speed.y = -4.0;
    max_speed.z = 0.0;
}

void daEnPuchiPakkun_c::executeState_Walk() {
    updateModel();
    HandleYSpeed();
    doSpriteMovement();
    SmoothRotation(&rot.y, nipperAngles[direction], 0x500);

    if (!(dEn_c_EnBgCheck(this))) {
        if (collMgr.isOnTopOfTile() && (_45D == false) && (speed.y <= 0.0f)) {
            velocity2.x = velocity2.x + _310;
        }
    } else {
        velocity2.x = 0.0;
        speed.y = 0.0;
        if (!(checkForLedge(2.5f))) {
            doStateChange(&StateID_Turn);
            return;
        }
        if (CheckDanceValues_Bahps(1)) {
            isBahJump = true;
            doStateChange(&StateID_Jump);
        }
        if (isPlayerAbove()) {
            doStateChange(&StateID_Jump);
        }
    }
    if (collMgr.outputMaybe & 0x15 << direction & 0x3f) {
        doStateChange(&StateID_Turn);
    }
    DoStuffAndMarkDead(this, pos, 1.0);
}

void daEnPuchiPakkun_c::endState_Walk() { }

void daEnPuchiPakkun_c::beginState_Turn() {
    if (acState.getPreviousState() != &StateID_Walk) {
        playChrAnim("walk", 0, 0.0, 1.0, false);
    } else {
        direction = direction^1;
    }
    speed.x = 0.0;
}

void daEnPuchiPakkun_c::executeState_Turn() {
    updateModel();
    HandleYSpeed();
    doSpriteMovement();

    if (!(dEn_c_EnBgCheck(this))) {
        if (collMgr.isOnTopOfTile() && _45D == false && speed.y <= 0.0f) {
            velocity2.x = velocity2.x + _310;
        }
    } else {
        speed.y = 0.0;
        if (CheckDanceValues_Bahps(1)) {
            isBahJump = true;
            doStateChange(&StateID_Jump);
        }
    }

    DoStuffAndMarkDead(this, pos, 1.0);

    //we use turnangles so the nipper turns the right direction
    bool doneTurning = SmoothRotation(&rot.y, nipperTurnAngles[direction], 0x500);

    if (doneTurning) {
        doStateChange(&StateID_Walk);
    }
}

void daEnPuchiPakkun_c::endState_Turn() {
    rot.y = nipperAngles[direction];
}

void daEnPuchiPakkun_c::beginState_Jump() {
    playChrAnim("jump", 0, 0.0, 1.0, true);
    //set Y speed for gravity
    y_speed_inc = -0.1875;
    max_speed.x = 0.0;
    max_speed.y = -4.0;
    max_speed.z = 0.0;

    speed.x = 0.0;
    if (isBahJump) {
        speed.y = 2.0;
    } else {
        switch (jumpHeight) {
            default:
                speed.y = 5.5; 
                break;
            case 1:
                speed.y = 4.5;
                break;
            case 2:
                speed.y = 3.5;
                break;
        }
    }
}

void daEnPuchiPakkun_c::executeState_Jump() {
    updateModel();
    HandleYSpeed();
    doSpriteMovement();

    if ((dEn_c_EnBgCheck(this)) & 1) {
        if (walks) {
            doStateChange(&StateID_Walk);
        } else {
            doStateChange(&StateID_Idle);
        }
    }

    DoStuffAndMarkDead(this, pos, 1.0);
}

void daEnPuchiPakkun_c::endState_Jump() {
    isBahJump = false;
}

void daEnPuchiPakkun_c::beginState_IceWait() {
    scale = (Vec3){0.89, 0.89, 0.89};
    anmChr.setCurrentFrame(0.0);
}

extern "C" fBase_c *getConnectChild(fBase_c *);
extern "C" fBase_c *getConnectBrNext(fBase_c *);

void daEnPuchiPakkun_c::executeState_IceWait() {
    u8 fireballState = 0;
    daFreezer_c *freezer;
    //make sure our AC_FREEZER still exists
    for (freezer = (daFreezer_c *)getConnectChild(this); freezer != (daFreezer_c *)NULL; freezer = (daFreezer_c *)getConnectBrNext(freezer)) {
        if (freezer != (daFreezer_c *)NULL) {
            //if it does, check if it's melting
            fireballState = (freezer->touchedByFire != 0) + 1;
        }
    }

    //we're MELTING
    if (fireballState == 2) {
        scale = (Vec3){1.0, 1.0, 1.0};
    }
    //our AC_FREEZER no longer exists, so we've melted
    if (fireballState == 0) {
        aPhysics.addToList();
        if (walks) {
            doStateChange(&StateID_Walk);
        } else {
            doStateChange(&StateID_Idle);
        }
    }
    
}

void daEnPuchiPakkun_c::endState_IceWait() { }