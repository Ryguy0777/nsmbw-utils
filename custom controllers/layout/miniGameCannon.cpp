#include <game.h>
#include <controls/custom_controllers.h>

// Size: 0x2E8
class dMiniGameCannon_c : public dBase_c {
public:
	m2d::EmbedLayout_c layout;					// 0x70
	dStateWrapper_c<dMiniGameCannon_c> state;	// 0x208

	nw4r::lyt::Pane *rootPane;			// 0x244
	nw4r::lyt::Pane *N_4P_Pos_0X[4];	// 0x248
	nw4r::lyt::Pane *N_3P_Pos_0X[3];	// 0x258
	nw4r::lyt::Pane *N_2P_Pos_00[2];	// 0x264
	nw4r::lyt::Pane *N_1P_Pos_00;		// 0x26C
	nw4r::lyt::Pane *N_info_00;			// 0x270
	nw4r::lyt::Pane *N_result_00;		// 0x274
	nw4r::lyt::Pane *N_gameCannon_00;	// 0x278
	nw4r::lyt::Pane *N_result_01;		// 0x27C
	nw4r::lyt::Pane *N_XP_00[4];		// 0x280
	nw4r::lyt::TextBox *T_XUp_00[4];	// 0x290
	nw4r::lyt::TextBox *T_info_04;		// 0x2A0
	nw4r::lyt::Picture *P_XP_00[4];		// 0x2A4
	nw4r::lyt::Picture *P_BG_00;		// 0x2B4
	nw4r::lyt::Picture *P_titleBase_00;	// 0x2B8
	nw4r::lyt::Pane *W_gameCannon_00;	// 0x2BC

	int  currentState;		// 0x2C0 -- 0 = first open with play info, 1 = open for results
	int  playerLives[4];	// 0x2C4
	int  playerCount;		// 0x2D4
	bool layoutLoaded;		// 0x2DA
	bool visible;			// 0x2DB
	bool willOpen;			// 0x2DA
	bool playerActive[4];	// 0x2DB
	bool _2DF;				// 0x2DF -- unused? set to false in finalizeState_TitleExitAnimeEndWait()
	bool willOpenTitle;		// 0x2E0
	bool animationActive;	// 0x2E1
	bool willExitTitle;		// 0x2E2
	bool willExitMain;		// 0x2E3
	bool willExitResult;	// 0x2E4

	// formerly padding bytes
	u16 timer;		// 0x2E5
	u8  fadeState;	// 0x2E7

	void updatePrompts();

	void finalizeState_StartWait();
	void executeState_NowDisp();

	USING_STATES(dMiniGameCannon_c);
	REF_NINTENDO_STATE(ExitAnimeEndWait); // 0x80993000
};

void dMiniGameCannon_c::finalizeState_StartWait() {
	this->playerCount = 0;
	for (int i = 0; i < 4; i++) {
		if (QueryPlayerAvailability(i)) {
			this->playerActive[i] = true;
			this->playerCount++;
		} else {
			this->playerActive[i] = false;
		}
	}

	// force wiimote icon just in case of a different controller type
	this->fadeState = 0;
	this->updatePrompts();
}

void dMiniGameCannon_c::updatePrompts() {
	int messageIDs[4] = {0x01, 0x01, 0x08, 0x09};
	nw4r::lyt::TextBox *T_info_01 = layout.findTextBoxByName("T_info_01");
	WriteBMGToTextBox(T_info_01, GetBMG(), 0x12C, messageIDs[fadeState], 0);

	int controllerIDs[4] = {ControllerType::Wiimote, ControllerType::Nunchuck, ControllerType::Classic, ControllerType::GameCube};
	GameMgrP->currentControllerType = controllerIDs[fadeState];
	nw4r::lyt::TextBox *T_info_02 = layout.findTextBoxByName("T_info_02");
	WriteBMGToTextBox(T_info_02, GetBMG(), 0x12C, 0x02, 0);
	GameMgrP->currentControllerType = GameKeyInstance->remocons[0]->controllerType;
}

void dMiniGameCannon_c::executeState_NowDisp() {
	// left panel
	nw4r::lyt::Pane *N_fukidashi_00 = layout.findPaneByName("N_fukidashi_00");
	nw4r::lyt::Picture *P_classic_00 = layout.findPictureByName("P_classic_00");
	nw4r::lyt::Picture *P_gamecube_00 = layout.findPictureByName("P_gamecube_00");

	// right panel
	nw4r::lyt::Picture *P_yoko_01 = layout.findPictureByName("P_yoko_01");
	nw4r::lyt::Picture *P_nunchuk_01 = layout.findPictureByName("P_nunchuk_01");
	nw4r::lyt::Picture *P_classic_01 = layout.findPictureByName("P_classic_01");
	nw4r::lyt::Picture *P_gamecube_01 = layout.findPictureByName("P_gamecube_01");

	if (timer == 0) { // wiimote time
		fadeState = 0;
	} else if (timer == 240) { // nunchuk time
		fadeState = 1;
	} else if (timer == 480) { // classic time
		fadeState = 2;
	} else if (timer == 720) { // gamecube time
		fadeState = 3;
	}

	// now fade between controllers
	if (fadeState == 1) {
		if (P_yoko_01->alpha != 0) {
			P_yoko_01->alpha -= 17;

			P_nunchuk_01->alpha += 17;
		}
	} else if (fadeState == 2) {
		if (N_fukidashi_00->alpha != 0) {
			N_fukidashi_00->alpha -= 17;
			P_nunchuk_01->alpha -= 17;

			P_classic_00->alpha += 17;
			P_classic_01->alpha += 17;
		}
	} else if (fadeState == 3) {
		if (P_classic_00->alpha != 0) {
			P_classic_00->alpha -= 17;
			P_classic_01->alpha -= 17;

			P_gamecube_00->alpha += 17;
			P_gamecube_01->alpha += 17;
		}
	}

	timer++;
	this->updatePrompts();

	if (this->willExitMain) {
		timer = 0;
		state.setState(&StateID_ExitAnimeEndWait);
	}
}
