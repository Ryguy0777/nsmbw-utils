#pragma once

#include <daEnKuribo_c.h>
#include <profile.h>
#include <sfx.h>

class daEnHeiho_c : public daEnKuriboBase_c {
    public:
        enum heyhoTypes {
            Walker, //walks, falls at ledges
            WalkerLedge, //walks, turns at ledges
            Sleeper, //sleeps
            Jumper, //jumps
            Pacer, //walks a specific distance
        };

        heyhoTypes type;
        int color;
        u8 health;
        int distance; //only for pacer
        u8 spawnDir; //only used for jumper and pacer

        int timer;
	    float dying;
	    float Baseline; //pacer start position
        float finalPos[2]; //pacer end position
        int jumpCounter;
        u16 hhAngles[2];
        bool hasSetWalkAnim;

        mEf::es2 effect;
        
        dStateBase_c *stateForRecover;
 
        void dieFall_Begin();
        void dieFall_Execute();

        void dieOther_Begin();
		void dieOther_Execute();

        bool CreateIceActors();

        void initializeState_Walk();
        void executeState_Walk();
        void initializeState_Turn();
        void executeState_Turn();
        void initializeState_TrplnJump();
        void executeState_TrplnJump();

        void reactFumiProc(dStageActor_c* killingActor);

        void loadModel();

        void initialize();
        void setTurnState();
        void vf2D0(dStageActor_c* collidingActor);

        void setWalkSpeed();

        void playChrAnim(const char* name, int playsOnce, float unk2, float rate);
        bool checkForLedge(float xOffset);

        static dActor_c *build();

        USING_STATES(daEnHeiho_c);
        DECLARE_STATE(Sleep);
        DECLARE_STATE(Jump);
        DECLARE_STATE(Dizzy);
        DECLARE_STATE(Idle);
};