#pragma once
#include <game.h>

class daEnKuriboBase_c : public dEn_c {
	public:
		mHeapAllocator_c allocator;

		nw4r::g3d::ResFile resFile;
		m3d::mdl_c model;
		m3d::anmChr_c anmChr;
		nw4r::g3d::ResAnmTexPat resPat;
        m3d::anmTexPat_c anmTexPat;

		float zOffset;
		u32 isFrozen;
		float savedSpeed;

		mEf::es2 levelEffect;
		u8 levelOneEffect[0x138];

		u16 chkZoneBoundParam;
		u16 pad;

		//overrides
		int onCreate();
		int onDelete();
		int onExecute();
		int onDraw();

		~daEnKuriboBase_c();

		void _vf5C();

		void spriteCollision(ActivePhysics *apThis, ActivePhysics *apOther);
		void playerCollision(ActivePhysics *apThis, ActivePhysics *apOther);
		void yoshiCollision(ActivePhysics *apThis, ActivePhysics *apOther);

		void dieOther_Begin();
		void dieOther_Execute();
		void dieOther_End();

		void playEnemyDownSound2();
		void _vf220(void *player);

		void _vf23C();
		void _vf240();
		int _vf244();

		//virtual funcs
		virtual void initializeState_Walk();
        virtual void executeState_Walk();
        virtual void finalizeState_Walk();
        virtual void initializeState_Turn();
        virtual void executeState_Turn();
        virtual void finalizeState_Turn();
        virtual void initializeState_TrplnJump();
        virtual void executeState_TrplnJump();
        virtual void finalizeState_TrplnJump();

        virtual void calcModel();
        virtual void reactFumiProc(dStageActor_c* killingActor);
        virtual void vf2AC(); // unknown
        virtual void reactSpinFumiProc(dStageActor_c* killingActor); // unofficial name
        virtual void reactYoshiFumiProc(dStageActor_c* killingActor); // unofficial name

        virtual void createModel();
        virtual void loadModel(); // unofficial name
        virtual void loadWingModel(); // unofficial name
        virtual void drawModel();

        virtual void initialize();
        virtual void setTurnState(); // unofficial name
        virtual void vf2D0(dStageActor_c* collidingActor); // unknown

        virtual void setWalkSpeed();
        virtual void playWalkAnim(); // unofficial name
        virtual void updateModel(); // unofficial name
        virtual void walkEffect();

        virtual bool isWakidashi() const;
        virtual bool vf2E8(); // unknown
        virtual bool isBgmSync() const;

		bool isOnEnLiftRemoconTrpln();
		
		//states
		USING_STATES(daEnKuriboBase_c);
		REF_NINTENDO_STATE(Walk);
		REF_NINTENDO_STATE(Turn);
		REF_NINTENDO_STATE(TrplnJump);
		REF_NINTENDO_STATE(DieOther);
};

class daEnKuribo_c : public daEnKuriboBase_c {
	public:
		u32 tumbleSensor[3];

		float targetXPos;
		float targetYPos;

		u8 tumbleState;
		bool keepMovingUpwards;

		u16 nybble11;
		u32 unused;

		int onDelete();
		~daEnKuribo_c();

		void _vf5C();

		bool collisionCat7_GroundPound(ActivePhysics *apThis, ActivePhysics *apOther);

		void playEnemyDownSound2();

		bool CreateIceActors();

		void _vf23C();
		void _vf240();
		int _vf244();

		void reactSpinFumiProc(dStageActor_c* killingActor);
        void reactYoshiFumiProc(dStageActor_c* killingActor);

		void loadWingModel();
        void drawModel();

		void initialize();
        void setTurnState();

		void setWalkSpeed();
		
		void updateModel();
        void walkEffect();

		bool isWakidashi() const;
        bool vf2E8();
        bool isBgmSync() const;

		USING_STATES(daEnKuribo_c);
		REF_NINTENDO_STATE(Born_Split1);
		REF_NINTENDO_STATE(Born_Split2);
		REF_NINTENDO_STATE(BlockAppear);
		REF_NINTENDO_STATE(Down);
		REF_NINTENDO_STATE(Auto_Walk);
		REF_NINTENDO_STATE(Dokan_Down);
		REF_NINTENDO_STATE(EatOut_Ready);
};

class daEnPataKuribo_c : public daEnKuriboBase_c {
	public:
		nw4r::g3d::ResFile wingRes;
		m3d::mdl_c wingModel;
		m3d::anmChr_c wingAnim;

		u16 turningTimer;
		u16 jumpTimer;

		u32 _8DC;
		u32 hasWings;
		u32 _8E4;

		~daEnPataKuribo_c();

		void _vf5C();

		bool collisionCat2_IceBall_15_YoshiIce(ActivePhysics *apThis, ActivePhysics *apOther);

		void playEnemyDownSound2();

		bool CreateIceActors();

		void _vf238();
		void _vf23C();
		void _vf240();
		int _vf244();

		void calcModel();
        void reactFumiProc(dStageActor_c* killingActor);

        void reactSpinFumiProc(dStageActor_c* killingActor);

		void loadWingModel();
        void drawModel();

        void initialize();
        void setTurnState();
        void vf2D0(dStageActor_c* collidingActor);

        void setWalkSpeed();

        void updateModel();
        void walkEffect();

        bool isWakidashi() const;
        bool vf2E8();
        bool isBgmSync() const;

		USING_STATES(daEnPataKuribo_c);
		REF_NINTENDO_STATE(Pata_Walk);
		REF_NINTENDO_STATE(Pata_Turn);
};