#pragma once

#include <game.h>
#include <controls/lib/pad.h>
#include <controls/lib/si.h>

struct KPADEXStatus_fs {
    Vec2 stick;
    Vec3 acc;
    float acc_value, acc_speed;
};

struct KPADEXStatus_cl {
    u32 hold, trig, release;
    Vec2 lstick;
    Vec2 rstick;
    float ltrigger, rtrigger;
};

struct KPADEXStatus_bl {
    f64 tgc_weight;
    f64 weight[4];
    f64 weight_ave[4];
    s32 weight_err;
    s32 tgc_weight_err;
};

// doesn't actually exist
// guitar techically uses EXStatus_cl but this is for ease of reading
struct KPADEXStatus_gh {
    u32 hold, trig, release;
    Vec2 stick;
    u32 notsure;
    float slider;
    u32 alsonotsure;
    float whammy;
};

union KPADEXStatus {
    KPADEXStatus_fs fs;
    KPADEXStatus_cl cl;
    KPADEXStatus_bl bl;
    KPADEXStatus_gh gh;
};

#define WPAD_DEV_CORE                   0   // no extension
#define WPAD_DEV_FREESTYLE              1   // nunchuck
#define WPAD_DEV_CLASSIC                2   // classic
#define WPAD_DEV_BALANCE_CHECKER        3   // balance board
#define WPAD_DEV_VSM                    4   // vitality sensor

#define WPAD_DEV_MPLS                   5   // motionplus
#define WPAD_DEV_MPLS_FREESTYLE         6   // motionplus + nunchuck
#define WPAD_DEV_MPLS_CLASSIC           7   // motionplus + classic

#define WPAD_DEV_TRAIN                  16  // train controller
#define WPAD_DEV_GUITAR                 17  // guitar hero
#define WPAD_DEV_DRUM                   18  // guitar hero drums
#define WPAD_DEV_TAIKO                  19  // taiko drum

// these aren't real
#define WPAD_DEV_GAMECUBE               30 // gamecube controller
#define WPAD_DEV_GCN_FUTURE             31 // invalid gc device

#define WPAD_DEV_MPLS_FUTURE            250 // motionplus unsupported
#define WPAD_DEV_FUTURE                 251 // unsupported
#define WPAD_DEV_NOT_SUPPORTED          252 // not recognized
#define WPAD_DEV_NOT_FOUND              253 // no wiimote
#define WPAD_DEV_UNKNOWN                255 // not initialized 

struct KPADStatus {
    u32 hold; // held down
    u32 trigger; // pressed since last sample
    u32 release; // released since last sample
    Vec acc;
    float acc_magnitude, acc_variation;
    Vec2 pos;
    Vec2 vec;
    f32 speed;
    Vec2 horizon;
    Vec2 hori_vec;
    f32 hori_speed;
    f32 dist;
    f32 dist_vec;
    f32 dist_speed;
    Vec2 acc_vertical;
    u8 dev_type; // 0 = wiimote, 1 = nunchuck, 2 = classic, 0x11 = guitar, 0xfb = unsupported, 0xff = reading?
    s8 wpad_err;
    s8 dpd_valid_fg;
    u8 data_format;
    KPADEXStatus ex_status;
};

namespace EGG {
    class CoreController {
    public:
        // not making these virtual for ease of patching
        void beginFrame(); // 802bd0d0
        bool downTrigger(u32 button); // 80017ce0

        void stopRumbleMgr(); // 802bccb0

        void mapButtons();

        void *vtable;
        u32 channel;
        u32 held, triggered, released;
        u32 _14;
        KPADStatus status;
        KPADStatus weirdStatusThing[15];
        int readStatusIdx;
        u8 flag, pad1[3];
        Vec3 accel;
        Vec2 dpdPos;
        u32 idleTime, motorPattern;
        bool enableMotor;
        u8 pad2[3];
        s32 motorFrameDuration;
        u8 motorPatternLength, motorPatternPos;
        void *rumbleMgr;
        Mtx postureMatrix, postureMatrixPrev;
        u8 accelFlags, pad3[3];
        s32 maxAccelFrameTime, accelFrameTime[3];
        f32 maxAccelDiff;
        Vec3 prevAccel;
    };

    class CoreControllerMgr {
    public:
        // these are actually virtual, but like with CoreController,
        // I trick the compiler into treating them as non-virtual to simplify how I patch them
        void beginFrame(); // 0x802bdbb0
        void endFrame(); // 802bdc60

        void readPad();
        void resetPad();
        
        // I hate this
        u8 unk[0xe2c];
        u32 *devTypePtr; // 0xe2c
    };
}

typedef struct ControllerType {
    enum Value {
        Wiimote,
        Nunchuck,
        Invalid,
        None,
        //NEW:
        Classic,
        GameCube,
        Guitar
    };
};

extern EGG::CoreController *CoreControllers[4];

class dRemoconMng_c {
public:
    class dConnect_c {
    public:
        class dExtension_c {
        public:
            u32 _0; // might be vtable
            u32 controllerID;
            u32 extensionType;
            dStateWrapper_c<dExtension_c> state;

            USING_STATES(dExtension_c);
            REF_NINTENDO_STATE(Freestyle); // nunchuck
            REF_NINTENDO_STATE(None); // wiimote only
            REF_NINTENDO_STATE(Other); // invalid controller type
            REF_NINTENDO_STATE(Wait); // disconnected wiimote

            DECLARE_STATE(Classic); // new, classic controller
            DECLARE_STATE(GameCube); // new, gamecube controller
            DECLARE_STATE(Guitar); // new, guitar hero guitar
        };
        void *vtable;
        int controllerID;
        dExtension_c extension;
        u32 someValueObtainedFromSub8016F5A0;
        bool willDisconnect, is_rumble_enabled;
        u8 _56, _57;
        u32 counter;
        dStateWrapper_c<dConnect_c> state;
    };
    void *vtable;
    dConnect_c *wiimotes[4];
};

extern dRemoconMng_c *RemoconMngInstance;

class dGameKeyCore_c {
public:
    void *vtable;
    u32 id; // mPad::CH_e
    u32 controllerType;
    u32 hold, prev_hold;
    u32 held_buttons, prev_held_buttons;
    u32 newly_pressed;
    u32 _20, _24, _28, _2C, _30;
    Vec acc, prev_acc;
    Vec2 acc_vert_x, prev_acc_vert_x;
    Vec2 acc_vert_y, prev_acc_vert_y;
    Vec2 acc_vert_z;
    Vec2 angle, last_angle;
    float move_dist, prev_move_dist;
    bool is_shaking;
    u8 _8D;
    u16 tiltAmount;
    s8 shake_timer1, shake_timer2, shake_timer3;
    u8 _93;
};

class dGameKey_c {
public:
    void *vtable;
    dGameKeyCore_c *remocons[4];
};

extern dGameKey_c *GameKeyInstance;

struct BIEntry {
    u32 data[10];
};

class dAcPyKey_c {
public:
    int playerID;
    u16 heldButtons, nowPressed;
    u16 lastHeldButtons, lastNowPressed;
    u16 applyToHeldButtonsWithFlag7_permanent, applyToHeldButtonsWithFlag7_transient;
    u16 forcedShakeValue;
    u16 flags; // flag 0: No Input flag 3: force Two on flag 4: force Two off flag 6: Nothing But Holding Right flag 7: Force Specific Values flag 8: No Shaking
    u8 downHeldCounter;
    u8 dontResetGPValuesNextTick;
    u8 currentlyHoldingDown;
    bool shake_jump;
    bool shaking_18;
    bool wasActionExecuted;
    u16 _1a;
    u32 countdownAfterFlag6Deactivated;
    u32 _20;
    BIEntry rollingHistoryOfTwo[4];
    BIEntry rollingHistoryOfTwoModifiedByFlags[4];

    u16 newTriggerOne();
    u16 newButtonOne();
    u16 newButtonDush();
};

#define WPAD_BUTTON_LEFT            0x0001
#define WPAD_BUTTON_RIGHT           0x0002
#define WPAD_BUTTON_DOWN            0x0004
#define WPAD_BUTTON_UP              0x0008
#define WPAD_BUTTON_PLUS            0x0010
#define WPAD_BUTTON_2               0x0100
#define WPAD_BUTTON_1               0x0200
#define WPAD_BUTTON_B               0x0400
#define WPAD_BUTTON_A               0x0800
#define WPAD_BUTTON_MINUS           0x1000
#define WPAD_BUTTON_HOME            0x8000

#define WPAD_BUTTON_Z               0x2000
#define WPAD_BUTTON_C               0x4000

#define WPAD_CL_BUTTON_UP           0x0001
#define WPAD_CL_BUTTON_LEFT         0x0002
#define WPAD_CL_TRIGGER_ZR          0x0004
#define WPAD_CL_BUTTON_X            0x0008
#define WPAD_CL_BUTTON_A            0x0010
#define WPAD_CL_BUTTON_Y            0x0020
#define WPAD_CL_BUTTON_B            0x0040
#define WPAD_CL_TRIGGER_ZL          0x0080
#define WPAD_CL_TRIGGER_R           0x0200
#define WPAD_CL_BUTTON_PLUS         0x0400
#define WPAD_CL_BUTTON_HOME         0x0800
#define WPAD_CL_BUTTON_MINUS        0x1000
#define WPAD_CL_TRIGGER_L           0x2000
#define WPAD_CL_BUTTON_DOWN         0x4000
#define WPAD_CL_BUTTON_RIGHT        0x8000

#define GH3_BUTTON_STRUM_UP         0x0001
#define GH3_BUTTON_YELLOW		    0x0008
#define GH3_BUTTON_GREEN		    0x0010
#define GH3_BUTTON_BLUE		        0x0020
#define GH3_BUTTON_RED		        0x0040
#define GH3_BUTTON_ORANGE   		0x0080
#define GH3_BUTTON_PLUS		        0x0400
#define GH3_BUTTON_MINUS    		0x1000
#define GH3_BUTTON_STRUM_DOWN   	0x4000

static PADStatus PadStatus[4];

// created on game boot and stays in memory constantly until game shutdown

#define CCFLAG_HAS_CUSTOM           0x01
#define CCFLAG_NO_AUTOCONNECT       0x02
#define CCFLAG_HAS_AUTOCONNECT      0x04

#define CCFLAG_IS_PROCON            0x08
#define CCFLAG_NO_ACC               0x10
#define CCFLAG_NO_WIIMOTE           0x20

struct dPADInfo {
    u32 hold, trig, release;
    Vec2 stick;
    Vec2 substick;
    float ltrigger, rtrigger;
};

struct dCustomControllerInfo {
    u32 controllerType;
    u8 flags;
    u32 lastPressed;
    dPADInfo padInfo;
};

typedef struct GCCType {
    enum Value {
        GCCInvalid = -1,
        GCCNone = 0,
        GCCStandard = 1,
    };
};

class dCustomController_c {
public:
    dCustomControllerInfo controllers[4];
    bool isHBMOpen;

    static dCustomController_c *instance;
    
    void mapClassicButtons(EGG::CoreController *controller, int controllerID);
    void mapGameCubeButtons(EGG::CoreController *controller, int controllerID);
    void mapGuitarButtons(EGG::CoreController *controller, int controllerID);
    void clearWiimotedata(KPADStatus *kStatus, bool disablePointer);

    u32 checkForGCConnection(int channel);
    void setGamecubeDevType(int channel);
    static void setPadInfo();

    static void changeRemoconMgrState(dRemoconMng_c::dConnect_c::dExtension_c *self, u32 extension);
    static void shakeController(int channel);
};

dCustomController_c *dCustomController_c::instance = 0;

inline dCustomControllerInfo *getCustomController(int channel) {
    return &dCustomController_c::instance->controllers[channel];
}

extern void *replayClasses[4];

inline dCustomControllerInfo *getCustomController(int channel, bool accountForDemo) {
    if (accountForDemo) {
        if (replayClasses[channel] == NULL) {
            return &dCustomController_c::instance->controllers[channel];
        } else {
            return NULL;
        }
    }
    return &dCustomController_c::instance->controllers[channel];
}