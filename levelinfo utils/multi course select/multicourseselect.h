#pragma once
#include <game.h>

class dMultiCourseSelectContents_c : public dBase_c {
public:
	m2d::EmbedLayout_c layout;
	dStateWrapper_c<dMultiCourseSelectContents_c> state;
	nw4r::lyt::Pane *rootPane;
	nw4r::lyt::Picture
		*P_enpty, *P_corse_00,
		*P_corseFrame_00, *P_clearFrame_00,
		*P_allClearF_00, *P_clearBase_00,
		*P_coin_00;
	nw4r::lyt::TextBox
		*T_corseNum_00, *T_corsePic_00,
		*T_W_corseNum_00, *T_W_corseNum_01,
		*T_W_corseNum_02, *T_W_corsePic_00,
		*T_clear_00, *T_allClear_00;
	nw4r::lyt::Pane *N_courseInfo_00, *N_clear_00;

	int action; // 1 = on, 2 = hit, 3 = off, 5 = menu pause, 6 = cleared
	bool layoutLoaded, visible, _292, animationActive, buttonSelected;
	u8 pad[3];

	int buttonID, currentPage, previousPage; // last one is used to check if data needs to be updated
	int cursorState, timer; // cursor states: 0 = none, 1 = being showed, 2 = cancelled
	int worldNumber, levelNumber;

	const char *getTextureName(int world, int level);
	void loadInfo(); // new
};

class dMultiCourseSelect_c : public dBase_c {
public:
	m2d::EmbedLayout_c layout;
	m2d::ResAccLoader_c resAccLoader;
	dMultiCourseSelectContents_c *contents[20];
	dStateWrapper_c<dMultiCourseSelect_c> state;

	nw4r::lyt::Pane *rootPane;
	nw4r::lyt::Picture
		*P_Bg_00, *P_arrowCL_00, *P_arrowCR_00,
		*P_title_00, *P_title_00_vs;
	nw4r::lyt::Pane
		*N_Pbase[4],
		*N_1Player_Pos_00, *N_2Player_Pos[2], *N_3Player_Pos[3],
		*N_Bg_00, *N_Bg_00_vs,
		*N_corse_Pos_00[10], *N_corse_Pos_01[10],
		*N_title_00, *N_scissor_00, *N_cursorSize_00;
	nw4r::lyt::TextBox
		*T_titleMulti_00, *T_titleMulti_01,
		*T_rankStar_00, *T_rankStar_01,
		*T_rankStar_02, *T_rankStar_03,
		*T_guideViewC_00;
	nw4r::lyt::Pane
		*W_title_00, *W_Base_00, *W_Base_00_vs,
		*W_Base_01, *W_Base_01_vs;

	u32 pageChangeDirection;

	struct LevelData {
		int worldNum, levelNum;
		u16 unk, pad;
		int unk2;
	};
	LevelData favorites[10];

	float N_Pbase_transX[4], _4F0;
	int currentSelection, previousSelection, _4FC;
	int currentPage; // 0 = recommended, 10 = favorites
	int unlockedWorldCount, _508;
	bool layoutLoaded, _50D, isSuccession, _50F, _510, _511;
	bool contentsResLoaded, _513, _514, worldsUnlocked[11];
	bool selectingWorld, pad[3];

	void getLevelID(u8 *world, u8 *level, int selection);
	void setButtonLevelID(int button, int maxButtonCount);

	void loadTitleFromLI(int tb, int type);
	void setupTitle(int tb);
	void colorTitleText(int tb, int type);
};