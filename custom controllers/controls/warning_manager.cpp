#include <controls/dWarningManager_c.h>
#include <sfx.h>

int dWarningManager_c::newCreate() {
	int ret;
	if (WarningOther.createLayout() == 0 || WarningNunchuk.createLayout() == 0 ||
		WarningYoKo.createLayout() == 0 || WarningBattery.createLayout() == 0 ||
		WarningErrorInfo.createLayout() == 0 || WarningClassic.createLayout() == 0 || WarningGameCube.createLayout() == 0) {
		ret = 0;
	} else {
		_B64 = 0;
		_B68 = 5;
		_B6C = 5;
		_B70 = 5;
		_B74 = 0;
		WarningMgrCreated = true;
		isBatteryActive = 0;
		isSaveErrorActive = 0;
		_B50 = 0;
		_B90 = 0;
		if (RemoconMngInstance) {
			lastWiimoteType[0] = RemoconMngInstance->wiimotes[0]->extension.extensionType;
			lastWiimoteType[1] = RemoconMngInstance->wiimotes[1]->extension.extensionType;
			lastWiimoteType[2] = RemoconMngInstance->wiimotes[2]->extension.extensionType;
			lastWiimoteType[3] = RemoconMngInstance->wiimotes[3]->extension.extensionType;
		}
		maybe_not_array[0] = 4;
		ret = 1;
		maybe_not_array[1] = 4;
		maybe_not_array[2] = 4;
		maybe_not_array[3] = 4;
		_B74 = 0;
		_B78 = 0;
	}
	return ret;
}

int dWarningManager_c::newDelete() {
	bool ret;
	if (WarningOther.doDelete() == 0 || WarningNunchuk.doDelete() == 0 ||
		WarningYoKo.doDelete() == 0 || WarningBattery.doDelete() == 0 ||
		WarningClassic.doDelete() == 0 || WarningGameCube.doDelete() == 0) {
		ret = false;
	} else {
		ret = true;
	}
	return ret;
}

int dWarningManager_c::newDraw() {
	if (_B68 != 5) {
		WarningOther.draw();
		WarningNunchuk.draw();
		WarningYoKo.draw();
		WarningBattery.draw();
		WarningErrorInfo.draw();
		WarningClassic.draw();
		WarningGameCube.draw();
	}
	return true;
}

void dWarningManager_c::newAllWarningEnd(bool keepForbid) {
	_B74 = 0;
	_B78 = 0;
	WarningMgrCheck = false;
	if (!keepForbid) {
		WarningMgrForbid = 0;
	}
	isBatteryActive = 0;
	isErrorActive = 0;
	_B90 = 0;
	_B68 = 5;
	_B6C = 5;
	_B70 = 5;
	maybe_not_array[0] = 4;
	maybe_not_array[1] = 4;
	maybe_not_array[2] = 4;
	maybe_not_array[3] = 4;
	if (WarningOther.isVisible) {
		WarningOther.isVisible = false;
		WarningOther.acState.setState(&WarningOther.StateID_StartWait);
	}
	if (WarningNunchuk.isVisible) {
		WarningNunchuk.isVisible = false;
		WarningNunchuk.acState.setState(&WarningNunchuk.StateID_StartWait);
	}
	if (WarningYoKo.isVisible) {
		WarningYoKo.isVisible = false;
		WarningYoKo.acState.setState(&WarningYoKo.StateID_StartWait);
	}
	if (WarningBattery.isVisible) {
		WarningBattery.isVisible = false;
		WarningBattery._28A = 1;
	}
	if (WarningErrorInfo.isVisible) {
		WarningErrorInfo.isVisible = false;
		WarningErrorInfo._22E = 1;
		WarningErrorInfo.execute();
	}
	if (WarningClassic.isVisible) {
		WarningClassic.isVisible = false;
		WarningClassic.acState.setState(&WarningClassic.StateID_StartWait);
	}
	if (WarningGameCube.isVisible) {
		WarningGameCube.isVisible = false;
		WarningGameCube.acState.setState(&WarningGameCube.StateID_StartWait);
	}
	isSaveErrorActive = 0;
	_B50 = 0;
	acState.setState(&StateID_WarningCheck);
}

extern u16 nowScene;
extern void *SomeWipeClass;
extern "C" u32 getFaderStatus(void *); // 8016de50
void dWarningManager_c::newExecuteWarningCheck() {
	u32 controllerNum = 0;
	if ((nowScene != BOOT) && (getFaderStatus(SomeWipeClass) == 1) && (WarningMgrForbid == 0) && (_B90 == 0)) {
		do {
			u32 extension = RemoconMngInstance->wiimotes[controllerNum]->extension.extensionType;
			if (GameMgrP->CharIDs[controllerNum] > 0) {
				if (lastWiimoteType[controllerNum] == ControllerType::None) {
					lastWiimoteType[controllerNum] = extension;
				} else if (((extension != lastWiimoteType[controllerNum]) || (extension == ControllerType::Invalid)) && (extension != ControllerType::None)) {
					_B8C = 1;
					_B64 = controllerNum;
					if (WarningMgrCheck)
						MapSoundPlayer(SoundRelatedClass, SE_SYS_DIALOGUE_IN, 1);
					WarningMgrCheck = true;
					if (extension == ControllerType::Wiimote) {
						lastWiimoteType[controllerNum] = ControllerType::Wiimote;
						YoKoDispSetup(controllerNum);
						_B68 = 2;
						return;
					}
					if (extension == ControllerType::Nunchuck) {
						lastWiimoteType[controllerNum] = ControllerType::Nunchuck;
						NunchukDispSetup(controllerNum);
						_B68 = 1;
						return;
					}
					if (extension == ControllerType::Invalid) {
						lastWiimoteType[controllerNum] = ControllerType::Invalid;
						newOtherDispSetup(controllerNum);
						_B68 = 1;
						return;
					}
					if (extension == ControllerType::Classic) {
						lastWiimoteType[controllerNum] = ControllerType::Classic;
						ClassicDispSetup(controllerNum);
						_B68 = 1;
						return;
					}
					if (extension == ControllerType::GameCube) {
						lastWiimoteType[controllerNum] = ControllerType::GameCube;
						GameCubeDispSetup(controllerNum);
						_B68 = 1;
						return;
					}
				}
			}
			controllerNum++;
		} while (controllerNum < 4);
		WarningMgrCheck = false;
	}
}

void dWarningManager_c::newExecuteExitAnimeEndWait() {
	if (ForbidCheck() == 0) {
		if (WarningOther._1E6 == 0) {
			if (WarningNunchuk._1E6 == 0) {
				if (WarningYoKo._1E6 == 0) {
					if (WarningClassic._1E6 == 0) {
						if (WarningGameCube._1E6 == 0) {
							_B68 = 5;
							acState.setState(&StateID_WarningCheck);
						} else {
							WarningGameCube.execute();
						}
					} else {
						WarningClassic.execute();
					}
				} else {
					WarningYoKo.execute();
				}
			} else {
				WarningNunchuk.execute();
			}
		} else {
			WarningOther.execute();
		}
	}
}

extern "C" void assignSomeColoursToWindow(nw4r::lyt::Pane *window, u32 charID);

void dWarningManager_c::newOtherDispSetup(u32 player) {
	for(int i = 0; i < 4; i++) {
		CoreControllers[RemoconMngInstance->wiimotes[i]->controllerID]->stopRumbleMgr();
	}

	WarningOther.setPlayerNo(player);

	u32 charID = Player_ID[player];
	assignSomeColoursToWindow(WarningOther.W_extensionCla, charID);

	// here we change the message shown
	// depending on if the invalid controller type is a wii or gc device
	int msgId = 0x03;
	if (CoreControllers[RemoconMngInstance->wiimotes[player]->controllerID]->status.dev_type == WPAD_DEV_GCN_FUTURE) {
		msgId = 0x1a;
	}
	dScript::Res_c *msgRes = GetBMG();
	nw4r::lyt::TextBox *T_clasic_S_00 = WarningOther.layout.findTextBoxByName("T_clasic_S_00");
	nw4r::lyt::TextBox *T_clasic_00 = WarningOther.layout.findTextBoxByName("T_clasic_00");
	WriteBMGToTextBox(T_clasic_S_00, msgRes, 0x68, msgId, 0);
	WriteBMGToTextBox(T_clasic_00, msgRes, 0x68, msgId, 0);

	acState.setState(&StateID_WarningCaution);
}

CREATE_STATE_E(dWarningManager_c, WarningClassic);
CREATE_STATE_E(dWarningManager_c, WarningGameCube);

void dWarningManager_c::ClassicDispSetup(u32 player) {
	for(int i = 0; i < 4; i++) {
		CoreControllers[RemoconMngInstance->wiimotes[i]->controllerID]->stopRumbleMgr();
	}

	WarningClassic.setPlayerNo(player);

	u32 charID = Player_ID[player];
	assignSomeColoursToWindow(WarningClassic.W_extensionCC, charID);

	acState.setState(&StateID_WarningClassic);
}

void dWarningManager_c::executeState_WarningClassic() {
	if(!ForbidCheck()) {
		WarningClassic.execute();
		if(!WarningClassic._1E6) {
			_B8C = 0;
			if(HitAnyKey()) {
				GotoWarningCheck();
				WarningClassic.WindowCloseSetup();
			}
		}
	}
}

void dWarningManager_c::GameCubeDispSetup(u32 player) {
	for(int i = 0; i < 4; i++) {
		CoreControllers[RemoconMngInstance->wiimotes[i]->controllerID]->stopRumbleMgr();
	}

	WarningGameCube.setPlayerNo(player);

	u32 charID = Player_ID[player];
	assignSomeColoursToWindow(WarningGameCube.W_extensionGC, charID);

	acState.setState(&StateID_WarningGameCube);
}

void dWarningManager_c::executeState_WarningGameCube() {
	if(!ForbidCheck()) {
		WarningGameCube.execute();
		if(!WarningGameCube._1E6) {
			_B8C = 0;
			if(HitAnyKey()) {
				GotoWarningCheck();
				WarningGameCube.WindowCloseSetup();
			}
		}
	}
}