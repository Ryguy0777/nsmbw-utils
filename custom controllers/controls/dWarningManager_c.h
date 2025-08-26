#pragma once

#include <controls/custom_controllers.h>
#include <controls/dWarningClassic_c.h>
#include <controls/dWarningGameCube_c.h>

// old ass shit

class dWarningOther_c {
public:
    void *vtable;
    m2d::EmbedLayout_c layout;
    dStateWrapper_c<dWarningOther_c> acState;

    nw4r::lyt::Pane *rootPane;
    nw4r::lyt::TextBox *T_player;
    nw4r::lyt::Pane *W_extensionCla;

    u8 layoutLoaded, isVisible, _1E6, _1E7, RemNumber;

    int createLayout(); // 80019c10
    bool doDelete(); // 80019e00
    void draw(); // 80019de0
    void execute(); // 80019d70

    void setPlayerNo(u32 num); // 8001a080

    USING_STATES(dWarningOther_c);
    REF_NINTENDO_STATE(StartWait);
};

class dWarningNunchuk_c {
public:
    void *vtable;
    m2d::EmbedLayout_c layout;
    dStateWrapper_c<dWarningNunchuk_c> acState;

    nw4r::lyt::Pane *rootPane;
    nw4r::lyt::TextBox *T_player;
    nw4r::lyt::Pane *W_extensionCN;

    u8 layoutLoaded, isVisible, _1E6, _1E7, RemNumber;

    int createLayout(); // 80018f20
    bool doDelete(); // 80019110
    void draw(); // 800190f0
    void execute(); // 80019080

    USING_STATES(dWarningNunchuk_c);
    REF_NINTENDO_STATE(StartWait);
};

class dWarningYoKo_c {
public:
    void *vtable;
    m2d::EmbedLayout_c layout;
    dStateWrapper_c<dWarningYoKo_c> acState;

    nw4r::lyt::Pane *rootPane;
    nw4r::lyt::TextBox *T_player;
    nw4r::lyt::Pane *W_extensionYoko;

    u8 layoutLoaded, isVisible, _1E6, _1E7, RemNumber;

    int createLayout(); // 8001a930
    bool doDelete(); // 8001ab20
    void draw(); // 8001ab00
    void execute(); // 8001aa90

    USING_STATES(dWarningYoKo_c);
    REF_NINTENDO_STATE(StartWait);
};

class dWarningBattery_c {
public:
    void *vtable;
    u32 _4;
    m2d::EmbedLayout_c layout;
    dStateWrapper_c<dWarningBattery_c> acState;

    nw4r::lyt::Pane *rootPane;
    nw4r::lyt::Pane 
        *N_batt_00, *N_batt_01, *N_batt_02, *N_batt_03,
        *Null_battPos_1P, *Null_battPos_2P, *Null_battPos_3P, *Null_battPos_4P,
        *N_batt;
    
    nw4r::lyt::Picture
        *P_batB_03, *P_BatB_02, *P_BatB_01, *P_BatB_00, *P_Bat_00,
        *P_batB_13, *P_BatB_12, *P_BatB_11, *P_BatB_10, *P_Bat_01,
        *P_batB_23, *P_BatB_22, *P_BatB_21, *P_BatB_20, *P_Bat_02,
        *P_batB_33, *P_BatB_32, *P_BatB_31, *P_BatB_30, *P_Bat_03;
    
    Vec3 battTranslates;
    u8 unk[36];
    u8 layoutLoaded, isVisible, whichOnesAreActive;
    u8 _287, _288, _289, _28A;

    int createLayout(); // 80016020
    bool doDelete(); // 800162e0
    void draw(); // 800162c0
};

class dWarningErrorInfo_c {
public:
    void *vtable;
    u32 _4;
    m2d::EmbedLayout_c layout;
    dStateWrapper_c<dWarningBattery_c> acState;

    nw4r::lyt::Pane *rootPane;
    nw4r::lyt::TextBox
        *T_infoS_00, *T_info_00, *T_player_00,
        *T_yes_00, *T_yes_01, *T_no_00, *T_no_01,
        *T_center_00, *T_center_01;

    nw4r::lyt::Pane *N_playerButton;
    nw4r::lyt::Picture *P_yesBase_00, *P_noBase_00, *P_centerBase_00;

    u8 unk[12];
    u32 _220;
    u32 _224;
    int _228;
    u8 _22C, isVisible, _22E;
    bool _22F;

    int createLayout(); // 800171f0
    void draw(); // 80017440
    void execute(); // 80017340
};

extern bool WarningMgrCreated; // 8042a5bd
extern u32 WarningMgrForbid; // 8042a5c0
extern bool WarningMgrCheck; // 8042a5bc

class dWarningManager_c : public dBase_c {
public:
    u32 _70;

    dWarningOther_c WarningOther; // 0x74
    dWarningNunchuk_c WarningNunchuk; // 0x260
    dWarningYoKo_c WarningYoKo; // 0x44C
    dWarningBattery_c WarningBattery; // 0x638

    u8 pad2[0x18]; // 0x8C3

    dWarningErrorInfo_c WarningErrorInfo; // 0x8DC

    dStateWrapper_c<dWarningManager_c> acState; // 0xB0C

    u8 pad3[8]; // 0xB48
    u32 _B50;

    u32 lastWiimoteType[4]; // 0xB54

    u32 _B64;
    u32 _B68;
    u32 _B6C;
    u32 _B70;
    u32 _B74;
    u32 _B78;

    u32 maybe_not_array[4]; // 0xB54

    u8 _B8C;
    u8 isBatteryActive; // 0xB8D
    u8 isErrorActive; // 0xB8E
    u8 isSaveErrorActive; // 0xB8F

    u8 _B90;
    u8 _B91;
    u8 _B92;
    u8 _B93;

    //new
    dWarningClassic_c WarningClassic; // 0xB94
	dWarningGameCube_c WarningGameCube; // 0xD80

    int newCreate(); // replaces 8010d5b0
    int newDelete(); // replaces 8010d7e0
    int newDraw(); // replaces 8010d780

    void newAllWarningEnd(bool keepForbid); // replaces 8010e5a0

    void newExecuteWarningCheck(); // replaces 8010df40
    void newExecuteExitAnimeEndWait(); // replaces 8010e2d0

    void NunchukDispSetup(u32 player); // 8010d9d0
    void YoKoDispSetup(u32 player); // 8010da80
    void newOtherDispSetup(u32 player); // replaces 8010db30

    void GotoWarningCheck(); // 0x8010dc40
    bool ForbidCheck(); // 8010dc60
    bool HitAnyKey(); // 8010dbe0

    void ClassicDispSetup(u32 player);
    void GameCubeDispSetup(u32 player);

    USING_STATES(dWarningManager_c);
    REF_NINTENDO_STATE(WarningCheck);
    REF_NINTENDO_STATE(WarningCaution);
    DECLARE_STATE(WarningClassic);
	DECLARE_STATE(WarningGameCube);
};