#include <daEnKuribo_c.h>
#include <profile.h>
#include <sfx.h>

class daEnKabochan_c : public daEnKuribo_c {
    public:
        bool isCracked;

        nw4r::g3d::ResAnmClr resClr;
        m3d::anmClr_c anmClr;

        mEf::es2 breakEffect;

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
Profile SplunkinProfile(&daEnKabochan_c::build, SpriteId::EN_KABOCHAN, &SplunkinSpriteData, ProfileId::EN_KURIBO, ProfileId::EN_KABOCHAN, "EN_KABOCHAN", SplunkinArcList, 0x12);

int daEnKabochan_c::onCreate() {
    isCracked = false;

    daEnKuriboBase_c::onCreate();
    
    return true;
}

bool daEnKabochan_c::collisionCat7_GroundPound(ActivePhysics *apThis, ActivePhysics *apOther) {
    if (keepMovingUpwards == false) {
        PlaySound(this, SE_EMY_DOWN);
		SpawnEffect("Wm_mr_softhit", 0, &pos, (S16Vec *)0, (Vec*)0);
		addScoreWhenHit(apOther->owner);


        breakEffect.spawn("Wm_en_splunkinbreak", 0, &pos, (S16Vec *)0, &(Vec){1.5, 1.5, 1.5});
        
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
    if (!isCracked) {
        breakEffect.spawn("Wm_en_splunkinbreak", 0, &(Vec){pos.x, pos.y+8.0, pos.z}, (S16Vec *)0, (Vec*)0);
        isCracked = true;
        anmChr.setUpdateRate(3.0f);
        anmClr.setFrameForEntry(1.0, 0);
        setWalkSpeed();
        _vf224();
    } else {
        breakEffect.spawn("Wm_en_splunkinbreak_b", 0, &(Vec){pos.x, pos.y+8.0, pos.z}, (S16Vec *)0, &(Vec){1.5, 1.5, 1.5});
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