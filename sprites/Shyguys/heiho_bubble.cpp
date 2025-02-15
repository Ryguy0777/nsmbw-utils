#include <heiho.h>

class daEnHeihoBubble_c : public daEnHeiho_c {
    public:
        bool isBubble;
        bool popsSensor;
        u8 bubbleMovement; // 0 = none, 1 = horizontal, 2 = vertical, 3 = circular
        u8 bubbleDistance;

        Vec2 bubbleBaseline;

        nw4r::g3d::ResFile bubbleRes;
        m3d::mdl_c bubbleModel;
		m3d::anmChr_c bubbleAnim;
        m3d::anmTexSrt_c bubbleSrt;

        ActivePhysics bubbleCollider;

        dStageActor_c *playerPtr;

        int onCreate();

        void playerCollision(ActivePhysics *apThis, ActivePhysics *apOther);
	    void yoshiCollision(ActivePhysics *apThis, ActivePhysics *apOther);

        bool collisionCat3_StarPower(ActivePhysics *apThis, ActivePhysics *apOther);

        bool collisionCatD_Drill(ActivePhysics *apThis, ActivePhysics *apOther);

        bool collisionCat7_GroundPound(ActivePhysics *apThis, ActivePhysics *apOther);
        bool collisionCat7_GroundPoundYoshi(ActivePhysics *apThis, ActivePhysics *apOther);

	    bool collisionCat1_Fireball_E_Explosion(ActivePhysics *apThis, ActivePhysics *apOther);
	    bool collisionCat2_IceBall_15_YoshiIce(ActivePhysics *apThis, ActivePhysics *apOther);
 
        void dieFall_Begin();

        void eatIn_Begin();

        bool CreateIceActors();

        void addScoreWhenHit(void *other);
	    void _vf260(void *other); // fumiSE
	    void _vf264(dStageActor_c *other); // fumiEffect
	    void _vf268(void *other); //spinfumiSE
	    void spawnHitEffectAtPositionAgain(Vec2 pos); //spinfumiEffect

	    void _vf278(void *other); // yoshifumiSE
	    void _vf27C(); // yoshifumiEffect

        void calcModel();

        void loadModel();

        void drawModel();

        void initialize();
        void vf2D0(dStageActor_c* collidingActor);

        void updateModel();

        void popBubble(dStageActor_c *killer);
        void popIfPlayerBelow();

        static dActor_c *build();

        USING_STATES(daEnHeihoBubble_c);
        DECLARE_STATE(Bubble);
        DECLARE_STATE(BubbleFall);
};

CREATE_STATE(daEnHeihoBubble_c, Bubble);
CREATE_STATE(daEnHeihoBubble_c, BubbleFall);

dActor_c *daEnHeihoBubble_c::build() {
	void *buffer = AllocFromGameHeap1(sizeof(daEnHeihoBubble_c));
	return new(buffer) daEnHeihoBubble_c;
}

const char* heihoBubbleArcList [] = {
	"heiho",
	NULL
};

const SpriteData heihoBubbleSpriteData = {ProfileId::EN_HEIHO_BALLOON, 8, -16, 0, 8, 8, 8, 0, 0, 0, 0, 0};
Profile heihoBubbleProfile(&daEnHeihoBubble_c::build, SpriteId::EN_HEIHO_BALLOON, &heihoBubbleSpriteData, ProfileId::EN_KURIBO, ProfileId::EN_HEIHO_BALLOON, "EN_HEIHO_BALLOON", heihoBubbleArcList, 0x12);

int daEnHeihoBubble_c::onCreate() {
    //bubble stuff
    
    
    return daEnHeiho_c::onCreate();
}

extern "C" char usedForDeterminingStatePress_or_playerCollision(dEn_c* t, ActivePhysics *apThis, ActivePhysics *apOther, int unk1);
extern "C" bool CallXCompareFuncLT(float, float);

void daEnHeihoBubble_c::playerCollision(ActivePhysics *apThis, ActivePhysics *apOther) {
    playerPtr = apOther->owner;
    if (apThis == &bubbleCollider) {
        char hitType = usedForDeterminingStatePress_or_playerCollision(this, apThis, apOther, 0);
        popBubble(apOther->owner);
        return;
    }
    return daEnKuriboBase_c::playerCollision(apThis, apOther);
}

void daEnHeihoBubble_c::yoshiCollision(ActivePhysics *apThis, ActivePhysics *apOther) {
    if (apThis == &bubbleCollider) {
        char hitType = usedForDeterminingStatePress_or_playerCollision(this, apThis, apOther, 0);
        popBubble(apOther->owner);
        return;
    }
    return daEnKuriboBase_c::yoshiCollision(apThis, apOther);
}

bool daEnHeihoBubble_c::collisionCat3_StarPower(ActivePhysics *apThis, ActivePhysics *apOther) {
    if (apThis == &bubbleCollider) {
        popBubble(apOther->owner);
        return true;
    }
    return daEnKuriboBase_c::collisionCat3_StarPower(apThis, apOther);
}

bool daEnHeihoBubble_c::collisionCatD_Drill(ActivePhysics *apThis, ActivePhysics *apOther) {
    if (apThis == &bubbleCollider) {
        popBubble(apOther->owner);
        return false;
    }
    return daEnKuriboBase_c::collisionCatD_Drill(apThis, apOther);
}

bool daEnHeihoBubble_c::collisionCat7_GroundPound(ActivePhysics *apThis, ActivePhysics *apOther) {
    if (apThis == &bubbleCollider) {
        popBubble(apOther->owner);
        return false;
    }
    return daEnKuriboBase_c::collisionCat7_GroundPound(apThis, apOther);
}

bool daEnHeihoBubble_c::collisionCat7_GroundPoundYoshi(ActivePhysics *apThis, ActivePhysics *apOther) {
    if (apThis == &bubbleCollider) {
        popBubble(apOther->owner);
        return false;
    }
    return daEnKuriboBase_c::collisionCat7_GroundPoundYoshi(apThis, apOther);
}

bool daEnHeihoBubble_c::collisionCat1_Fireball_E_Explosion(ActivePhysics *apThis, ActivePhysics *apOther) {
    if (apThis == &bubbleCollider) {
        popBubble(apOther->owner);
        return true;
    }
    return daEnKuriboBase_c::collisionCat1_Fireball_E_Explosion(apThis, apOther);
}

bool daEnHeihoBubble_c::collisionCat2_IceBall_15_YoshiIce(ActivePhysics *apThis, ActivePhysics *apOther) {
    if (apThis == &bubbleCollider) {
        popBubble(apOther->owner);
        return true;
    }
    return daEnKuriboBase_c::collisionCat2_IceBall_15_YoshiIce(apThis, apOther);
}

void daEnHeihoBubble_c::dieFall_Begin() {
    if (isBubble) {
        popBubble(NULL);
    }
    playChrAnim("diefall", 0, 0.0, 2.0);
    return dEn_c::dieFall_Begin();
}

void daEnHeihoBubble_c::eatIn_Begin() {
    if (isBubble) {
        popBubble(NULL);
    }
    return dEn_c::eatIn_Begin();
}

extern "C" void sub_80024C20(void);
extern "C" void __destroy_arr(void*, void(*)(void), int, int);
//extern "C" __destroy_arr(struct DoSomethingCool, void(*)(void), int cnt, int bar);

bool daEnHeihoBubble_c::CreateIceActors() {
    if (isBubble) {
        popBubble(NULL);
    }
	struct DoSomethingCool my_struct = { 0, {pos.x, pos.y-2.3, pos.z}, {1.3, 1.5, 1.5}, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
    frzMgr.Create_ICEACTORs( (void*)&my_struct, 1 );
    __destroy_arr( (void*)&my_struct, sub_80024C20, 0x3C, 1 );
    return true;
}

void daEnHeihoBubble_c::addScoreWhenHit(void *other) {
    if (isBubble) {
        return;
    } else {
        return dEn_c::addScoreWhenHit(other);
    }
}

void daEnHeihoBubble_c::_vf260(void *other) {
    if (isBubble) {
        return;
    } else {
        return dEn_c::_vf260(other);
    }
}

void daEnHeihoBubble_c::_vf264(dStageActor_c *other) {
    if (isBubble) {
        return;
    } else {
        return dEn_c::_vf264(other);
    }
}

void daEnHeihoBubble_c::_vf268(void *other) {
    if (isBubble) {
        return;
    } else {
        return dEn_c::_vf268(other);
    }
}

// reimplementation because ninji/tempus got params wrong
void daEnHeihoBubble_c::spawnHitEffectAtPositionAgain(Vec2 pos) {
    if (isBubble) {
        return;
    } else {
        Vec effectPos;
        effectPos.x = playerPtr->pos.x;
        effectPos.y = playerPtr->pos.y;
        effectPos.z = 5500.0;
        SpawnEffect("Wm_en_hit", 0, &effectPos, (S16Vec *)0, (Vec *)0);
        return;
    }
}

void daEnHeihoBubble_c::_vf278(void *other) {
    if (isBubble) {
        return;
    } else {
        return dEn_c::_vf278(other);
    }
}

void daEnHeihoBubble_c::_vf27C() {
    if (isBubble) {
        return;
    } else {
        return dEn_c::_vf27C();
    }
}

void daEnHeihoBubble_c::calcModel() {
    daEnKuriboBase_c::calcModel();
    if (isBubble) {
        mMtx bubbleMatrix = matrix;
        bubbleMatrix.translation(pos.x, pos.y+10.0, pos.z+60.0);
        bubbleModel.setDrawMatrix(bubbleMatrix);
        bubbleModel.setScale(1.0, 1.0, 1.0);
        bubbleModel.calcWorld(false);
    }
}

void daEnHeihoBubble_c::loadModel() {
    bubbleRes.data = getResource("balloon", "g3d/balloon.brres");
    nw4r::g3d::ResMdl balloonmdl = bubbleRes.GetResMdl("balloon");
	bubbleModel.setup(balloonmdl, &allocator, 0x20, 1, 0);

	nw4r::g3d::ResAnmChr resAnmBub = bubbleRes.GetResAnmChr("float");
	bubbleAnim.setup(balloonmdl, resAnmBub, &allocator, 0);
	bubbleAnim.bind(&bubbleModel, resAnmBub, 0);
	bubbleModel.bindAnim(&bubbleAnim, 0.0);
	bubbleAnim.setUpdateRate(1.0);

    nw4r::g3d::ResAnmTexSrt resSrtBub = bubbleRes.GetResAnmTexSrt("float");
    bubbleSrt.setup(balloonmdl, resSrtBub, &allocator, 0, 1);
	bubbleSrt.bindEntry(&bubbleModel, resSrtBub, 0, 4);
    bubbleModel.bindAnim(&bubbleSrt, 1.0);
    bubbleSrt.setUpdateRateForEntry(1.0, 0);

    return daEnHeiho_c::loadModel();
}

void daEnHeihoBubble_c::drawModel() {
    model.scheduleForDrawing();
    if (isBubble) {
        bubbleModel.scheduleForDrawing();
    }
}

void daEnHeihoBubble_c::initialize() {
    isBubble = true;
    popsSensor = settings >> 23 & 1;
    bubbleMovement = settings >> 21 & 3;
    bubbleDistance = settings >> 4 & 0xF;

    type = (heyhoTypes)(settings >> 28 & 0xF);
    color = settings >> 24 & 0xF;
    health = settings >> 17 & 1;
    distance = settings >> 12 & 0xF;
    spawnDir = (settings >> 8 & 1)^1;

    anmTexPat.setFrameForEntry(color, 0);

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

    doStateChange(&StateID_Bubble);
    return;
}

void daEnHeihoBubble_c::vf2D0(dStageActor_c* collidingActor) {
    bool directionThing = CallXCompareFuncLT(collidingActor->pos.x + collidingActor->pos_delta2.x, pos.x + pos_delta2.x);
    dStateBase_c *state = acState.getCurrentState();
    if (state != &StateID_Bubble && state != &StateID_BubbleFall && state != &StateID_Sleep && state != &StateID_Dizzy) {
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

void daEnHeihoBubble_c::updateModel() {
    model._vf1C();
    if (isBubble) {
        bubbleModel._vf1C();
        bubbleSrt.process();
    }
}

extern "C" void startEnemySound(void*,SFX,Vec2*,int);
extern "C" void cvtSndObjctPos(Vec2 *out, Vec *stage_pos);
extern void *SoundClassRelated;

void daEnHeihoBubble_c::popBubble(dStageActor_c *killer) {
    isBubble = false;
    SpawnEffect("Wm_mr_balloonburst", 0, &(Vec){pos.x, pos.y + 10, 0}, (S16Vec *)0, (Vec *)0);
    Vec2 soundPos;
    cvtSndObjctPos(&soundPos, &pos);
    startEnemySound(SoundClassRelated, SE_PLY_BALLOON_BRAKE, &soundPos, 0);
    bubbleCollider.removeFromList();
    if (killer) {
        if (type < Jumper || type > Pacer) {
            bool directionThing = CallXCompareFuncLT(killer->pos.x + killer->pos_delta2.x, pos.x + pos_delta2.x);
            direction = directionThing;
        }
        doStateChange(&StateID_BubbleFall);
    }
}

void daEnHeihoBubble_c::popIfPlayerBelow() {
    for (int i = 0; i < 4; i++) {
        dAcPy_c *player = dAcPy_c::findByID(i);
        //grab any active players
        if (player) {
            //make sure they're not bubbled
            if (strcmp(player->states2.getCurrentState()->getName(), "dAcPy_c::StateID_Balloon")) {
                //are we in the same x range as the shyguy?
                if (48.0 >= abs(player->pos.x - pos.x)) {
                    //are we in the y range?
                    if (pos.y >= player->pos.y && player->pos.y >= pos.y - 104.0) {
                        popBubble(player);
                    }
                }
            }
        }
    }
}

extern "C" u8 dEn_c_EnBgCheck(dEn_c *self); //checks if we're on top of a tile
extern "C" bool CheckDanceValues_Bahps(u32 param); //checks for bahs
extern "C" void DoStuffAndMarkDead(dStageActor_c *actor, Vec vector, float unk); //kills if touching lava
extern "C" int SmoothRotation(short* rot, u16 amt, int unk2); //rotates sprite until amt, returns true when finished

void daEnHeihoBubble_c::beginState_Bubble() {
    if (bubbleMovement > 0) {
        playChrAnim("bubble2", 0, 0.0, 1.0);
    } else {
        playChrAnim("bubble", 0, 0.0, 1.0);
    }

    rot.y = 0;

    bubbleBaseline.x = pos.x;
    bubbleBaseline.y = pos.y;
    timer = 0;

    ActivePhysics::Info bubble_cc;
    bubble_cc.xDistToCenter = 0.0;
	bubble_cc.yDistToCenter = 10.5;
	bubble_cc.xDistToEdge = 15.0;
	bubble_cc.yDistToEdge = 15.0;
	bubble_cc.category1 = 4;
	bubble_cc.category2 = 0;
	bubble_cc.bitfield1 = 0x5F;
	bubble_cc.bitfield2 = 0xFF80A086; //0xff82fffe;
	bubble_cc.unkShort1C = 0;
	bubble_cc.callback = &dEn_c::collisionCallback;
    bubbleCollider.initWithStruct(this, &bubble_cc);
    bubbleCollider.addToList();
}

void daEnHeihoBubble_c::executeState_Bubble() {
    updateModel();

    if (bubbleMovement == 0) {
        //make bubble bob up and down very slightly
        pos.y = bubbleBaseline.y + ( sin(timer * 3.14 / 60.0) * 1.0 );
    } else if (bubbleMovement == 1) { //horizontal
        //make bubble bob up and down
        pos.y = bubbleBaseline.y + ( sin(timer * 3.14 / 60.0) * 3.0 );
        //make bubble move to destination
        pos.x = bubbleBaseline.x + ( sin(timer * 3.14 / 600.0) * (float)bubbleDistance * 16.0);
    } else if (bubbleMovement == 2) { //vertical
        //make bubble move to destination
        pos.x = bubbleBaseline.x + ( sin(timer * 3.14 / 60.0) * 3.0 );
        //make bubble bob side to side
        pos.y = bubbleBaseline.y + ( sin(timer * 3.14 / 600.0) * (float)bubbleDistance * 16.0);
    } else if (bubbleMovement == 3) { //circular
        pos.x = bubbleBaseline.x + ( sin(timer * 3.14 / 600.0) * (float)bubbleDistance * 16.0);
        pos.y = bubbleBaseline.y + ( cos(timer * 3.14 / 600.0) * (float)bubbleDistance * 16.0);
    }

    if (popsSensor) {
        popIfPlayerBelow();
    }

	timer++;

    DoStuffAndMarkDead(this, pos, 1.0);
}

void daEnHeihoBubble_c::endState_Bubble() { }

void daEnHeihoBubble_c::beginState_BubbleFall() {
    playChrAnim("fall", 0, 0.0, 1.0);
    speed.x = 0.0;

    y_speed_inc = -0.1875;
    max_speed.x = 0.0;
    max_speed.y = -4.0;
    max_speed.z = 0.0;
}

void daEnHeihoBubble_c::executeState_BubbleFall() {
    updateModel();
    HandleYSpeed();
    doSpriteMovement();

	if (dEn_c_EnBgCheck(this) & 1) {
        //set walking distances for pacer now that we're on the ground
        if (type == Pacer) {
            float finalOffset = 16.0 * distance - 8.0;
            finalPos[0] = pos.x + finalOffset;
            finalPos[1] = pos.x - finalOffset;
        }
        switch (type) {
            case Walker:
            case WalkerLedge:
            case Pacer:
                rot.y = hhAngles[direction];
                doStateChange(&StateID_Walk);
                break;
            case Sleeper:
                doStateChange(&StateID_Sleep);
                break;
            case Jumper:
                rot.y = hhAngles[direction];
                doStateChange(&StateID_Jump);
                break;
            default:
                rot.y = hhAngles[direction];
                doStateChange(&StateID_Idle);
                break;
        }
    }

    DoStuffAndMarkDead(this, pos, 1.0);
}

void daEnHeihoBubble_c::endState_BubbleFall() { }