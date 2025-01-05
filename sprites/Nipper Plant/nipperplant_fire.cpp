#include <daFireBall_Base_c.h>
#include <profile.h>

class daFireBall_Nipper_c : public daFireBall_Base_c {
    public:
        void executeState_Move();

        bool initialize();
        
        void setCc();

        static dActor_c *build();
};

dActor_c *daFireBall_Nipper_c::build() {
	void *buffer = AllocFromGameHeap1(sizeof(daFireBall_Nipper_c));
	return new(buffer) daFireBall_Nipper_c;
}

Profile NipperFireProfile(&daFireBall_Nipper_c::build, ProfileId::AC_PAKKUN_PUCHI_FIRE, NULL, ProfileId::PAKKUN_FIREBALL, ProfileId::AC_PAKKUN_PUCHI_FIRE, "AC_PAKKUN_PUCHI_FIRE", NULL, 0x22);

void daFireBall_Nipper_c::executeState_Move() {
    HandleYSpeed();
    UpdateObjectPosBasedOnSpeedValuesReal();

    if (collMgr.calculateBelowCollision() || collMgr.calculateAboveCollision(0) || collMgr.calculateAdjacentCollision()) {
        killFireBall();
        nw4r::snd::SoundHandle handle;
        PlaySoundWithFunctionB4(SoundRelatedClass, &handle, SE_OBJ_EMY_FIRE_DISAPP, 0);
    }
}

float nipperfirespeeds[4] = {0.453125, 0.90625, 1.359375, 1.8125};

bool daFireBall_Nipper_c::initialize() {
    direction = settings & 1;
    int distance = settings >> 4 & 0xF;
    float baseXSpeed = nipperfirespeeds[distance];
    speed.x = (direction) ? -baseXSpeed : baseXSpeed;
    speed.y = 3.0;
    y_speed_inc = -0.1875;
    max_speed.y = -4.0;
    return true;
}

extern "C" void BrosFireCallBack(ActivePhysics *, ActivePhysics *);

void daFireBall_Nipper_c::setCc() {
    ActivePhysics::Info nipper_fire_cc;
    nipper_fire_cc.xDistToCenter = 0.0;
    nipper_fire_cc.yDistToCenter = 0.0;
    nipper_fire_cc.xDistToEdge = 3.0;
    nipper_fire_cc.yDistToEdge = 3.0;
    nipper_fire_cc.category1 = 3;
    nipper_fire_cc.category2 = 0;
    nipper_fire_cc.bitfield1 = 0xF;
    nipper_fire_cc.bitfield2 = 0x8800;
    nipper_fire_cc.callback = &BrosFireCallBack;

    aPhysics.initWithStruct(this, &nipper_fire_cc);
    aPhysics.addToList();
}