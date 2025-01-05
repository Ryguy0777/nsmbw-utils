#include <bomb_projectile.h>

CREATE_STATE(daBombProjectile_c, ThrowWait);
CREATE_STATE(daBombProjectile_c, EatWait);
CREATE_STATE(daBombProjectile_c, Throw);
CREATE_STATE(daBombProjectile_c, Explode);

daBombProjectile_c::daBombProjectile_c() : acState(this) { }

dActor_c *daBombProjectile_c::build() {
	void *buffer = AllocFromGameHeap1(sizeof(daBombProjectile_c));
	return new(buffer) daBombProjectile_c;
}

Profile BombProjProfile(&daBombProjectile_c::build, ProfileId::BROS_BOMB, NULL, ProfileId::EN_BOMHEI, ProfileId::BROS_BOMB, "BROS_BOMB", NULL, 0x22);

void BombProjectileCallback(ActivePhysics *apThis, ActivePhysics *apOther) {
    daBombProjectile_c *bomb = (daBombProjectile_c *)apThis->owner;
    dStageActor_c *other = apOther->owner;
    u32 otherKind = other->stageActorType;
    if (bomb->playerBomb) { // if we've been spat by a yoshi
        if (otherKind == 3) { //colliding with an entity
            bomb->acState.setState(&daBombProjectile_c::StateID_Explode);
        }
    } else { // thrown by a bro
        if (otherKind == 1) { //touching player
            bomb->acState.setState(&daBombProjectile_c::StateID_Explode);
        } else if (otherKind == 2) { //touching yoshi
            if (apOther->info.category2 == 0xF) { // if we're colliding with yoshi's tounge
                bomb->acState.setState(&daBombProjectile_c::StateID_EatWait);
            } else {
                u8 *playerNum = other->_vf6C(); //yoshi has a player on him
                if (*playerNum < 4) {
                    bomb->acState.setState(&daBombProjectile_c::StateID_Explode);
                }
            }
        }
    }
};

extern "C" void dAcPy_vf3F4(void* mario, void* other, int t);
extern "C" void DamageYoshi(void* yoshi, void* other, int type);
void BombExplosionCallback(ActivePhysics *apThis, ActivePhysics *apOther) {
    daBombProjectile_c *bomb = (daBombProjectile_c *)apThis->owner;
    dEn_c *other = (dEn_c *)apOther->owner;
    if (!bomb->playerBomb) { // only collide with players if we've been thrown by a bro
        u32 otherKind = other->stageActorType;
        if (otherKind == 1) {
            dAcPy_vf3F4(other, bomb, 0);
        } else if (otherKind == 2) {
            u8 *playerNum = other->_vf6C();
            if (*playerNum < 4) {
                DamageYoshi(other, bomb, 0);
            }
        }
    }
}

extern "C" s8 getNearestPlayerXDirection(dStageActor_c *, Vec *);

int daBombProjectile_c::onCreate() {
    if (settings) {
        owner = (daEnBrosBase_c *)searchById(settings);
    }
    // setup model
    allocator.link(-1, GameHeaps[0], 0, 0x20);

	resFile.data = getResource("bros_bombhei", "g3d/bros_bombhei.brres");
    bmdl = resFile.GetResMdl("bombhei");
	model.setup(bmdl, &allocator, 0x108, 1, 0);
	SetupTextures_Enemy(&model, 0);

    resClr = resFile.GetResAnmClr("bombhei");
	anmClr.setup(bmdl, resClr, &allocator, 0, 1);
    anmClr.bind(&model, resClr, 0, 1);
    model.bindAnim(&anmClr, 0.0f);

	allocator.unlink();

    // collider
    ActivePhysics::Info bombproj_cc;
    bombproj_cc.xDistToCenter = 0.0;
    bombproj_cc.yDistToCenter = 8.0;
    bombproj_cc.xDistToEdge = 8.0;
    bombproj_cc.yDistToEdge = 8.0;
    bombproj_cc.category1 = 3;
    bombproj_cc.category2 = 0;
    bombproj_cc.bitfield1 = 0x4F;
    bombproj_cc.bitfield2 = 0x8000;
    bombproj_cc.callback = &BombProjectileCallback;
    aPhysics.initWithStruct(this, &bombproj_cc);

    // tile collider
    static const lineSensor_s below(-4<<12, 4<<12, 0<<12);
	static const pointSensor_s above(0<<12, 16<<12);
    static const lineSensor_s adjacent(6<<12, 8<<12, 7<<12);

	collMgr.init(this, &below, &above, &adjacent);

    if (owner) {
        direction = owner->direction;
    } else {
        direction = getNearestPlayerXDirection(this, &pos);
    }
    rot.y = (direction) ? 0xE000 : 0x2000;

    // these are used to set the "center" of the sprite
    // used by yoshi tongue and dieFall
    pos_delta2.x = 0.0;
    pos_delta2.y = 8.0;
    pos_delta2.z = 0.0;

    spriteSomeRectX = 16.0;
	spriteSomeRectY = 16.0;
	_320 = 0.0;
	_324 = 8.0;


    // yoshi can eat bomb but not swallow
    _36D = 1;

    // wait for the bro to release us
    if (owner) {
        acState.setState(&StateID_ThrowWait);
    } else {
        acState.setState(&StateID_Throw);
    }

    return true;
}

int daBombProjectile_c::onExecute() {
    acState.execute();
    checkZoneBoundaries(0);
    return true;
}

int daBombProjectile_c::onDraw() {
    model.scheduleForDrawing();
    return true;
}

extern "C" void changePosAngle(VEC3 *, S16Vec *, int);
extern "C" void MtxToVec(Mtx *, Vec3 *);
void daBombProjectile_c::_vf5C() {
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
    model.setScale(scale.x, scale.y, scale.z);
    model.calcWorld(false);

    nw4r::g3d::ResNode root = bmdl.GetResNode("skl_root");
	int rootNodeID = root.GetID();
    Mtx rootMtx;
	model.getMatrixForNode(rootNodeID, &rootMtx);
    MtxToVec(&rootMtx, &rootPos);

    return;
}

bool daBombProjectile_c::_vf8C(void *other) {
    // coming out of yoshi's mouth
	dAcPy_c *player = (dAcPy_c *)other;
	direction = player->direction;
    pos.y += 3.0;
    pos.z = 5750.0;
    aPhysics.info.bitfield2 = 0;
    // we own the bomb now
    playerBomb = true;
    player_owner = player;
	acState.setState(&daBombProjectile_c::StateID_Throw);
	return true;
}

float tileXOffsets[9] = {
    -16.0, 0.0, 16.0,
    -16.0, 0.0, 16.0,
    -16.0, 0.0, 16.0
};

float tileYOffsets[9] = {
    16.0, 16.0, 16.0,
    0.0, 0.0, 0.0,
    -16.0, -16.0, -16.0
};

//idk what this does
void daBombProjectile_c::explodeTiles() {
    Vec2 tileToCheck;
    if (explosionTimer < 9) {
        tileToCheck.x = (pos.x * 0.0625) * 16.0 + tileXOffsets[explosionTimer];
        tileToCheck.y = (pos.y * 0.0625) * 16.0 + tileYOffsets[explosionTimer];
    } else {
        tileToCheck.x = 0.0;
        tileToCheck.y = 0.0;
    }
    for (int i = 0; i < 2; i++) {
        UnitType type = (UnitType)collMgr.getTileBehaviour1At(tileToCheck.x, tileToCheck.y, currentLayerID);
        if (type & (QUESTION|EXPLODABLE|BRICK)) {
            dBgGm_c::instance->placeTile((int)tileToCheck.x & 0xffff, (int)-tileToCheck.y & 0xffff, 0, 0);
        }
    }
    
}

void daBombProjectile_c::beginState_ThrowWait() {
    scale = (Vec){0.5, 0.5, 0.5};
}

void daBombProjectile_c::executeState_ThrowWait() {
    Vec3 handPos;
    MtxToVec(owner->mtx1, &handPos);
    pos = handPos;
    rot.y = owner->rot.y;
    if (scale.x < 1.0) {
        scale.x += 0.02;
        scale.y += 0.02;
        scale.z += 0.02;
    }
    if (readyToThrow) {
        acState.setState(&StateID_Throw);
    }
}

void daBombProjectile_c::endState_ThrowWait() {
    scale = (Vec){1.0, 1.0, 1.0};
}

void daBombProjectile_c::beginState_EatWait() {
    anmClr.setFrameForEntry(0.0, 0);
    anmClr.setUpdateRate(0.0);
    aPhysics.removeFromList();
}

void daBombProjectile_c::executeState_EatWait() { }

void daBombProjectile_c::endState_EatWait() {
}

void daBombProjectile_c::beginState_Throw() {
    rot.y = (direction) ? 0xE000 : 0x2000;
    // now we're adding our physics
    aPhysics.addToList();

    anmClr.setUpdateRate(1.0);

    if (!playerBomb) {
        speed.x = (direction) ? -2.1 : 2.1;
        speed.y = 3.5;
        max_speed.y = -6.0;
        y_speed_inc = -0.2;
        if (owner) {
            speed.x = speed.x + owner->pos_delta.x * 1.1;
        }
    } else {
        speed.x = player_owner->speed.x * 0.5 + ((direction) ? -3.85 : 3.85);
        speed.y = 2.8;
        y_speed_inc = -0.225;
        max_speed.x = (direction) ? -2.9: 2.9;
        _118 = -4.0;
        max_speed.y = -4.0;
        x_speed_inc = 0.04;
    }
}

extern "C" void cvtSndObjctPos(Vec2 *out, Vec *stage_pos);
extern void *SoundClassRelated;
extern "C" void holdEmySound(void *, SFX, int, Vec2 *, u64);

void daBombProjectile_c::executeState_Throw() {
    anmClr.process();
    HandleYSpeed();
    HandleXSpeed();
    UpdateObjectPosBasedOnSpeedValuesReal();

    // effect and sound
    igniteEffect.spawn("Wm_en_bombignition", 0, &rootPos, (S16Vec *)0, (Vec *)0);
    Vec2 soundPos;
    cvtSndObjctPos(&soundPos, &pos);
    holdEmySound(SoundClassRelated, SE_EMY_BH_HIBANA, id, &soundPos, 0);

    // explode after a while
    if (anmClr.isEntryAnimationDone(0)) {
        shouldExplode = true;
    }

    if (collMgr.calculateBelowCollision() || collMgr.calculateAboveCollision(0) || collMgr.calculateAdjacentCollision()) {
        shouldExplode = true;
    }

    if (shouldExplode) {
        acState.setState(&StateID_Explode);
    }
}

void daBombProjectile_c::endState_Throw() { }

extern "C" void startEnemySound(void*,SFX,Vec2*,int);

void daBombProjectile_c::beginState_Explode() {
    aPhysics.removeFromList();
    SpawnEffect("Wm_en_explosion", 0, &rootPos, (S16Vec *)0, (Vec *)0);
    SpawnEffect("Wm_en_bombheibreak", 0, &rootPos, (S16Vec *)0, (Vec *)0);

    Vec2 soundPos;
    cvtSndObjctPos(&soundPos, &pos);
    startEnemySound(SoundClassRelated, SE_EMY_BH_BOMB, &soundPos, 0);
    
    scale = (Vec){0.0, 0.0, 0.0};

    ActivePhysics::Info explode_cc;
    explode_cc.xDistToCenter = 0.0;
    explode_cc.yDistToCenter = 8.0;
    explode_cc.xDistToEdge = 18.0;
    explode_cc.yDistToEdge = 18.0;
    explode_cc.category1 = 3;
    explode_cc.category2 = 9;
    explode_cc.bitfield1 = 0xEF; // 0x6F | 0xEF
    explode_cc.bitfield2 = 0;
    explode_cc.unkShort1C = 1; // 0 | 1
    explode_cc.callback = &BombExplosionCallback;

    aPhysics.initWithStruct(this, &explode_cc);
    aPhysics.addToList();

    explosionTimer = 0;
}

void daBombProjectile_c::executeState_Explode() {
    explosionTimer++;
    if (explosionTimer < 9) {
        explodeTiles();
        return;
    }
    if (explosionTimer != 0x10) {
        return;
    }
    Delete(1);
    return;
}

void daBombProjectile_c::endState_Explode() { }