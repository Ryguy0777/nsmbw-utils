#pragma once
#include <game.h>
#include <sfx.h>

class daFireBall_Base_c : public dActorState_c {
    public:
        mHeapAllocator_c allocator;
        StageActorLight light;
        u32 didSplash;
        u32 killCountDownToDestroy;
        u32 liquidStat;
        u32 _414;
        u32 liquidHeight;
        Vec startVec;
        u32 _428;
        mEf::es2 effect;
        
        //overrides
		int onCreate();

		int onDelete();

		int onExecute();
        int beforeExecute();

		int onDraw();

        void willBeDeleted();

        ~daFireBall_Base_c();
        
        void eatIn();
	    void disableEatIn();

        //new vfuncs
        virtual void initializeState_Move();
        virtual void executeState_Move();
        virtual void finalizeState_Move();
        virtual void initializeState_Kill();
        virtual void executeState_Kill();
        virtual void finalizeState_Kill();
        virtual void initializeState_EatIn();
        virtual void executeState_EatIn();
        virtual void finalizeState_EatIn();
        virtual void initializeState_EatNow();
        virtual void executeState_EatNow();
        virtual void finalizeState_EatNow();

        virtual bool initialize();
        virtual bool createCheck();
        virtual void setCc();
        virtual void setBc();
        virtual void chgZpos();
        virtual void fireEffect();
        virtual void beginSplash(float splash_pos_y);
        virtual void beginYoganSplash(float splash_pos_y);
        virtual void beginPoisonSplash(float splash_pos_y);
        virtual float getLightRad();
        virtual void _vf138();
        virtual void _vf13C();

        void killFireBall();

        //states
        USING_STATES(daFireBall_Base_c);
		REF_NINTENDO_STATE(Move);
		REF_NINTENDO_STATE(Kill);
		REF_NINTENDO_STATE(EatIn);
		REF_NINTENDO_STATE(EatNow);
};