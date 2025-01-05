#include <daEnKuribo_c.h>
#include <profile.h>
#include <sfx.h>

class daEnPataKuribon_c : public daEnPataKuribo_c {
    public:
        int flipTimer;
        int carriedByPlayerID;
        Vec carryVec;
        float apRestoreXDist;
        float apRestoreYDist;
        float apRestoreXEdge;
        float apRestoreYEdge;

        dStageActor_c *spinLiftUpPlayer;

        int onCreate();

        int _vf74();
        void itemPickedUp();

        void spriteCollision(ActivePhysics *apThis, ActivePhysics *apOther);
        void playerCollision(ActivePhysics *apThis, ActivePhysics *apOther);
        void yoshiCollision(ActivePhysics *apThis, ActivePhysics *apOther);

        void dieOther_Begin();
		void dieOther_Execute();
		void dieOther_End();

        void initializeState_Walk();

        void calcModel();
        void reactFumiProc(dStageActor_c* killingActor);
        void loadModel();
        void loadWingModel();

        void updateModel();
        void playWalkAnim();

        void doCarryCollider();
        void killWhenCarried(dAcPy_c *player);
        void checkTimer();
        void playKickSound(int player_id);
        
        static dActor_c *build();

        USING_STATES(daEnPataKuribon_c);
        DECLARE_STATE(Dizzy);
        DECLARE_STATE(Carry);
        DECLARE_STATE(Throw);
        DECLARE_STATE(Kick);
        DECLARE_STATE(Recover);
};
CREATE_STATE(daEnPataKuribon_c, Dizzy);
CREATE_STATE(daEnPataKuribon_c, Carry);
CREATE_STATE(daEnPataKuribon_c, Throw);
CREATE_STATE(daEnPataKuribon_c, Kick);
CREATE_STATE(daEnPataKuribon_c, Recover);

dActor_c *daEnPataKuribon_c::build() {
	void *buffer = AllocFromGameHeap1(sizeof(daEnPataKuribon_c));
	daEnPataKuribon_c *c = new(buffer) daEnPataKuribon_c;
    c->hasWings = 1;
    return c;
}

const char* PataGaloombaArcList[] = {
    "kuribon",
    "kuribon_wing",
    NULL
};

const SpriteData PataGaloombaSpriteData = {ProfileId::EN_PATA_KURIBON, 8, 0xFFFFFFF0, 0, 8, 8, 8, 0, 0, 0, 0, 0};
Profile PataGaloombaProfile(&daEnPataKuribon_c::build, SpriteId::EN_PATA_KURIBON, &PataGaloombaSpriteData, ProfileId::EN_PATA_KURIBO, ProfileId::EN_PATA_KURIBON, "EN_PATA_KURIBON", PataGaloombaArcList, 0x12);

int daEnPataKuribon_c::onCreate() {
    createModel();

    ActivePhysics::Info kuribon_cc;

    kuribon_cc.xDistToCenter = 0.0;
	kuribon_cc.yDistToCenter = 8.0;
	kuribon_cc.xDistToEdge = 8.0;
	kuribon_cc.yDistToEdge = 8.0;
	kuribon_cc.category1 = 0x3;
	kuribon_cc.category2 = 0x0;
	kuribon_cc.bitfield1 = 0x6F;
	kuribon_cc.bitfield2 = 0xFFBEFFFE;
	kuribon_cc.unkShort1C = 0;
	kuribon_cc.callback = &dEn_c::collisionCallback;

    aPhysics.initWithStruct(this, &kuribon_cc);
    aPhysics.addToList();
    
    max_speed.y = -4.0;

    s16 angles[2] = {0x2000, 0xE000};
    direction = dSprite_c__getXDirectionOfFurthestPlayerRelativeToVEC3(this, pos);
	rot.y = angles[direction];

    appearsOnBackFence = settings >> 16 & 1;
    zOffset = 0.0;
    chkZoneBoundParam = 0;
    _518 = 0;

    initialize();
    
    return true;
}

int daEnPataKuribon_c::_vf74() {
    dStateBase_c *state = acState.getCurrentState();
    bool isStateEqual = state->isEqual(&StateID_Dizzy);
    return isStateEqual;
}

void daEnPataKuribon_c::itemPickedUp() {
    u8 *playerNum = spinLiftUpPlayer->_vf6C();
    u32 player_id = (u32)*playerNum;
    carriedByPlayerID = player_id;
    carryVec.x = 0.0;
    carryVec.y = 0.0;
    carryVec.z = 0.0;
    doStateChange(&StateID_Carry);
}

void daEnPataKuribon_c::spriteCollision(ActivePhysics *apThis, ActivePhysics *apOther) {
    u16 targetFlags = apOther->info.unkShort1C;
    dActor_c *target = apOther->owner;
    //if we're throwing or kicking, kill the colliding actor
    if (apThis->info.category2 == 9 && (targetFlags & 0x100) && collisionCat9_RollingObject(apThis, apOther)) {
        apOther->someFlagByte = apOther->someFlagByte | 2;
        apThis->someFlagByte = apThis->someFlagByte | 2;
        return;
    }
    //if we're carrying, kill both actors
    dStateBase_c *currentState = acState.getCurrentState();
    if (currentState == &StateID_Carry && (apOther->info.bitfield2 & 0x200) && target->name != ProfileId::EN_HATENA_BALLOON && collisionCat9_RollingObject(apThis, apOther)) {
        apOther->someFlagByte = apOther->someFlagByte | 2;
        return;
    }
    //if we're walking, turn
    if (currentState != &StateID_Walk) {
        if (currentState != &StateID_Pata_Walk) {
            return dEn_c::spriteCollision(apThis, apOther);
        } else {
            return daEnPataKuribo_c::spriteCollision(apThis, apOther);
        }
    } else {
        return daEnPataKuribo_c::spriteCollision(apThis, apOther);
    }
}

extern "C" char usedForDeterminingStatePress_or_playerCollision(dEn_c* t, ActivePhysics *apThis, ActivePhysics *apOther, int unk1);

extern "C" bool isPlayerStatus(daPlBase_c *self, u32 status);
extern "C" bool setPlayerCarry(dEn_c *self, dAcPy_c *player);
extern "C" Vec ConvertStagePositionIntoScreenPosition__Maybe(Vec);
extern "C" void Actor_GetVec3WithValuesAdded(Vec *pos, dActor_c *actor);
extern "C" u32 getRemotePlayer(int player_id);
extern "C" void cvtSndObjctPos(Vec2 *out, Vec *stage_pos);
extern void *SoundClassRelated;
extern "C" void startEnemySound(void*,SFX,Vec2*,int);

//ghidra's decomp looked like it was driving on the freeway in the fast lane with a rabid wolverine in it's underwear...
//not explaining this because I'm not really even sure how it works
void daEnPataKuribon_c::playerCollision(ActivePhysics *apThis, ActivePhysics *apOther) {
    spinLiftUpPlayer = apOther->owner;
    dAcPy_c *player = (dAcPy_c *)apOther->owner;
    dEn_c *playerCarry = (dEn_c *)fBase_c::searchById(*((int*)(((int)player)+0x2A78)));
    if (playerCarry != this) {
        bool something = apThis->firstFloatArray[0] < 0.0; // what the FUCK is this
        u8 *playerNum = player->_vf6C();
        u32 player_id = (u32)*playerNum;
        dStateBase_c *currentState = acState.getCurrentState();
        if (currentState != &StateID_Dizzy) {
            if (currentState != &StateID_Throw) {
                if (currentState != &StateID_Kick) {
                    u8 hitType = usedForDeterminingStatePress_or_playerCollision(this, apThis, apOther, false);
                    if (hitType == 1) {
                        which_player = *playerNum;
                        collMgr.whichPlayerOfParent____ = *playerNum;
                        if (acState.getCurrentState() == &StateID_Carry) { //dizzy?
                            doStateChange(&StateID_Kick);
                            return;
                        }
                        return reactFumiProc(apOther->owner);
                    } else if (hitType == 3) {
                        return reactSpinFumiProc(apOther->owner);
                    } else if (hitType == 0) {
                        if (acState.getCurrentState() == &StateID_Carry) {
                            return;
                        }
                        return dEn_c::playerCollision(apThis, apOther);
                    } else {
                        return;
                    }
                }
            }
            direction = something;
            counter_504[player_id] = 0x20;
            playKickSound(player_id);
            which_player = *playerNum;
            collMgr.whichPlayerOfParent____ = *playerNum;
            doStateChange(&StateID_Kick);
        } else {
            // EVERYTHING I KNOW IS WRONG
            if (setPlayerCarry(this, player) == 0) {
                direction = something;
                counter_504[player_id] = 0x20;
                playKickSound(player_id);
                which_player = *playerNum;
                collMgr.whichPlayerOfParent____ = *playerNum;
                doStateChange(&StateID_Kick);
            } else {
                carriedByPlayerID = player_id;
                carryVec.x = 0.0;
                carryVec.y = -5.0;
                carryVec.z = 6.0;
                doStateChange(&StateID_Carry);
            }
        }
    }
}

void daEnPataKuribon_c::yoshiCollision(ActivePhysics *apThis, ActivePhysics *apOther) {
    float something = apThis->firstFloatArray[2]; // what the FUCK is this
    dStageActor_c *yoshi = apOther->owner;
    u8 *playerNum = yoshi->_vf6C();
    u32 player_id = (u32)*playerNum;
    u8 hitType = usedForDeterminingStatePress_or_playerCollision(this, apThis, apOther, false);
    if (hitType == 0) {
        dStateBase_c *currentState = acState.getCurrentState();
        if (currentState != &StateID_Dizzy) {
            if (currentState != &StateID_Throw) {
                if (currentState != &StateID_Kick) {
                    return daEnKuriboBase_c::yoshiCollision(apThis, apOther);
                }
            }
        }
        if (player_id < 4) {
            direction = something < 0.0;
            which_player = *playerNum;
            counter_504[player_id] = 0x20;
            collMgr.whichPlayerOfParent____ = *playerNum;
            playKickSound(player_id);
            doStateChange(&StateID_Kick);
        }
    } else {
        reactYoshiFumiProc(yoshi);
    }
}

void daEnPataKuribon_c::dieOther_Begin() {
    return dEn_c::dieOther_Begin();
}

void daEnPataKuribon_c::dieOther_Execute() {
	return dEn_c::dieOther_Execute();
}

void daEnPataKuribon_c::dieOther_End() {
	return dEn_c::dieOther_End();
}

void daEnPataKuribon_c::initializeState_Walk() {
    dStateBase_c *prevState = acState.getPreviousState();
    if (prevState != &StateID_Turn) {
        playWalkAnim();
    }
    setWalkSpeed();
    //set Y speed for gravity
    y_speed_inc = -0.1875;
    max_speed.x = 0.0;
    max_speed.y = -4.0;
    max_speed.z = 0.0;
}

//setting the carry position is done *after* everything else has executed, to prevent lag
extern "C" Vec *calcCarryPos(Vec*,dEn_c*,Vec*);

void daEnPataKuribon_c::calcModel() {
    Vec calcedPos;
    if (acState.getCurrentState() == &StateID_Carry) {
        calcCarryPos(&calcedPos, this, &carryVec);
        pos = calcedPos;
    }
    return daEnPataKuribo_c::calcModel();
}

void daEnPataKuribon_c::reactFumiProc(dStageActor_c* killingActor) {
    if (hasWings == 0) {
        doStateChange(&StateID_Dizzy);
    } else {
        hasWings = 0;
        setWalkSpeed();
        doStateChange(&StateID_Walk);
    }
    return;
}

void daEnPataKuribon_c::loadModel() {
    resFile.data = getResource("kuribon", "g3d/kuribon.brres");
    nw4r::g3d::ResMdl bmdl = resFile.GetResMdl("kuribon");
	model.setup(bmdl, &allocator, 0x32C, 1, 0);
	SetupTextures_Enemy(&model, 0);

	nw4r::g3d::ResAnmChr resAnmChr = resFile.GetResAnmChr("walk");
	anmChr.setup(bmdl, resAnmChr, &allocator, 0);

    resPat = resFile.GetResAnmTexPat("walk");
	anmTexPat.setup(bmdl, resPat, &allocator, 0, 1);
    return;
}

void daEnPataKuribon_c::loadWingModel() {
    wingRes.data = getResource("kuribon_wing", "g3d/wing.brres");
    nw4r::g3d::ResMdl wmdl = wingRes.GetResMdl("wing");
	wingModel.setup(wmdl, &allocator, 0x32C, 1, 0);
	SetupTextures_Enemy(&wingModel, 0);

	nw4r::g3d::ResAnmChr resAnmChr = wingRes.GetResAnmChr("wing_kuri");
	wingAnim.setup(wmdl, resAnmChr, &allocator, 0);
    return;
}

void daEnPataKuribon_c::updateModel() {
    model._vf1C();
    return;
}

void daEnPataKuribon_c::playWalkAnim() {
    nw4r::g3d::ResAnmChr resAnmChr = resFile.GetResAnmChr("walk");
    anmChr.bind(&model, resAnmChr, 0);
    model.bindAnim((m3d::banm_c *)&anmChr, 2.0);
    anmChr.setUpdateRate(2.0f);
    return;
}

extern "C" bool SetPosForCarry(Vec*,Vec*,float*,u8,u32,dStageActor_c**);

s8 anotherByteArray[2] = {1, 0xFF};
//sets the collider when we carry
void daEnPataKuribon_c::doCarryCollider() {
    float somePosThing[2] = {-8.0, 8.0};
    Vec _24, _30, _3c, _48, _54;
    float someFloat;
    Actor_GetVec3WithValuesAdded(&_3c, this);
    Actor_GetVec3WithValuesAdded(&_48, this);
    Actor_GetVec3WithValuesAdded(&_54, this);
    _24.x = (somePosThing[direction] + _3c.x);
    _24.y = _48.y;
    _24.z = _54.z;
    _30.x = _24.x + anotherByteArray[direction] * 16.0;
    _30.y = _48.y;
    _30.z = _54.z;
    aPhysics.info.xDistToCenter = apRestoreXDist;
    aPhysics.info.yDistToCenter = apRestoreYDist;
    aPhysics.info.xDistToEdge = apRestoreXEdge;
    aPhysics.info.yDistToEdge = apRestoreYEdge;
    someFloat = 0.0;
    if (SetPosForCarry(&_24, &_30, &someFloat, currentLayerID, 1, NULL)) {
        aPhysics.info.yDistToCenter = 8.0;
        aPhysics.info.yDistToEdge = 8.0;
        aPhysics.info.xDistToCenter = (_24.x + someFloat) * 0.5 - pos.x;
        aPhysics.info.xDistToEdge = abs((_24.x - someFloat) * 0.5);
    }
}

//kills other actors when carried? not sure
void daEnPataKuribon_c::killWhenCarried(dAcPy_c *player) {
    u8 *playerNum = player->_vf6C();
    deathInfo.isDead = 1;
    deathInfo.xSpeed = 0.0;
    deathInfo.ySpeed = 3.0;
    deathInfo.maxYSpeed = -4.0;
    deathInfo.yAccel = -0.1875;
    deathInfo.state = &StateID_DieFall;
    deathInfo._14 = 0xFFFFFFFF;
    deathInfo._18 = -1;
    deathInfo._1C = direction;
    deathInfo._1D = *playerNum;
}

void daEnPataKuribon_c::playKickSound(int player_id) {
    Vec actorPos;
    Vec2 stagePos;
    Actor_GetVec3WithValuesAdded(&actorPos, this);
    u32 remId = getRemotePlayer(player_id);
    cvtSndObjctPos(&stagePos, &actorPos);
    startEnemySound(SoundClassRelated, SE_EMY_KAME_KERU, &stagePos, 0);
}

void daEnPataKuribon_c::checkTimer() {
    if (flipTimer >= 600) {
        doStateChange(&StateID_Recover);
    } else if (flipTimer >= 480) {
        anmChr.setUpdateRate(2.0f);
    } else {
        anmChr.setUpdateRate(1.0f);
    }
    flipTimer++;
}

extern "C" u8 dEn_c_EnBgCheck(dEn_c *self); //checks if we're on top of a tile
extern "C" void DoStuffAndMarkDead(dStageActor_c *actor, Vec vector, float unk); //kills if touching lava
extern "C" int SmoothRotation(short* rot, u16 amt, int unk2); //rotates sprite until amt, returns true when finished

s16 dizzyAnglesPata[2] = {0xA000, 0x6000};

void daEnPataKuribon_c::beginState_Dizzy() {
    dStateBase_c *prevState = acState.getPreviousState();
    if (prevState != &StateID_Dizzy && prevState != &StateID_Kick && prevState != &StateID_Throw) {
        nw4r::g3d::ResAnmChr resAnmChr = resFile.GetResAnmChr("ReverseSwoon");
        anmChr.bind(&model, resAnmChr, 0);
        model.bindAnim(&anmChr, 0.0);
        anmChr.setUpdateRate(1.0f);
    }
    flipTimer = 0;
    rot.x = 0x8000;
    rot.y = dizzyAnglesPata[direction];
    speed.x = 0.0;
    y_speed_inc = -0.1875;
    max_speed.x = 0.0;
    max_speed.y = -4.0;
    max_speed.z = 0.0;
}

void daEnPataKuribon_c::executeState_Dizzy() {
    checkTimer();
    updateModel();
    HandleYSpeed();
    doSpriteMovement();

	if (dEn_c_EnBgCheck(this)) {
        if (isOnEnLiftRemoconTrpln()) {
            //bounce galoomba if on bouncy stuff
            speed.y = 5.5;
        } else {
            speed.y = 0.0;
        }
    }

    DoStuffAndMarkDead(this, pos, 1.0);
}

void daEnPataKuribon_c::endState_Dizzy() { }

extern "C" void setRide(void *dRc_c, void *ride);
extern "C" bool updateWhenCarried(dEn_c *self);
extern "C" void cancelCarry(dAcPy_c *self, dStageActor_c *actor);

u8 chainlinkModes[2] = {1, 2};

void daEnPataKuribon_c::beginState_Carry() {
    apRestoreXDist = aPhysics.info.xDistToCenter;
    apRestoreYDist = aPhysics.info.yDistToCenter;
    apRestoreXEdge = aPhysics.info.xDistToEdge;
    apRestoreYEdge = aPhysics.info.yDistToEdge;
    aPhysics.info.bitfield1 = (aPhysics.info.bitfield1 | 0x90);
    aPhysics.info.category2 = 9;
    setRide(&classAt2DC, 0);
    _120 = (_120 & ~0x2);
    speed.y = 0.0;
}

void daEnPataKuribon_c::executeState_Carry() {
    checkTimer();
    dAcPy_c *player = dAcPy_c::findByID(carriedByPlayerID);
    updateModel();
    if ((_12C & 1) == 0) {
        rot.y = (player->rot.y + 0x8000);
        updateWhenCarried(this);
        DoStuffAndMarkDead(this, pos, 1.0);
    } else {
        u8 carryDirection = (directionForCarry >> 24 & 0xFF); //headers are wrong so I have to grab it this way
        direction = carryDirection;
        rot.y = dizzyAnglesPata[carryDirection];
        if (updateWhenCarried(this)) {
            killWhenCarried(player);
            return;
        }
        u32 actorDirection = direction;
        if ((collMgr.outputMaybe & 0x15 << (actorDirection & 0x3f)) == 0) {
            pos.x = pos.x + anotherByteArray[actorDirection] * 6.0;
        }
        if ((_12C & 2) == 0) {
            doStateChange(&StateID_Dizzy);
        } else {
            doStateChange(&StateID_Throw);
        }
    }
    doCarryCollider();
}

void daEnPataKuribon_c::endState_Carry() {
    dAcPy_c *player = dAcPy_c::findByID(carriedByPlayerID);
    cancelCarry(player, this);
    aPhysics.info.bitfield1 = (aPhysics.info.bitfield1 & 0xFFFFFF7F);
    aPhysics.info.category2 = 0;
    aPhysics.info.xDistToCenter = apRestoreXDist;
    aPhysics.info.yDistToCenter = apRestoreYDist;
    aPhysics.info.xDistToEdge = apRestoreXEdge;
    aPhysics.info.yDistToEdge = apRestoreYEdge;
    setRide(&classAt2DC, 0);
    collMgr.outputMaybe = 0;
    _12C = (_12C & 0xFFFFFFFC);
    _120 = (_120 | 0x2);
    rot.y = dizzyAnglesPata[direction];
}

extern "C" u32 EnBgCheckFoot(dEn_c *);
extern "C" u32 EnBgCheckWall(dEn_c *);

void daEnPataKuribon_c::beginState_Throw() {
    float throwSpeeds[2] = {4.0, -4.0};
    s8 playerNum = which_player;
    _134 = 0;
    x_speed_inc = 0.0625;
    speed.x = throwSpeeds[direction];
    max_speed.x = 0.0;
    rot.y = dizzyAnglesPata[direction];
    counter_504[playerNum] = 10;
    aPhysics.info.category2 = 9;
}

void daEnPataKuribon_c::executeState_Throw() {
    checkTimer();
    updateModel();
    HandleXSpeed();
    HandleYSpeed();
    doSpriteMovement();
    u32 sensorFlags = EnBgCheckFoot(this);
    bool isFoot = collMgr.isOnTopOfTile();
    if (isFoot) {
        stuffRelatingToCollisions(0.1875, 0.65, 0.5);
    }
    collMgr.calculateAboveCollision(sensorFlags);
    EnBgCheckWall(this);
    if (collMgr.outputMaybe & 0x15 << (direction & 0x3F)) {
        direction ^= 1;
        rot.y = dizzyAnglesPata[direction];
        speed.x = -speed.x * 0.5;
    }
    DoStuffAndMarkDead(this, pos, 1.0);
    if (speed.x == 0.0) {
        doStateChange(&StateID_Dizzy);
    }
}

void daEnPataKuribon_c::endState_Throw() {
    _134 = 0;
    x_speed_inc = 0.0;
    max_speed.x = 0.0;
    aPhysics.info.category2 = 0;
}

void daEnPataKuribon_c::beginState_Kick() {
    anmChr.setUpdateRate(2.0);
    flipTimer = 0;
    float kickSpeeds[2] = {2.5, -2.5};
    speed.x = kickSpeeds[direction];
    speed.y = 2.0;
    speed.z = 0.0;
    rot.y = dizzyAnglesPata[direction];
    _134 = 0;
    y_speed_inc = -0.1875;
    aPhysics.info.category2 = 9;
}

void daEnPataKuribon_c::executeState_Kick() {
    checkTimer();
    updateModel();
    HandleXSpeed();
    HandleYSpeed();
    doSpriteMovement();
    u32 sensorFlags = EnBgCheckFoot(this);
    bool isFoot = collMgr.isOnTopOfTile();
    if (isFoot) {
        stuffRelatingToCollisions(0.1875, 0.5, 0.5);
    }
    collMgr.calculateAboveCollision(sensorFlags);
    EnBgCheckWall(this);
    if (collMgr.outputMaybe & 0x3c000000) {
        if (speed.y > 0.0) {
            speed.y = speed.y * -0.85;
        }
    }
    if (collMgr.outputMaybe & 0x15 << (direction & 0x3F)) {
        direction ^= 1;
        rot.y = dizzyAnglesPata[direction];
        speed.x = -speed.x;
    }
    DoStuffAndMarkDead(this, pos, 1.0);
    if (speed.x == 0.0) {
        doStateChange(&StateID_Dizzy);
    }
}

void daEnPataKuribon_c::endState_Kick() {
    _134 = 0;
    aPhysics.info.category2 = 0;
}

void daEnPataKuribon_c::beginState_Recover() {
    nw4r::g3d::ResAnmChr resAnmChr = resFile.GetResAnmChr("ReverseRecover");
    anmChr.bind(&model, resAnmChr, 1);
    model.bindAnim(&anmChr, 0.0);
    anmChr.setUpdateRate(2.0f);

    flipTimer = 0;
    speed.x = 0.0;
    y_speed_inc = -0.1875;
    max_speed.x = 0.0;
    max_speed.y = -4.0;
    max_speed.z = 0.0;
}

void daEnPataKuribon_c::executeState_Recover() {
    updateModel();
    HandleYSpeed();
    doSpriteMovement();

    if (dEn_c_EnBgCheck(this)) {
        if (isOnEnLiftRemoconTrpln()) {
            //bounce galoomba if on bouncy stuff
            speed.y = 5.5;
        } else {
            speed.y = 0.0;
        }
    }

    if (anmChr.getCurrentFrame() == 42.0) {
        s16 recoverAngles[2] = {0x2000, 0xE000};
        rot.y = recoverAngles[direction];
        rot.x = 0;
        if (dEn_c_EnBgCheck(this)) {
            speed.y = 2.0;
        }
    }

    if (anmChr.isAnimationDone()) {
        doStateChange(&StateID_Walk);
    }

    DoStuffAndMarkDead(this, pos, 1.0);
}

void daEnPataKuribon_c::endState_Recover() { }