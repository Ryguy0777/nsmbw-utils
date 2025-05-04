#include <game.h>
#include <utils.h>
#include "levelinfo.h"

class dModelPlayDate_c : public dBase_c {
public:
	dStateWrapper_c<dModelPlayDate_c> state;
	m2d::EmbedLayout_c layout;

	nw4r::lyt::Pane *rootPane;
	nw4r::lyt::TextBox *T_modelTitle_00, *T_needCoin_01, *T_worldNum_00, *T_worldCourse_00, *T_pictureFont_00;
	nw4r::lyt::Picture *P_model1_00, *P_model1S_00, *P_modelBefore1, *P_starCoin_00, *P_wave_00;
	nw4r::lyt::Pane *N_Coin_00, *W_model1_00;

	bool layoutLoaded, visible, willUpdate, animationActive;
	int previousAction, action, movieCost, worldNum, levelNum;
	int currentStatus, previousStatus, movieType, buttonID;

	void loadInfo();
};

void dModelPlayDate_c::loadInfo() {
	dLevelInfo_c::entry_s *level = dLevelInfo_c::s_info.searchBySlot(worldNum, levelNum);
	if (level) {
		const wchar_t *worldName, *levelName;
		worldName = getWorldNumber(level->displayWorld);
		levelName = getLevelNumber(level->displayWorld, level->displayLevel);

		T_worldNum_00->SetString(worldName);
		if (level->displayLevel > 19) {
			T_worldCourse_00->SetVisible(false);
			T_pictureFont_00->SetVisible(true);
			T_pictureFont_00->SetString(levelName);
		} else {
			T_pictureFont_00->SetVisible(false);
			T_worldCourse_00->SetVisible(true);
			T_worldCourse_00->SetString(levelName);
		}
	} else {
		T_worldNum_00->SetString(L"?");
		T_pictureFont_00->SetVisible(false);
		T_worldCourse_00->SetVisible(true);
		T_worldCourse_00->SetString(L"?");
	}
}