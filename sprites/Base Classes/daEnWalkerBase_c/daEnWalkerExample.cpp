#include <daEnWalkerBase_c.h>
#include <profile.h>

class daEnWalkerExample_c : public daEnWalkerBase_c {
    public:
        void initialize();

        void loadModel();
        void updateModel();

        static dActor_c *build();
};

dActor_c *daEnWalkerExample_c::build() {
	void *buffer = AllocFromGameHeap1(sizeof(daEnWalkerExample_c));
	return new(buffer) daEnWalkerExample_c;
}

const char* WalkerExArcList [] = {
	"walker",
	NULL
};

const SpriteData WalkerExSpriteData = {ProfileId::EN_WALKER_EXAMPLE, 8, 0xFFFFFFF0, 0, 8, 8, 8, 0, 0, 0, 0, 0};
Profile WalkerExProfile(&daEnWalkerExample_c::build, SpriteId::EN_WALKER_EXAMPLE, &WalkerExSpriteData, ProfileId::EN_KERONPA, ProfileId::EN_WALKER_EXAMPLE, "EN_WALKER_EXAMPLE", WalkerExArcList, 0x12);

void daEnWalkerExample_c::initialize() {
    // set the variables used for walking/turning states
    //walkAngles[0] = 0x1000;
    //walkAngles[1] = 0xF000;
    //turnAmt = 0x200;

    turnsAtLedge = true;
    walkSpeed = 0.5;
    offsetForLedgeCheck = 2.5;

    // hitbox collider
    ActivePhysics::Info walker_cc;

    walker_cc.xDistToCenter = 0.0;
	walker_cc.yDistToCenter = 8.0;
	walker_cc.xDistToEdge = 8.0;
	walker_cc.yDistToEdge = 8.0;
	walker_cc.category1 = 0x3;
	walker_cc.category2 = 0x0;
	walker_cc.bitfield1 = 0x6F;
	walker_cc.bitfield2 = 0xFFBAFFFE;
	walker_cc.unkShort1C = 0;
	walker_cc.callback = &dEn_c::collisionCallback;

    aPhysics.initWithStruct(this, &walker_cc);
    aPhysics.addToList();

    // tile collider
	static const lineSensor_s below(-4<<12, 4<<12, 0<<12);
	static const pointSensor_s above(0<<12, 16<<12);
    static const lineSensor_s adjacent(3<<12, 8<<12, 8<<12);

	collMgr.init(this, &below, &above, &adjacent);

    // ice actor stuff
    ice_struct.scale = (Vec){1.3, 1.3, 1.3};
    ice_struct.pos.y = -0.4;
    
    // terminal velocity
    max_speed.y = -4.0;

    // set direction relative to player
    direction = dSprite_c__getXDirectionOfFurthestPlayerRelativeToVEC3(this, pos);
	rot.y = walkAngles[direction];

    scale.x = 1.0;
    scale.y = 1.0;
    scale.z = 1.0;

    // these are used to set the "center" of the sprite, this depends on the model really...
    // the Y one is the important one, it's usually 8
    // you'll also need to adjust the positions of the tile collider, hitbox, and spawn offset
    // if your sprite isn't spinning around it's center, you set this wrong
    pos_delta2.x = 0.0;
    pos_delta2.y = 8.0;
    pos_delta2.z = 0.0;

    // drawing bounds
    spriteSomeRectX = 16.0;
	spriteSomeRectY = 16.0;
	_320 = 0.0;
	_324 = 8.0;

    // enable "hard hit" effects
    flags_4FC |= 2;

    // set intial state
    doStateChange(&StateID_Walk);

    return;
}

void daEnWalkerExample_c::loadModel() {
    resFile.data = getResource("walker", "g3d/walker.brres");
    nw4r::g3d::ResMdl bmdl = resFile.GetResMdl("walker");
	model.setup(bmdl, &allocator, 0x227, 1, 0);
	SetupTextures_Enemy(&model, 0);

	nw4r::g3d::ResAnmChr resAnmChr = resFile.GetResAnmChr("wait");
	anmChr.setup(bmdl, resAnmChr, &allocator, 0);
    anmChr.bind(&model, resAnmChr, 0);
	model.bindAnim(&anmChr, 0.0);
	anmChr.setUpdateRate(1.0);

    return;
}

void daEnWalkerExample_c::updateModel() {
    model._vf1C();
    return;
}