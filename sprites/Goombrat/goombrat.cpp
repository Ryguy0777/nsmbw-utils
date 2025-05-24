//TODO: bah animation and making the little shit look down when near a ledge
#include <daEnKuribo_c.h>
#include <profile.h>

class daEnKakibo_c : public daEnKuribo_c {
    public:
        void executeState_Walk();
        void executeState_Turn();

        void loadModel();

        void playWalkAnim();

        bool checkForLedge(float xOffset);

        static dActor_c *build();
};

dActor_c *daEnKakibo_c::build() {
	void *buffer = AllocFromGameHeap1(sizeof(daEnKakibo_c));
	return new(buffer) daEnKakibo_c;
}

const char* GoombratArcList [] = {
	"kakibo",
	NULL
};

const SpriteData GoombratSpriteData = {ProfileId::EN_KAKIBO, 8, 0xFFFFFFF0, 0, 8, 8, 8, 0, 0, 0, 0, 0};
Profile GoombratProfile(&daEnKakibo_c::build, SpriteId::EN_KAKIBO, &GoombratSpriteData, ProfileId::EN_KAKIBO, ProfileId::EN_KAKIBO, "EN_KAKIBO", GoombratArcList, 0x12);

u16 kakiboAngles[2] = {0x2000, 0xE000};

extern "C" u8 dEn_c_EnBgCheck(dEn_c *self);
extern "C" bool CheckDanceValues_Bahps(u32 param);
extern "C" void DoStuffAndMarkDead(dStageActor_c *actor, Vec vector, float unk);
extern "C" int SmoothRotation(short* rot, u16 amt, int unk2);

void daEnKakibo_c::executeState_Walk() {
    updateModel();
    HandleYSpeed();
    doSpriteMovement();
    SmoothRotation(&rot.y, kakiboAngles[direction], 0x200);
    walkEffect();

    if (!(dEn_c_EnBgCheck(this))) {
        if (collMgr.isOnTopOfTile() && (_45D == false) && (speed.y <= 0.0f)) {
            velocity2.x = velocity2.x + _310;
        }
    } else {
        velocity2.x = 0.0;
        speed.y = 0.0;
        if (isOnEnLiftRemoconTrpln()) {
            doStateChange(&StateID_TrplnJump);
            return;
        }
        if (!(checkForLedge(2.5))) {
            doStateChange(&StateID_Turn);
            return;
        }
    }
    if (collMgr.outputMaybe & 0x15 << direction & 0x3f) {
        doStateChange(&StateID_Turn);
    }
    DoStuffAndMarkDead(this, pos, 1.0);
    return;
}

void daEnKakibo_c::executeState_Turn() {
    updateModel();
    HandleYSpeed();
    doSpriteMovement();

    if (!(dEn_c_EnBgCheck(this))) {
        if (collMgr.isOnTopOfTile() && (_45D == false) && (speed.y <= 0.0f)) {
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

    bool doneTurning = SmoothRotation(&rot.y, kakiboAngles[direction], 0x200);

    if (doneTurning) {
        doStateChange(&StateID_Walk);
    }
    return;
}

void daEnKakibo_c::loadModel() {
    resFile.data = getResource("kakibo", "g3d/kakibo.brres");
    nw4r::g3d::ResMdl bmdl = resFile.GetResMdl("kakibo");
	model.setup(bmdl, &allocator, 0x227, 1, 0);
	SetupTextures_Enemy(&model, 0);

	nw4r::g3d::ResAnmChr resAnmChr = resFile.GetResAnmChr("walk");
	anmChr.setup(bmdl, resAnmChr, &allocator, 0);

    resPat = resFile.GetResAnmTexPat("walk");
	anmTexPat.setup(bmdl, resPat, &allocator, 0, 1);
    return;
}

void daEnKakibo_c::playWalkAnim() {
    nw4r::g3d::ResAnmChr resAnmChr = resFile.GetResAnmChr("walk");
    anmChr.bind(&model, resAnmChr, 0);
    model.bindAnim((m3d::banm_c *)&anmChr, 2.0);
    anmChr.setUpdateRate(2.0f);

    model.bindAnim(&anmTexPat, 0.0);
    anmTexPat.setUpdateRateForEntry(1.0f, 0);
    anmTexPat.setEntryByte34(0, 0);
    u32 rand = MakeRandomNumber(0xb4);
    anmTexPat.setFrameForEntry((float)rand, 0);
    return;
}

bool daEnKakibo_c::checkForLedge(float xOffset) {
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