#include <daEnWalkerBase_c.h>

CREATE_STATE(daEnWalkerBase_c, Walk);
CREATE_STATE(daEnWalkerBase_c, Turn);

int daEnWalkerBase_c::onCreate() {
    // load the models
    allocator.link(-1, GameHeaps[0], 0, 0x20);
    loadModel();
    allocator.unlink();

    // set default variable values
    walkAngles[0] = 0;
    walkAngles[1] = 0;
    turnAmt = 0;

    turnsAtLedge = false;
    walkSpeed = 0.0;
    offsetForLedgeCheck = 0.0;


    // NOTE: the "position" field of the ice struct to be the offset of the position when the ice actor is made
    // CreateIceActors adds the values to the actor's current position 
    ice_struct = (DoSomethingCool){0, {0.0, 0.0, 0.0}, {1.0, 1.0, 1.0}, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };

    // call initialize
    initialize();

    return true;
}

int daEnWalkerBase_c::onExecute() {
    // execute state and delete if outside zone
    acState.execute();
    checkZoneBoundaries(0);
    return true;
}

// if you have extra models and need to schedule them for drawing, override this
int daEnWalkerBase_c::onDraw() {
    model.scheduleForDrawing();
    return true;
}

// this is something EVERYONE gets wrong
// newer does a crappy version of this in a "updateModelMatricies" function
// you're supposed to do it like this in finalUpdate/_vf5C
extern "C" void changePosAngle(VEC3 *, S16Vec *, int);
void daEnWalkerBase_c::_vf5C() {
    // calc base model
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

    // calc other models
    onCalcModel();

    return;
}

// turn when colliding with another actor
void daEnWalkerBase_c::spriteCollision(ActivePhysics *apThis, ActivePhysics *apOther) {
    // ensure we have turned fully before turning again
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

extern "C" char usedForDeterminingStatePress_or_playerCollision(dEn_c* t, ActivePhysics *apThis, ActivePhysics *apOther, int unk1);
void daEnWalkerBase_c::playerCollision(ActivePhysics *apThis, ActivePhysics *apOther) {
    dStageActor_c *playerPtr = apOther->owner;
    char hitType = usedForDeterminingStatePress_or_playerCollision(this, apThis, apOther, 0);
    if (hitType == 0) { // player hit
        return dEn_c::playerCollision(apThis, apOther);
    } else if (hitType == 1) { // jumped on
        reactJump(playerPtr);
    } else if (hitType == 3) { // spin jumped on
        reactSpinJump(playerPtr);
    }
    return;
}

void daEnWalkerBase_c::yoshiCollision(ActivePhysics *apThis, ActivePhysics *apOther) {
    dStageActor_c *yoshiPtr = apOther->owner;
    char hitType = usedForDeterminingStatePress_or_playerCollision(this, apThis, apOther, 0);
    if (hitType == 0) { // yoshi hit
        return dEn_c::yoshiCollision(apThis, apOther);
    } else if (hitType == 1) { // jumped on by yoshi
        reactYoshiJump(yoshiPtr);
    }
    return;
}

extern "C" void sub_80024C20(void);
extern "C" void __destroy_arr(void*, void(*)(void), int, int);
bool daEnWalkerBase_c::CreateIceActors() {
    DoSomethingCool real_ice_struct = ice_struct;
    // ice_struct.pos is now used to store the offset values
    real_ice_struct.pos.x += pos.x;
    real_ice_struct.pos.y += pos.y;
    real_ice_struct.pos.z += pos.z;

    frzMgr.Create_ICEACTORs( (void*)&real_ice_struct, 1 );
    __destroy_arr( (void*)&real_ice_struct, sub_80024C20, 0x3C, 1 );
    return true;
}

// the following functions are designed to be overridden

// called in create, use this to set variables
void daEnWalkerBase_c::initialize() {
    return;
}

// called in create, load your models + animations here
void daEnWalkerBase_c::loadModel() {
    return;
}

// called in execute state functions, update your animations here
void daEnWalkerBase_c::updateModel() {
    return;
}

// called in _vf5C, calculate other model matricies here
void daEnWalkerBase_c::onCalcModel() {
    return;
}

// you don't have to override these, but you're encouraged to

// used for behavior when actor is jumped on
void daEnWalkerBase_c::reactJump(dStageActor_c *player) {
    Vec2 killSpeed;
    killSpeed.x = speed.x;
    killSpeed.y = speed.y;
    // kill by jumping with state DieFumi
    killWithSpecifiedState(player, &killSpeed, &StateID_DieFumi);
    return;
}

// used for behavior when actor is spin jumped on
void daEnWalkerBase_c::reactSpinJump(dStageActor_c *player) {
    killByDieFall(player);
    return;
}

// used for behavior when actor is jumped on by yoshi
extern "C" void dEn_c_setDeathInfo_YoshiFumi(dEn_c *, dStageActor_c *);
void daEnWalkerBase_c::reactYoshiJump(dStageActor_c *yoshi) {
    dEn_c_setDeathInfo_YoshiFumi(this, yoshi);
    return;
}

// these are meant to be overridden

// called during walk state
void daEnWalkerBase_c::preWalk() {
    return;
}

void daEnWalkerBase_c::onWalk() {
    return;
}

void daEnWalkerBase_c::postWalk() {
    return;
}

// called during turn
void daEnWalkerBase_c::preTurn() {
    return;
}

void daEnWalkerBase_c::onTurn() {
    return;
}

void daEnWalkerBase_c::postTurn() {
    return;
}

// you only need to override these if needed

void daEnWalkerBase_c::setWalkSpeed() {
    speed.x = (direction == 0) ? walkSpeed : -walkSpeed;
}

bool daEnWalkerBase_c::checkForLedge() {
    float xOffs[] = {offsetForLedgeCheck, -offsetForLedgeCheck};

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

void daEnWalkerBase_c::beginState_Walk() {
    preWalk();
    setWalkSpeed();
    //set Y speed for gravity
    y_speed_inc = -0.1875;
    max_speed.x = 0.0;
    max_speed.y = -4.0;
    max_speed.z = 0.0;
    return;
}

extern "C" u8 dEn_c_EnBgCheck(dEn_c *self);
extern "C" void DoStuffAndMarkDead(dStageActor_c *actor, Vec vector, float unk);
extern "C" int SmoothRotation(short* rot, u16 amt, int unk2);

void daEnWalkerBase_c::executeState_Walk() {
    // update model and handle speed
    updateModel();
    HandleYSpeed();
    doSpriteMovement();

    //ensure we're facing the right angle
    SmoothRotation(&rot.y, walkAngles[direction], turnAmt);

    if (!(dEn_c_EnBgCheck(this))) { // if NOT standing on a tile
        if (collMgr.isOnTopOfTile() && (_45D == false) && (speed.y <= 0.0f)) {
            velocity2.x = velocity2.x + _310;
        }
    } else { //standing on a tile
        velocity2.x = 0.0;
        speed.y = 0.0;
        if (turnsAtLedge && !(checkForLedge())) {
            doStateChange(&StateID_Turn);
            return;
        }
    }

    onWalk();

    if (collMgr.outputMaybe & 0x15 << direction & 0x3f) { // touching a tile from the side
        doStateChange(&StateID_Turn);
    }
    DoStuffAndMarkDead(this, pos, 1.0); // die if touching lava
    return;
}

void daEnWalkerBase_c::endState_Walk() {
    postWalk();
    return;
}

void daEnWalkerBase_c::beginState_Turn() {
    preTurn();
    direction ^= 1;
    speed.x = 0.0;
    return;
}

void daEnWalkerBase_c::executeState_Turn() {
    // update model and handle speed
    updateModel();
    HandleYSpeed();
    doSpriteMovement();

    if (!(dEn_c_EnBgCheck(this))) { // not on tile
        if (collMgr.isOnTopOfTile() && _45D == false && speed.y <= 0.0f) {
            velocity2.x = velocity2.x + _310;
        }
    } else { //standing on something
        speed.y = 0.0;
    }

    onTurn();

    DoStuffAndMarkDead(this, pos, 1.0); // die if touching lava

    // go back to walk once done turning
    bool doneTurning = SmoothRotation(&rot.y, walkAngles[direction], turnAmt);
    if (doneTurning) {
        doStateChange(&StateID_Walk);
    }
}

void daEnWalkerBase_c::endState_Turn() {
    postTurn();
    return;
}