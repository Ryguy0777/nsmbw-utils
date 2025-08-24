#include <game.h>
#include <utils.h>
#include "levelinfo.h"

extern char CurrentWorld;
extern char CurrentLevel;

enum GameFlag {
    DisableActorSpawning = 1,
    IsStopped = 2, // enables player state change freeze
    LevelLoadedMaybe = 4,
    ExtraMode = 0x10,
    GameOver = 0x20,
    CoinBattle = 0x40,
    };

extern u32 GameFlag;

class Pausewindow_c : public dBase_c {
	public:
		int onCreate();
		int onDelete();
		int onExecute();
		int onDraw();

		Pausewindow_c();
		~Pausewindow_c();

		m2d::EmbedLayout_c layout;
		dStateWrapper_c<Pausewindow_c> state;

		nw4r::lyt::Picture
			*P_SBBase_00, *P_SBBase_02, *P_shadowBlack, *P_coin;

		nw4r::lyt::Pane *W_N_pauseMenu_00; 
		
		nw4r::lyt::TextBox 
			*T_tuzukeru_00, *T_tuzukeru_01, *T_modoru_00, *T_modoru_01,
			*T_worldNum_00, *T_corseNum_00, *T_corsePic_00, *T_osusumeText_00;
		
		nw4r::lyt::Pane
			*N_worldText_00, *N_osusumeText_00, *N_multiText_00;
		
		u32 nextButton, *currentButton, windowColourID;
		u8 layoutLoaded;
		bool isVisible;
		u8 _292, _293, animationIsPlaying, needsToSetButtonStrings;
		u8 unk[2];

		//new func
		void newLoadLevelName();
};

void Pausewindow_c::newLoadLevelName() {
	const wchar_t *convWorldName;
	const wchar_t *convLevelName;

	GameMgr *dInfo_c;
	u32 unk;

	dInfo_c = GameMgrP;
	unk = GameMgrP->_3E8;

	if ((int)unk > 9) {
    	unk = unk - 10;
	}

	int wnum = (int)CurrentWorld;
	int lnum = (int)CurrentLevel;

	N_osusumeText_00->SetVisible(false);
	N_multiText_00->SetVisible(false);

	if ((((GameFlag & CoinBattle) == 0) || (dInfo_c->_3E4 != 0)) || ((int)unk > 4)) {
		P_coin->SetVisible(false);
		T_worldNum_00->SetVisible(true);
	} else {
		P_coin->SetVisible(true);
		T_worldNum_00->SetVisible(false);
	}
	
	dLevelInfo_c::entry_s *level = dLevelInfo_c::s_info.searchBySlot(wnum, lnum);
	if (level) {
		convWorldName = getWorldNumber(level->displayWorld);
		convLevelName = getLevelNumber(level->displayLevel);

		T_worldNum_00->SetString(convWorldName);

		if (level->displayLevel > 19) {
			T_corsePic_00->SetVisible(true);
			T_corseNum_00->SetVisible(false);
			T_corsePic_00->SetString(convLevelName);
		} else {
			T_corsePic_00->SetVisible(false);
			T_corseNum_00->SetVisible(true);
			T_corseNum_00->SetString(convLevelName);
		}
	} else {
		T_corsePic_00->SetVisible(false);
		T_corseNum_00->SetVisible(true);
		T_worldNum_00->SetString(L"?");
		T_corseNum_00->SetString(L"?");
	}
}