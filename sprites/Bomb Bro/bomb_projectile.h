#pragma once
#include <game.h>
#include <sfx.h>
#include <UnitType.h>
#include <profile.h>
#include <daEnBrosBase.h>

// the bomb itself
// it's essentially just a hammer that explodes on contact
// in fact, it's so similar you could probably make the
// hammer suit throw these with some minor tweaks

class daBombProjectile_c : public dStageActor_c {
    public:
        daBombProjectile_c();

        dStateWrapper_c<daBombProjectile_c> acState;

        mHeapAllocator_c allocator;

		nw4r::g3d::ResFile resFile;
        nw4r::g3d::ResMdl bmdl;
		m3d::mdl_c model;
        nw4r::g3d::ResAnmClr resClr;
		m3d::anmClr_c anmClr;

        mEf::es2 igniteEffect;

        daEnBrosBase_c *owner; //0x58c
        dAcPy_c *player_owner;

        bool readyToThrow;
        bool shouldExplode;
        bool playerBomb;
        int explosionTimer;

        Vec3 rootPos;

        int onCreate();
        int onExecute();
        int onDraw();

        void _vf5C();
        bool _vf8C(void *other);

        void explodeTiles();

        static dActor_c *build();

        USING_STATES(daBombProjectile_c)
        DECLARE_STATE(ThrowWait);
        DECLARE_STATE(EatWait);
        DECLARE_STATE(Throw);
        DECLARE_STATE(Explode);
};