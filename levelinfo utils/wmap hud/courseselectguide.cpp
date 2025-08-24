#include <game.h>
#include <utils.h>
#include "levelinfo.h"

class dCourseSelectGuide_c {
    public:
        void *vtable;
        u32 unk;
        m2d::EmbedLayout_c layout;

        // state machines
		dStateWrapper_c<dCourseSelectGuide_c> state_courseInfo, state_guide, state_mapViewScrollUp,
		state_mapViewScrollDown, state_mapViewScrollLeft, state_MapViewScrollRight, state_shadow,
		state_mapView;

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
		bool layoutLoaded, shouldHideLives, shouldHideCourseInfo, hidingCourseInfo, setCourseInfoToLastFrame,
		hidingGuide, shouldHideGuide, _43C, setLivesAndGuideToLastFrame, keyWasPressed, exitingMapView, _440,
		isVisible, inhibitHUDShowAndHide, showShadow, hideShadow, isMapViewExitVisible, enterMapView,
		exitMapView, enteringMapView, animationActive, upArrowAnimActive, downArrowAnimActive,
		leftArrowAnimActive, rightArrowAnimActive, livesUpdated;
		
        //our new func
        void NewUpdateLevelDisplay(u32 param);
};

void dCourseSelectGuide_c::NewUpdateLevelDisplay(u32 param) {
    if (param == 1) {
		currentLevelNum = 0xfffffffe;
	} else {
		T_cSelect_pic->SetVisible(false);
		T_cSelect_00->SetVisible(true);

		dLevelInfo_c::entry_s *level = dLevelInfo_c::s_info.searchBySlot(currentWorldNum, currentLevelNum);
		if (level) {
			const wchar_t *convWorldName;
			const wchar_t *convLevelName;

			convWorldName = getWorldNumber(level->displayWorld);
			convLevelName = getLevelNumber(level->displayLevel);

			T_worldNum_00->SetString(convWorldName);

			if (level->displayLevel > 19) {
				T_cSelect_pic->SetVisible(true);
				T_cSelect_00->SetVisible(false);
				T_cSelect_pic->SetString(convLevelName);
			} else {
				T_cSelect_pic->SetVisible(false);
				T_cSelect_00->SetVisible(true);
				T_cSelect_00->SetString(convLevelName);
			}
		} else {
			if(currentLevelNum > 254) { //get a dot
				T_cSelect_pic->SetVisible(true);
				T_cSelect_00->SetVisible(false);
				T_cSelect_pic->SetString(L"6");
				dLevelInfo_c::entry_s *liWorld = dLevelInfo_c::s_info.searchBySlot(currentWorldNum, 38);
                if (liWorld) {
                    const wchar_t *worldNum;
                    worldNum = getWorldNumber(liWorld->displayWorld);
                    T_worldNum_00->SetString(worldNum);
                } else {
                    T_worldNum_00->SetString(L"?");
                }
			} else {
				T_cSelect_pic->SetVisible(false);
				T_cSelect_00->SetVisible(true);
				T_cSelect_00->SetString(L"?");
				T_worldNum_00->SetString(L"?");
			}
		}
	}
    return;
}
