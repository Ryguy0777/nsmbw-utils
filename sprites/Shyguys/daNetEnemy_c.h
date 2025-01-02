#include <game.h>

class daNetEnemy_c : public dEn_c {
    public:
        u32 isNetTurn; //don't think this actually exists?

        int onExecute();

        ~daNetEnemy_c();

        virtual void initializeState_NetWait();
        virtual void executeState_NetWait();
        virtual void finalizeState_NetWait();
        virtual void initializeState_NetMove();
        virtual void executeState_NetMove();
        virtual void finalizeState_NetMove();

        virtual void mdlPlay();
        virtual void calcMdl();

        USING_STATES(daNetEnemy_c);
		REF_NINTENDO_STATE(NetWait);
		REF_NINTENDO_STATE(NetMove);
};