#pragma once

#include <daEnKuribo_c.h>
#include <profile.h>
#include <sfx.h>

class daEnKuribon_c : public daEnKuribo_c {
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

        void updateModel();
        void playWalkAnim();

        void doCarryCollider();
        void killWhenCarried(dAcPy_c *player);
        void checkTimer();
        void playKickSound(int player_id);

        static dActor_c *build();

        USING_STATES(daEnKuribon_c);
        DECLARE_STATE(Dizzy);
        DECLARE_STATE(Carry);
        DECLARE_STATE(Throw);
        DECLARE_STATE(Kick);
        DECLARE_STATE(Recover);
};