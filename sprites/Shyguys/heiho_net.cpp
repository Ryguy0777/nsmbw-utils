#include <daNetEnemy_c.h>
#include <profile.h>
#include <UnitType.h>
#include <sfx.h>

class daEnHeihoNet_c : public daNetEnemy_c {
    public:
        mHeapAllocator_c allocator;
        
        nw4r::g3d::ResFile resFile;
        nw4r::g3d::ResMdl bmdl;
		m3d::mdl_c model;
		m3d::anmChr_c anmChr;
        m3d::anmTexPat_c anmTexPat;

        //settings
        int color;
        bool vertical;
        bool attacks;

        u16 hhnAngles[2];

        float targetYPos;
        u32 some_dir;
        int anotherTurnTimer;
        int turnTimer;
        u16 turnStep;

        int attackTimer;

        int onCreate();
        int onDraw();

        void _vf5C();

        void playerCollision(ActivePhysics *apThis, ActivePhysics *apOther);

        void dieFall_Begin();
        void dieFall_Execute();

        bool CreateIceActors();

        void finalizeState_NetMove();
        
        void mdlPlay();
        void calcMdl();

        void loadModel();
        void playChrAnim(const char* name, int playsOnce, float unk2, float rate);
        void playClimbAnim();

        int getNetKind(Vec2 *offset);

        void setMoveState();

        void checkAttack();
        bool isPlayerInAttackRange();
        void attackPlayer();

        static dActor_c *build();

        USING_STATES(daEnHeihoNet_c);
        DECLARE_STATE(MoveUD);
        DECLARE_STATE(Turn_St);
        DECLARE_STATE(Turn_Ed);
        DECLARE_STATE(MoveLR);
        DECLARE_STATE(Turn);
        DECLARE_STATE(Attack);
};
CREATE_STATE(daEnHeihoNet_c, MoveUD);
CREATE_STATE(daEnHeihoNet_c, Turn_St);
CREATE_STATE(daEnHeihoNet_c, Turn_Ed);
CREATE_STATE(daEnHeihoNet_c, MoveLR);
CREATE_STATE(daEnHeihoNet_c, Turn);
CREATE_STATE(daEnHeihoNet_c, Attack);

dActor_c *daEnHeihoNet_c::build() {
	void *buffer = AllocFromGameHeap1(sizeof(daEnHeihoNet_c));
	return new(buffer) daEnHeihoNet_c;
}

const char* heihoNetArcList [] = {
	"heiho_net",
	NULL
};

const SpriteData heihoNetSpriteData = {ProfileId::EN_HEIHO_NET, 8, -16, 0, 8, 8, 8, 0, 0, 0, 0, 0};
Profile heihoNetProfile(&daEnHeihoNet_c::build, SpriteId::EN_HEIHO_NET, &heihoNetSpriteData, ProfileId::EN_NET_NOKONOKO_UD, ProfileId::EN_HEIHO_NET, "EN_HEIHO_NET", heihoNetArcList, 0x12);

extern "C" s8 getNearestPlayerYDirection(dStageActor_c *, VEC3 *);

int daEnHeihoNet_c::onCreate() {
    //set angles
    hhnAngles[0] = 0x8000;
    hhnAngles[1] = 0x0000;

    //sprite settings
    color = settings >> 24 & 0xF;
    attacks = settings >> 19 & 1;
    vertical = settings >> 18 & 1;
    appearsOnBackFence = settings >> 16 & 1;

    ActivePhysics::Info heihonet_cc;

    heihonet_cc.xDistToCenter = 0.0;
	heihonet_cc.yDistToCenter = 10.0;
	heihonet_cc.xDistToEdge = 8.0;
	heihonet_cc.yDistToEdge = 10.0;
	heihonet_cc.category1 = 0x3;
	heihonet_cc.category2 = 0x0;
	heihonet_cc.bitfield1 = 0x6F;
	heihonet_cc.bitfield2 = 0xffbafffe;
	heihonet_cc.unkShort1C = 0;
	heihonet_cc.callback = &dEn_c::collisionCallback;

    aPhysics.initWithStruct(this, &heihonet_cc);
    aPhysics.addToList();

	rot.y = hhnAngles[appearsOnBackFence];

    if (vertical) {
        direction = getNearestPlayerYDirection(this, &pos);
    } else {
        direction = dSprite_c__getXDirectionOfFurthestPlayerRelativeToVEC3(this, pos);
    }
    some_dir = direction;
    
    loadModel();

    _518 = 0;

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
    pos_delta2.y = 9.0;
    pos_delta2.z = 0.0;

    spriteSomeRectX = 28.0;
	spriteSomeRectY = 32.0;
	_320 = 0.0;
	_324 = 12.0;

    _36D = 2;

    //ice stuff
    flags_4FC |= 2;
    flags_4FC = 6;
    frzMgr.setSomething(0, 3, 3);

    setMoveState();

    return true;
}

int daEnHeihoNet_c::onDraw() {
    model.scheduleForDrawing();
    return true;
}

void daEnHeihoNet_c::_vf5C() {
    calcMdl();
    return;
}

void daEnHeihoNet_c::mdlPlay() {
    model._vf1C();
    return;
}

extern "C" void changePosAngle(VEC3 *, S16Vec *, int);
void daEnHeihoNet_c::calcMdl() {
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

extern "C" char usedForDeterminingStatePress_or_playerCollision(dEn_c* t, ActivePhysics *apThis, ActivePhysics *apOther, int unk1);
void daEnHeihoNet_c::playerCollision(ActivePhysics *apThis, ActivePhysics *apOther) {
    dStageActor_c *playerPtr = apOther->owner;
    if (apThis == &aPhysics) {
        char hitType = usedForDeterminingStatePress_or_playerCollision(this, apThis, apOther, 0);
        if (hitType == 0) {
            return dEn_c::playerCollision(apThis, apOther);
        } else if (hitType == 1) {
            Vec2 killSpeed;
            killSpeed.x = 0.0;
            killSpeed.y = 0.0;
            killWithSpecifiedState(playerPtr, &killSpeed, &StateID_DieFall, 1);
        } else if (hitType == 3) {
            return killByDieFall(playerPtr);
        }
    }
}

void daEnHeihoNet_c::dieFall_Begin() {
    playChrAnim("diefall", 0, 0.0, 1.0);
    return dEn_c::dieFall_Begin();
}

void daEnHeihoNet_c::dieFall_Execute() {
    mdlPlay();
    return dEn_c::dieFall_Execute();
}

extern "C" void sub_80024C20(void);
extern "C" void __destroy_arr(void*, void(*)(void), int, int);
//extern "C" __destroy_arr(struct DoSomethingCool, void(*)(void), int cnt, int bar);

float hhnZIceOffsets[2] = {1.5, -1.5};

bool daEnHeihoNet_c::CreateIceActors() {
	struct DoSomethingCool my_struct = { 0, {pos.x, pos.y-3.8, pos.z+hhnZIceOffsets[appearsOnBackFence]}, {1.3, 1.5, 1.6}, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
    frzMgr.Create_ICEACTORs( (void*)&my_struct, 1 );
    __destroy_arr( (void*)&my_struct, sub_80024C20, 0x3C, 1 );
    return true;
}

const char *hhnAnims[2] = {"climb_lr", "climb_ud"};

void daEnHeihoNet_c::loadModel() {
    allocator.link(-1, GameHeaps[0], 0, 0x20);

    resFile.data = getResource("heiho_net", "g3d/heiho.brres");
    bmdl = resFile.GetResMdl("heiho");
	model.setup(bmdl, &allocator, 0x227, 1, 0);
	SetupTextures_Enemy(&model, 0);

    nw4r::g3d::ResAnmChr resAnmChr = resFile.GetResAnmChr(hhnAnims[vertical]);
	anmChr.setup(bmdl, resAnmChr, &allocator, 0);
    playClimbAnim();

    nw4r::g3d::ResAnmTexPat resPat = resFile.GetResAnmTexPat("color");
	anmTexPat.setup(bmdl, resPat, &allocator, 0, 1);
	anmTexPat.bindEntry(&model, &resPat, 0, 1);
	anmTexPat.setFrameForEntry(color, 0);
	anmTexPat.setUpdateRateForEntry(0.0f, 0);
	model.bindAnim(&anmTexPat);

    allocator.unlink();
    return;
}

void daEnHeihoNet_c::playChrAnim(const char* name, int playsOnce, float unk, float rate) {
	nw4r::g3d::ResAnmChr resAnmChr = resFile.GetResAnmChr(name);
	anmChr.bind(&model, resAnmChr, playsOnce);
	model.bindAnim(&anmChr, unk);
	anmChr.setUpdateRate(rate);
}

void daEnHeihoNet_c::playClimbAnim() {
	nw4r::g3d::ResAnmChr resAnmChr = resFile.GetResAnmChr(hhnAnims[vertical]);
	anmChr.bind(&model, resAnmChr, 0);
	model.bindAnim(&anmChr, 0.0);
    //we can't use playmode to reverse the animation so we do this instead
    float rate = 0.91;
    if (direction == RIGHT && !vertical || direction == LEFT && vertical) {
        rate = -0.91;
    }
	anmChr.setUpdateRate(rate);
    return;
}

//things get messy here...

extern "C" void Actor_GetVec3WithValuesAdded(Vec *pos, dActor_c *actor);
int daEnHeihoNet_c::getNetKind(Vec2 *offset) {
    Vec this_pos;
    Actor_GetVec3WithValuesAdded(&this_pos, this);
    this_pos.x += offset->x;
    this_pos.y += offset->y;
    UnitType tileType = (UnitType)collMgr.getTileBehaviour1At(this_pos.x, this_pos.y, currentLayerID);
    if (tileType & FENCE) {
        u32 tileKind = collMgr.getTileBehaviour2At(this_pos.x, this_pos.y, currentLayerID);
        if ((tileKind & 0xFF) > 1) {
            return tileKind & 0xFF;
        }
    }
    return 0xFFFFFFFF;
}

void daEnHeihoNet_c::setMoveState() {
    if (vertical) {
        doStateChange(&StateID_MoveUD);
    } else {
        doStateChange(&StateID_MoveLR);
    }
}

void daEnHeihoNet_c::checkAttack() {
    if (attacks) {
        if (attackTimer > 0) {
            attackTimer--;
        } else {
            if (isPlayerInAttackRange()) {
                doStateChange(&StateID_Attack);
            }
        }
    }
}

bool daEnHeihoNet_c::isPlayerInAttackRange() {
    for (int i = 0; i < 4; i++) {
        dAcPy_c *player = dAcPy_c::findByID(i);
        if (player) {
            s8 player_chainlinkMode = (*((s8*)(((int)player)+0x1035))-1);
            if (player_chainlinkMode != appearsOnBackFence) {
                if (strcmp(player->states2.getCurrentState()->getName(), "dAcPy_c::StateID_Balloon")) {
                    if ((8.0 >= abs(player->pos.x - pos.x)) && (8.0 >= abs(player->pos.y - pos.y))) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

void daEnHeihoNet_c::attackPlayer() {
    for (int i = 0; i < 4; i++) {
        dAcPy_c *player = dAcPy_c::findByID(i);
        if (player) {
            s8 player_chainlinkMode = (*((s8*)(((int)player)+0x1035))-1);
            if (player_chainlinkMode != appearsOnBackFence) {
                if (strcmp(player->states2.getCurrentState()->getName(), "dAcPy_c::StateID_Balloon")) {
                    if ((16.0 >= abs(player->pos.x - pos.x)) && (16.0 >= abs(player->pos.y - pos.y))) {
                        _vf220(player);
                    }
                }
            }
        }
    }
}

float hhnZPositions[2] = {1500.0, -2500.0};
u8 chainLinkModes[2] = {1, 2};

void daEnHeihoNet_c::finalizeState_NetMove() {
    if (vertical) {
        appearsOnBackFence ^= 1;
        pos.z = hhnZPositions[appearsOnBackFence];
        aPhysics.chainlinkMode = chainLinkModes[appearsOnBackFence];
        collMgr.chainlinkMode = chainLinkModes[appearsOnBackFence];
        aPhysics.info.callback = &dEn_c::collisionCallback;
        return;
    } else {
        return daNetEnemy_c::finalizeState_NetMove();
    }
}

float hhnSpeeds[2] = {0.5, -0.5};

void daEnHeihoNet_c::beginState_MoveUD() {
    dStateBase_c *prevState = acState.getPreviousState();
    if (prevState != &StateID_NetWait) {
        if (prevState != &StateID_NetMove) {
            if (prevState != &StateID_Turn) {
                playClimbAnim();
            }
            speed.y = hhnSpeeds[direction];
            aPhysics.chainlinkMode = chainLinkModes[appearsOnBackFence];
            collMgr.chainlinkMode = chainLinkModes[appearsOnBackFence];
            pos.z = hhnZPositions[appearsOnBackFence];
            targetYPos = 0.0;
        }
    }
}

Vec2 hhnTileOffsetsUD[2] = {{0.0, -2.0}, {0.0, -14.0}};

void daEnHeihoNet_c::executeState_MoveUD() {
    mdlPlay();
    UpdateObjectPosBasedOnSpeedValuesReal();
    checkAttack();
    if (direction == RIGHT) {
        u32 aboveTile = collMgr.calculateAboveCollision(SENSOR_POINT);
        if (aboveTile == SENSOR_POINT) {
            int netCheck = getNetKind(&hhnTileOffsetsUD[direction]);
            if (netCheck == -1) {
                doStateChange(&StateID_Turn_St);
            }
        } else {
            // never seems to get called?
            direction = LEFT;
            some_dir = 1;
            pos.y = last_pos.y;
            speed.y = -speed.y;
            anmChr.setUpdateRate(-0.91);
        }
    } else {
        int netCheck = getNetKind(&hhnTileOffsetsUD[direction]);
        if (netCheck == -1) {
            pos.y = last_pos.y;
            direction = RIGHT;
            some_dir = 0;
            speed.y = -speed.y;
            anmChr.setUpdateRate(0.91);
        }
    }
}

void daEnHeihoNet_c::endState_MoveUD() { }

void daEnHeihoNet_c::beginState_Turn_St() {
    dStateBase_c *prevState = acState.getPreviousState();
    if (prevState != &StateID_NetWait) {
        if (prevState != &StateID_NetMove) {
            if (prevState == &StateID_MoveUD) {
                playChrAnim("turn_st", 1, 0.0, anmChr.getUpdateRate());
                speed.x = 0.0;
                direction ^= 1;
                appearsOnBackFence ^= 1;
                targetYPos = pos.y;
                aPhysics.chainlinkMode = chainLinkModes[appearsOnBackFence];
                collMgr.chainlinkMode = chainLinkModes[appearsOnBackFence];
            }
        }
    }
}

void daEnHeihoNet_c::executeState_Turn_St() {
    mdlPlay();
    if(anmChr.isAnimationDone()) {
        doStateChange(&StateID_Turn_Ed);
    }
}

void daEnHeihoNet_c::endState_Turn_St() { }

void daEnHeihoNet_c::beginState_Turn_Ed() {
    dStateBase_c *prevState = acState.getPreviousState();
    if (prevState != &StateID_NetWait) {
        if (prevState != &StateID_NetMove) {
            if (prevState == &StateID_Turn_St) {
                playChrAnim("turn_ed", 1, 0.0, 0.0);
                speed.y = 2.2;
            }
        }
    }
}

s8 turnByteStep[2] = {1, 0xFF};
short turnStepAmt[2] = {0x400, 0xFC00};

void daEnHeihoNet_c::executeState_Turn_Ed() {
    mdlPlay();
    short rotThing = (turnStepAmt[direction] * turnByteStep[appearsOnBackFence]);
    if (rot.y != hhnAngles[appearsOnBackFence]) {
        rotThing += rot.y;
        rot.y = rotThing;
        if (rotThing == 0x4000 || rotThing == -0x4000) {
            pos.z = hhnZPositions[appearsOnBackFence];
        }
    }
    HandleYSpeed();
    UpdateObjectPosBasedOnSpeedValuesReal();
    if (pos.y < targetYPos) {
        anmChr.setUpdateRate(0.91);
        pos.y = targetYPos;
        rot.y = hhnAngles[appearsOnBackFence];
        if (anmChr.isAnimationDone()) {
            doStateChange(&StateID_MoveUD);
        }
    }
}

void daEnHeihoNet_c::endState_Turn_Ed() { }

void daEnHeihoNet_c::beginState_MoveLR() {
    dStateBase_c *prevState = acState.getPreviousState();
    if (prevState != &StateID_NetWait) {
        if (prevState != &StateID_NetMove) {
            if (prevState != &StateID_Turn) {
                playClimbAnim();
            }
            speed.x = hhnSpeeds[direction];
            aPhysics.chainlinkMode = chainLinkModes[appearsOnBackFence];
            collMgr.chainlinkMode = chainLinkModes[appearsOnBackFence];
            pos.z = hhnZPositions[appearsOnBackFence];
            targetYPos = 0.0;
        }
    }
}

Vec2 hhnTileOffsetsLR[2] = {{8.0, 0.0}, {-8.0, 0.0}};

void daEnHeihoNet_c::executeState_MoveLR() {
    mdlPlay();
    UpdateObjectPosBasedOnSpeedValuesReal();
    collMgr.calculateAdjacentCollision((float *)0x0);
    if ((collMgr.outputMaybe & 0x15 << (direction & 0x3F)) == 0) {
        checkAttack();
        int netCheck = getNetKind(&hhnTileOffsetsLR[direction]);
        if (netCheck == -1) {
            pos.x = last_pos.x;
            doStateChange(&StateID_Turn);
        }
    } else {
        direction ^= 1;
        some_dir ^= 1;
        targetYPos = 0.0;
        speed.x = -speed.x;
    }
}

void daEnHeihoNet_c::endState_MoveLR() { }

void daEnHeihoNet_c::beginState_Turn() {
    dStateBase_c *prevState = acState.getCurrentState();
    if (prevState != &StateID_NetWait) {
        if (prevState != &StateID_NetMove) {
            s8 turnByte = turnByteStep[some_dir];
            int turnTime = (int)(short)(int)(10.0 / abs(speed.x * 0.8)) << 1;
            anotherTurnTimer = turnTime;
            turnTimer = turnTime;
            turnStep = (short)turnByte * (short)(0x8000 / turnTime);
        }
    }
}

void daEnHeihoNet_c::executeState_Turn() {
    mdlPlay();
    UpdateObjectPosBasedOnSpeedValuesReal();
    turnTimer--;
    rot.y += turnStep;
    if (turnTimer == 0) {
        rot.y = hhnAngles[appearsOnBackFence];
        doStateChange(&StateID_MoveLR);
    } else if (turnTimer == anotherTurnTimer >> 1) {
        direction ^= 1;
        rot.y = (short)((turnByteStep[direction]) << 0xe);
        speed.x = -speed.x;
        appearsOnBackFence ^= 1;
        pos.z = hhnZPositions[appearsOnBackFence];
        aPhysics.chainlinkMode = chainLinkModes[appearsOnBackFence];
        collMgr.chainlinkMode = chainLinkModes[appearsOnBackFence];
    }
}

void daEnHeihoNet_c::endState_Turn() { }

void daEnHeihoNet_c::beginState_Attack() {
    attackTimer = 150;
    playChrAnim("attack", 1, 0.0, 1.0);
    speed.x = 0.0;
    speed.y = 0.0;
}

extern "C" void MtxToVec(Mtx *, Vec3 *);

void daEnHeihoNet_c::executeState_Attack() {
    mdlPlay();
    if (anmChr.getCurrentFrame() == 10.0) {
        nw4r::g3d::ResNode hand = bmdl.GetResNode("attach_R_hand");
		int handNodeID = hand.GetID();
        Mtx handMtx;
		model.getMatrixForNode(handNodeID, &handMtx);
        Vec3 handPos;
        MtxToVec(&handMtx, &handPos);

        SpawnEffect("Wm_mr_wirehit", 0, &handPos, (S16Vec *)0, (Vec *)0);
        nw4r::snd::SoundHandle handle;
        PlaySoundWithFunctionB4(SoundRelatedClass, &handle, SE_PLY_ATTACK_FENCE, 0);
    }

    if (anmChr.getCurrentFrame() > 10.0) {
        attackPlayer();
    }

    if (anmChr.isAnimationDone()) {
        setMoveState();
    }
}

void daEnHeihoNet_c::endState_Attack() { }