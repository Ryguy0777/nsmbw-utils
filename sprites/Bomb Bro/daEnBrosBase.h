#pragma once
#include <game.h>

#ifndef static_assert
    // https://stackoverflow.com/a/1597129
    #define TOKENPASTE(x, y) x ## y
    #define TOKENPASTE2(x, y) TOKENPASTE(x, y)

    #define static_assert(condition) typedef int TOKENPASTE2(static_assert_, __LINE__)[(condition) ? 1 : -1]
#endif // static_assert

#ifndef print_sizeof
    // https://stackoverflow.com/questions/20979565/how-can-i-print-the-result-of-sizeof-at-compile-time-in-c
    #define print_sizeof(type) char (*__kaboom)[sizeof(type)] = 1
#endif // static_assert

class daEnBrosBase_c : public dEn_c {
    public:
        class nodeCallback_c {
            public:
                virtual ~nodeCallback_c();
                virtual void timingA();
                virtual void timingB();
                virtual void timingC();

                daEnBrosBase_c *owner;
        };

        mHeapAllocator_c allocator;

		nw4r::g3d::ResFile resFile;
		m3d::mdl_c model;
		m3d::anmChr_c anmChr;
		nw4r::g3d::ResAnmTexPat resPat;
        m3d::anmTexPat_c anmTexPat;

        mMtx mtx1;
        mMtx mtx2;
        mMtx mtx3;
        mMtx mtx4;
        mMtx mtx5;

        int countdownUntilAttacking;
        int countdownAfterTouchingGround;
        int countsUpDirChangesInLiftHMB;
        int doPlayWalkBackAnim;
        int countdownWereNotMovingFor;
        int repeatedAttackCount;
        int setToZeroInMove;

        u32 unk;
        float _6FC, _700;
        u16 _704, _706;
        VEC3 startPos;
        u8 facingDirection_maybe; //might be u32
        u8 unk2[3];
        int heldProjectileID_maybe;

        nodeCallback_c callback;

        mEf::es2 effect;

        //overrides
		int onCreate();
		int onDelete();
		int onExecute();
		int onDraw();

        ~daEnBrosBase_c();

        void _vf5C();

        void playerCollision(ActivePhysics *apThis, ActivePhysics *apOther);
		void yoshiCollision(ActivePhysics *apThis, ActivePhysics *apOther);

        bool collisionCatD_Drill(ActivePhysics *apThis, ActivePhysics *apOther);
        bool collisionCat7_GroundPound(ActivePhysics *apThis, ActivePhysics *apOther);
        bool collisionCat2_IceBall_15_YoshiIce(ActivePhysics *apThis, ActivePhysics *apOther);
	    bool collisionCat13_Hammer(ActivePhysics *apThis, ActivePhysics *apOther);

        void dieFumi_Begin();
	    void dieFumi_Execute();
	    void dieFumi_End();
	    void dieFall_Begin();
	    void dieFall_Execute();
	    void dieFall_End();
        
        void _vf220(void *player);
        bool CreateIceActors();
	    void _vf230();
        void _vf238();

        //virtual funcs
        virtual void initializeState_Move();
        virtual void executeState_Move();
        virtual void finalizeState_Move();
        virtual void initializeState_Attack();
        virtual void executeState_Attack();
        virtual void finalizeState_Attack();
        virtual void initializeState_JumpSt();
        virtual void executeState_JumpSt();
        virtual void finalizeState_JumpSt();
        virtual void initializeState_Jump();
        virtual void executeState_Jump();
        virtual void finalizeState_Jump();
        virtual void initializeState_JumpEd();
        virtual void executeState_JumpEd();
        virtual void finalizeState_JumpEd();
        virtual void initializeState_AirAttack();
        virtual void executeState_AirAttack();
        virtual void finalizeState_AirAttack();

        virtual double getProjCreateFrameNum();
        virtual double getProjReleaseFrameNum();
        virtual void setSpeed();
        virtual int isOnScreen_maybe();
        virtual int getColor();
        virtual void calledInMoveBegin();
        virtual void getAdjustOffset(); //might return vec idk
        virtual void calcMoveDir();
        virtual void messWithHeldProjRotation();
        virtual void weaponCreate();
        virtual void weaponAttack();
        virtual void setJumpCnt();
        virtual void setJump();
        virtual void beginJump();
        virtual int isInvalidBg();
        virtual bool isAttackOK();
        virtual void beginAttk();
        virtual void setAttackAnm();
        virtual void initMoveCnt();
        virtual void initPosLv();
        virtual void initType();
        virtual void entryHIO();
        virtual void removeHIO();

        USING_STATES(daEnBrosBase_c);
        REF_NINTENDO_STATE(Move);
        REF_NINTENDO_STATE(Attack);
        REF_NINTENDO_STATE(JumpSt);
        REF_NINTENDO_STATE(Jump);
        REF_NINTENDO_STATE(JumpEd);
        REF_NINTENDO_STATE(AirAttack);
};