#pragma once
#include <game.h>
#include <sfx.h>

// base walking enemy class, for walking enemy sprites
class daEnWalkerBase_c : public dEn_c {
    public:
        // model stuff
        mHeapAllocator_c allocator;

        nw4r::g3d::ResFile resFile;
        m3d::mdl_c model;
        m3d::anmChr_c anmChr;

        // internal variables for walking/turning state
        u16 walkAngles[2];
        u16 turnAmt;

        bool turnsAtLedge;
        float walkSpeed;
        float offsetForLedgeCheck;

        DoSomethingCool ice_struct;

        int onCreate();
        int onExecute();
        int onDraw();

        void _vf5C(); // calc model matricies

        void spriteCollision(ActivePhysics *apThis, ActivePhysics *apOther);
        void playerCollision(ActivePhysics *apThis, ActivePhysics *apOther);
		void yoshiCollision(ActivePhysics *apThis, ActivePhysics *apOther);

        bool CreateIceActors();

        virtual void initialize(); // called in create

        virtual void loadModel();
        virtual void updateModel(); // update animations, etc
        virtual void onCalcModel(); // called in _vf5C

        // reacting to jumps
        virtual void reactJump(dStageActor_c* player);
        virtual void reactSpinJump(dStageActor_c* player);
        virtual void reactYoshiJump(dStageActor_c* yoshi);

        // called during walk and turn states
        virtual void preWalk();
        virtual void onWalk();
        virtual void postWalk();

        virtual void preTurn();
        virtual void onTurn();
        virtual void postTurn();

        virtual void setWalkSpeed();
        virtual bool checkForLedge();

        USING_STATES(daEnWalkerBase_c);
        DECLARE_STATE_VIRTUAL(Walk);
        DECLARE_STATE_VIRTUAL(Turn);
};