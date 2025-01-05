#include <daEnBrosBase.h>
#include <bomb_projectile.h>

class daEnBombBros_c : public daEnBrosBase_c {
    public:
        daBombProjectile_c *proj;

        bool collisionCat9_RollingObject(ActivePhysics *apThis, ActivePhysics *apOther);

        double getProjCreateFrameNum();
        double getProjReleaseFrameNum();
        void setSpeed();
        int isOnScreen_maybe();
        int getColor();
        void calledInMoveBegin();

        void weaponCreate();
        void weaponAttack();
        void setJumpCnt();
        void setJump();
        void beginJump();

        static dActor_c *build();
};

dActor_c *daEnBombBros_c::build() {
	void *buffer = AllocFromGameHeap1(sizeof(daEnBombBros_c));
	daEnBombBros_c *c = new(buffer) daEnBombBros_c;
    c->callback.owner = c;
    return c;
}

const char* BombBroArcList [] = {
	"bros_bombhei",
    "bros",
	NULL
};

const SpriteData BombBroSpriteData = {ProfileId::EN_BOMBBROS, 8, 0xFFFFFFF0, 0, 0x10, 8, 0x10, 0, 0, 0, 0, 0};
Profile BombBroProfile(&daEnBombBros_c::build, SpriteId::EN_BOMBBROS, &BombBroSpriteData, ProfileId::EN_HAMMERBROS, ProfileId::EN_BOMBBROS, "EN_BOMBBROS", BombBroArcList, 0x12);

bool daEnBombBros_c::collisionCat9_RollingObject(ActivePhysics *apThis, ActivePhysics *apOther) {
    if (apOther->owner->profileId == ProfileId::BROS_BOMB) {
        daBombProjectile_c *bomb = (daBombProjectile_c *)apOther->owner;
        if (!bomb->playerBomb) {
            return true;
        }
    }
    return dEn_c::collisionCat9_RollingObject(apThis, apOther);
}

double daEnBombBros_c::getProjCreateFrameNum() {
    return 1.875;
}

double daEnBombBros_c::getProjReleaseFrameNum() {
    return 30;
}

extern "C" int BasicBrosIsOnScreen(daEnBrosBase_c *);
int daEnBombBros_c::isOnScreen_maybe() {
    return BasicBrosIsOnScreen(this);
}

void daEnBombBros_c::setSpeed() {
    speed.x = (facingDirection_maybe) ? -0.6 : 0.6;
    speed.y = 0.0;
}

int daEnBombBros_c::getColor() {
    return 4;
}

void daEnBombBros_c::calledInMoveBegin() {
    countdownUntilAttacking = MakeRandomNumber(0x32) + 10;
}

extern "C" void MtxToVec(Mtx *, Vec3 *);
void daEnBombBros_c::weaponCreate() {
    Vec3 bombVec;
    MtxToVec(mtx1, &bombVec);
    proj = (daBombProjectile_c *)create(ProfileId::BROS_BOMB, id, &bombVec, (S16Vec *)0, 0);
    heldProjectileID_maybe = proj->id;
}

void daEnBombBros_c::weaponAttack() {
    if (proj) {
        proj->readyToThrow = true;
        heldProjectileID_maybe = 0;
    }
}

int bombBroCountdown[8] = {10, 20, 30, 50, 40, 30, 50, 60};

void daEnBombBros_c::setJumpCnt() {
    countdownAfterTouchingGround = (bombBroCountdown[MakeRandomNumber(8)] + 0x3c);
}

void daEnBombBros_c::setJump() {
    if (MakeRandomNumber(7) == 0) {
        speed.y = 3.5;
    } else {
        speed.y = 2.5;
    }
}

void daEnBombBros_c::beginJump() {
    setJump();
    doStateChange(&StateID_AirAttack);
}