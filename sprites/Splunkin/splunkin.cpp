#include <daEnKuribo_c.h>
#include <profile.h>
#include <sfx.h>

class daEnKabochan_c : public daEnKuribo_c {
    public:
        bool isCracked;

        nw4r::g3d::ResAnmClr resClr;
        m3d::anmClr_c anmClr;

        int onCreate();

        bool collisionCat7_GroundPound(ActivePhysics *apThis, ActivePhysics *apOther);

        void dieOther_Begin();

        bool CreateIceActors();

        void _vf260(void *other);

        void reactFumiProc(dStageActor_c* killingActor);
        void loadModel();

        void setWalkSpeed();

        void updateModel();
        void playWalkAnim();

        static dActor_c *build();
};

dActor_c *daEnKabochan_c::build() {
	void *buffer = AllocFromGameHeap1(sizeof(daEnKabochan_c));
	return new(buffer) daEnKabochan_c;
}

const char* SplunkinArcList [] = {
	"kabochan",
	NULL
};

const SpriteData SplunkinSpriteData = {ProfileId::EN_KABOCHAN, 8, 0xFFFFFFF0, 0, 8, 8, 8, 0, 0, 0, 0, 0};
Profile SplunkinProfile(&daEnKabochan_c::build, SpriteId::EN_KABOCHAN, &SplunkinSpriteData, ProfileId::EN_KABOCHAN, ProfileId::EN_KABOCHAN, "EN_KABOCHAN", SplunkinArcList, 0x12);

int daEnKabochan_c::onCreate() {
    isCracked = false;

    daEnKuriboBase_c::onCreate();
    
    return true;
}

bool daEnKabochan_c::collisionCat7_GroundPound(ActivePhysics *apThis, ActivePhysics *apOther) {
    if (keepMovingUpwards == false) {
        PlaySound(this, SE_EMY_DOWN);
		SpawnEffect("Wm_mr_softhit", 0, &pos, &(S16Vec){0,0,0}, &(Vec){1.0, 1.0, 1.0});
		addScoreWhenHit(apOther->owner);

        u32 idForGreen = MakeRandomNumber(5);
        for (int i = 0; i < 5; i++) {
            bool isGreen = false;
            if (i == idForGreen) {isGreen = true;}
            dStageActor_c::create(ProfileId::AC_KABOEFF, (isGreen << 5) | (1 << 4) | i, &(Vec){pos.x, pos.y+16.0, pos.z}, &rot, 0);
        }
        
        Vec2 killSpeed;
        killSpeed.y = speed.y;
        killSpeed.x = speed.x;
        killWithSpecifiedState(apOther->owner, &killSpeed, &dEn_c::StateID_DieOther);
    }
    return true;
}

void daEnKabochan_c::dieOther_Begin() {
    removeMyActivePhysics();
    Delete(deleteForever);
}

extern "C" void sub_80024C20(void);
extern "C" void __destroy_arr(void*, void(*)(void), int, int);

bool daEnKabochan_c::CreateIceActors() {
	struct DoSomethingCool my_struct = { 0, (Vec3){pos.x, pos.y-2.0, pos.z}, {1.2, 1.2, 1.4}, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
    frzMgr.Create_ICEACTORs( (void*)&my_struct, 1 );
    __destroy_arr( (void*)&my_struct, sub_80024C20, 0x3C, 1 );
    return true;
}


void daEnKabochan_c::_vf260(void *other) {
    int sfxID = (isCracked) ? SFX_SE_EMY_KABOCHAN_KILL : SFX_SE_EMY_KABOCHAN_BREAK;
    float sfxVol = (isCracked) ? 3.0 : 2.0;
    nw4r::snd::SoundHandle handle;
    PlaySoundWithFunctionB4(SoundRelatedClass, &handle, sfxID, 1);
    handle.SetVolume(sfxVol, 1);
    return;
}

void daEnKabochan_c::reactFumiProc(dStageActor_c* killingActor) {
    u32 idForGreen = MakeRandomNumber(5);
    for (int i = 0; i < 5; i++) {
        bool isGreen = false;
        if (i == idForGreen) {isGreen = true;}
        dStageActor_c::create(ProfileId::AC_KABOEFF, (isGreen << 5) | (isCracked << 4) | i, &(Vec){pos.x, pos.y+16.0, pos.z}, &rot, 0);
    }
    if (!isCracked) {
        isCracked = true;
        anmChr.setUpdateRate(3.0f);
        anmClr.setFrameForEntry(1.0, 0);
        setWalkSpeed();
        _vf224();
    } else {
        Vec2 killSpeed;
        killSpeed.y = speed.y;
        killSpeed.x = speed.x;
        killWithSpecifiedState(killingActor, &killSpeed, &dEn_c::StateID_DieOther);
    }
    
    return;
}

void daEnKabochan_c::loadModel() {
    resFile.data = getResource("kabochan", "g3d/kabochan.brres");
    nw4r::g3d::ResMdl bmdl = resFile.GetResMdl("kabochan");
	model.setup(bmdl, &allocator, 0x32C, 1, 0);
	SetupTextures_Enemy(&model, 0);

	nw4r::g3d::ResAnmChr resAnmChr = resFile.GetResAnmChr("walk");
	anmChr.setup(bmdl, resAnmChr, &allocator, 0);

    resClr = resFile.GetResAnmClr("break");
	anmClr.setup(bmdl, resClr, &allocator, 0, 1);

    return;
}

float SplunkinSpeeds[2][2] = {{0.5, -0.5},{0.75, -0.75}};

void daEnKabochan_c::setWalkSpeed() {
    speed.x = SplunkinSpeeds[isCracked][direction];
    return;
}

void daEnKabochan_c::updateModel() {
    model._vf1C();
    anmClr.process();
    return;
}

void daEnKabochan_c::playWalkAnim() {
    nw4r::g3d::ResAnmChr resAnmChr = resFile.GetResAnmChr("walk");
    anmChr.bind(&model, resAnmChr, 0);
    model.bindAnim((m3d::banm_c *)&anmChr, 2.0);
    anmChr.setUpdateRate(2.0f);
    anmClr.bind(&model, resClr, 0, 0);
    anmClr.setUpdateRateForEntry(0.0f, 0);
    anmClr.setFrameForEntry(0, 0);
    model.bindAnim(&anmClr, 1.0);
    return;
}

class dPumpkinEffects_c : public dStageActor_c {
    public:
        mHeapAllocator_c allocator;

		nw4r::g3d::ResFile resFile;
		m3d::mdl_c model;
        m3d::anmChr_c anmChr;
        nw4r::g3d::ResAnmTexPat resPat;
        m3d::anmTexPat_c anmTexPat;

        int fragID;
        bool isBig;
        bool isGreen;

        int onCreate();
        int onDraw();
		int onExecute();

        void updateModelMatrices();

        static dActor_c *build();
};

dActor_c *dPumpkinEffects_c::build() {
	void *buffer = AllocFromGameHeap1(sizeof(dPumpkinEffects_c));
	return new(buffer) dPumpkinEffects_c;
}

const SpriteData PumpEffSpriteData = {ProfileId::AC_KABOEFF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
Profile PumpEffProfile(&dPumpkinEffects_c::build, NULL, &PumpEffSpriteData, ProfileId::AC_KABOEFF, ProfileId::AC_KABOEFF, "AC_KABOEFF");

Vec fragSpeeds[5] = {
    {0.7, 1.6, 0.0},
    {0.1, 1.7, 0.0},
    {-0.06, 2.0, 0.0},
    {-0.4, 1.9, 0.0},
    {-0.9, 1.37, 0.0}
};

int dPumpkinEffects_c::onCreate() {
    fragID = settings & 0xF;
    isBig = settings >> 4 & 1;
    isGreen = settings >> 5 & 1;

    allocator.link(-1, GameHeaps[0], 0, 0x20);

    resFile.data = getResource("kabochan", "g3d/kabochan.brres");
    nw4r::g3d::ResMdl bmdl;
    u32 rand = MakeRandomNumber(2);
    if (rand == 0) {
        bmdl = resFile.GetResMdl("FX_Pumpkin");
    } else {
        bmdl = resFile.GetResMdl("FX_Pumpkin2");
    }
	model.setup(bmdl, &allocator, 0x227, 1, 0);
	SetupTextures_Enemy(&model, 0);

    resPat = resFile.GetResAnmTexPat("FX_Pumpkin");
	anmTexPat.setup(bmdl, resPat, &allocator, 0, 1);
    model.bindAnim(&anmTexPat, 0.0);
    anmTexPat.setUpdateRateForEntry(0.0f, 0);
    anmTexPat.setFrameForEntry(isGreen, 0);

    nw4r::g3d::ResAnmChr resAnmChr = resFile.GetResAnmChr("shrink");
    anmChr.setup(bmdl, resAnmChr, &allocator, 0);
	anmChr.bind(&model, resAnmChr, 1);
	model.bindAnim(&anmChr, 0);
	anmChr.setUpdateRate(0.5f);

    allocator.unlink();

    if (isBig) {
        scale = (Vec){0.55, 0.55, 0.55};
    } else {
        scale = (Vec){0.3, 0.3, 0.3};
    }

    float scaleMod = ((MakeRandomNumber(10) / 10) * scale.x);
    scale = (Vec){scale.x - scaleMod, scale.y - scaleMod, scale.z - scaleMod}; 

    pos.z += 100.0;

    y_speed_inc = -0.05;
    max_speed.x = speed.x;
    max_speed.y = -2.0;
    max_speed.z = 0.0;

    speed = fragSpeeds[fragID];

    rot.x = MakeRandomNumber(0xFFFF);
    rot.y = MakeRandomNumber(0xFFFF);
    rot.z = MakeRandomNumber(0xFFFF);

    onExecute();

    return true;
}

int dPumpkinEffects_c::onDraw() {
    model.scheduleForDrawing();
    return true;
}

int dPumpkinEffects_c::onExecute() {
    model._vf1C();
    rot.x += 0x400;
    rot.y += 0x400;
    rot.z += 0x400;
    updateModelMatrices();
    if (anmChr.isAnimationDone()) {
        Delete(1);
    }
    HandleYSpeed();
    UpdateObjectPosBasedOnSpeedValuesReal();
    return true;
}

void dPumpkinEffects_c::updateModelMatrices() {
	// This won't work with wrap because I'm lazy.
	matrix.translation(pos.x, pos.y, pos.z);
	matrix.applyRotationYXZ(&rot.x, &rot.y, &rot.z);

	model.setDrawMatrix(matrix);
	model.setScale(&scale);
	model.calcWorld(false);
}