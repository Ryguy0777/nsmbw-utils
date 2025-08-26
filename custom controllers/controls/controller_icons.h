#pragma once
#include <controls/custom_controllers.h>
#include <layout/dfukidashiInfo_c.h>

// doing these classes this way to prevent duplicate class declarations

class CourseSelectGuide {
    public:
        void *vtable;
        u32 unk;
        m2d::EmbedLayout_c layout;

        // state machines
		dStateWrapper_c<CourseSelectGuide> state_courseInfo, state_guide;
		
		dStateWrapper_c<CourseSelectGuide> 
			state_mapViewScrollUp, state_mapViewScrollDown,
			state_mapViewScrollLeft, state_MapViewScrollRight;

		dStateWrapper_c<CourseSelectGuide> state_shadow, state_mapView;

        nw4r::lyt::Pane
            *rootPane, *N_IconPos1P_00, *N_IconPos2P_00, *N_IconPos3P_00,
            *N_IconPos4P_00, *N_mapArrow_00, *N_proportionL_00, *N_proportionR_00,
            *N_proportionC_00, *N_guideViewC_00, *N_guideViewR_01, *N_left_00;

        nw4r::lyt::TextBox
            *T_worldNum_00, *T_cSelect_00, *T_cSelect_pic,
            *T_lifeNumber_00, *T_lifeNumber_01, *T_lifeNumber_02,
            *T_lifeNumber_03, *T_guideViewLS_00, *T_guideViewL_01;

        nw4r::lyt::Picture
            *P_cC_1_00, *P_cC_2_00, *P_cC_3_00, *P_cC_1s_00,
            *P_cC_2s_00, *P_cC_3s_00, *P_flagSkull_00, *P_marioFace_00,
            *P_luigiFace_00, *P_BkinoFace_00, *P_YkinoFace_00, *P_bgShadow_00;

        u32 lastControllerTypeUsed, currentWorldNum, currentLevelNum, currentLevelGroup,
		currentLivesValue[4], timer_WorldCourseOnStageWait, timer_GuideOnStageWait,
		currentLivesAlpha, isLivesFadedIn;

        u8 keyPressValue;
		bool layoutLoaded;
		bool shouldHideLives, shouldHideCourseInfo, hidingCourseInfo, setCourseInfoToLastFrame, hidingGuide, shouldHideGuide;
		u8 _43C;
		bool setLivesAndGuideToLastFrame, keyWasPressed, exitingMapView;
		u8 _440;
		bool isVisible;
		bool inhibitHUDShowAndHide;
		bool showShadow, hideShadow;
		bool isMapViewExitVisible, enterMapView, exitMapView, enteringMapView;
		bool animationActive;
		bool upArrowAnimActive, downArrowAnimActive, leftArrowAnimActive, rightArrowAnimActive;
		bool livesUpdated;

		static CourseSelectGuide *instance;
};

class WorldSelectGuide : public dBase_c {
public:
	m2d::EmbedLayout_c layout;                    // 0x70
	nw4r::lyt::Pane *rootPane;					  // 0x208
	nw4r::lyt::TextBox *T_worldNum_00;			  // 0x20C
	nw4r::lyt::Picture *P_pochi_00, *P_pochi_01;  // 0x210, 0x214
	nw4r::lyt::Picture *P_pochi_02, *P_pochi_03;  // 0x218, 0x21C
	nw4r::lyt::Pane *N_wCCGuide_00, *N_title;	  // 0x220, 0x224
	bool layoutLoaded;							  // 0x228
	bool visible;								  // 0x229
	bool showReds;								  // 0x22A
	bool showTopRed;							  // 0x22B
	u32 worldNum;								  // 0x22C
	u32 nextWorldNum;							  // 0x230
	f32 titleYPos;								  // 0x234

    //Total Size: 0x238
};

class CharacterChangeIndicator : public dBase_c {
public:
    m2d::EmbedLayout_c layout;           // 0x70
    nw4r::lyt::Pane *rootPane;           // 0x208
    nw4r::lyt::Picture *P_lamp_08;       // 0x20C
    nw4r::lyt::Picture *P_lamp_09;       // 0x210
    nw4r::lyt::Picture *P_lamp_10;       // 0x214
    nw4r::lyt::Picture *P_lamp_11;       // 0x218
    nw4r::lyt::Picture *P_InfoBaseS_00;  // 0x21C
    nw4r::lyt::Picture *P_InfoBase_00;   // 0x220
    nw4r::lyt::TextBox *T_buttonInfo_00; // 0x224
    nw4r::lyt::TextBox *T_buttonInfo_01; // 0x228
    nw4r::lyt::TextBox *T_button_00;     // 0x22C
    nw4r::lyt::TextBox *T_button_01;     // 0x230
    float _234;                          // 0x234
    bool layoutLoaded;                   // 0x238
    bool visible;                        // 0x239
    u8 isFlash;                          // 0x23A
    u8 _23B;                             // 0x23B
    int playerIndex;                     // 0x23C
    u32 unk1;                            // 0x240
    u32 unk2;                            // 0x244
    int currentState;                    // 0x248
    int buttonInfoMsgID;                 // 0x24C
};
