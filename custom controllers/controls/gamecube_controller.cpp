#include <controls/custom_controllers.h>
#include <controls/dWarningGameCube_c.h>

// dRemoconMng_c stuff

CREATE_STATE(dRemoconMng_c::dConnect_c::dExtension_c, GameCube);

void dRemoconMng_c::dConnect_c::dExtension_c::beginState_GameCube() {
    OSReport("gamecube controller detected in port %d!\n", controllerID+1);
    extensionType = ControllerType::GameCube;
    dCustomControllerInfo *cont = getCustomController(controllerID);
    cont->flags |= CCFLAG_HAS_CUSTOM;
    cont->flags |= CCFLAG_NO_ACC;
    cont->flags |= CCFLAG_NO_WIIMOTE;
    cont->controllerType = ControllerType::GameCube;
    return;
}

void dRemoconMng_c::dConnect_c::dExtension_c::executeState_GameCube() {
    u32 extension = *&(CoreControllers[controllerID]->status).dev_type;
    if (extension != WPAD_DEV_GAMECUBE) {
        dCustomController_c::changeRemoconMgrState(this, extension);
    }
    return;
}

void dRemoconMng_c::dConnect_c::dExtension_c::endState_GameCube() { }

// controller mappings

void dCustomController_c::mapGameCubeButtons(EGG::CoreController *controller, int controllerID) {
    KPADStatus *kStatus = &controller->status;
    dCustomControllerInfo *customController = getCustomController(controllerID);
    dPADInfo *gStatus = &customController->padInfo;

#ifdef DEBUG_INPUTS
    OSReport("GameCube: %08x %08x %08x\n", gStatus->hold, gStatus->trig, gStatus->release);
    OSReport("GameCube L: %f %f, %f R: %f %f, %f\n", gStatus->stick.x, gStatus->stick.y, gStatus->ltrigger, gStatus->substick.x, gStatus->substick.y, gStatus->rtrigger);
#endif

    clearWiimotedata(kStatus, false);

    // convert to sideways Wii Remote buttons
    if (gStatus->hold & PAD_BUTTON_LEFT) {
        kStatus->hold |= WPAD_BUTTON_UP;
    }
    if (gStatus->hold & PAD_BUTTON_RIGHT) {
        kStatus->hold |= WPAD_BUTTON_DOWN;
    }
    if (gStatus->hold & PAD_BUTTON_DOWN) {
        kStatus->hold |= WPAD_BUTTON_LEFT;
    }
    if (gStatus->hold & PAD_BUTTON_UP) {
        kStatus->hold |= WPAD_BUTTON_RIGHT;
    }

    if (gStatus->hold & PAD_BUTTON_A) {
        kStatus->hold |= WPAD_BUTTON_2;
    }
    if (gStatus->hold & PAD_BUTTON_B) {
        kStatus->hold |= WPAD_BUTTON_1;
    }
    if (gStatus->hold & PAD_BUTTON_Y) {
        kStatus->hold |= WPAD_BUTTON_C;
    }
    if (gStatus->hold & PAD_BUTTON_START) {
        kStatus->hold |= WPAD_BUTTON_PLUS;
    }
    if (gStatus->hold & PAD_TRIGGER_Z) {
        kStatus->hold |= WPAD_BUTTON_MINUS;
        //shake the damn thing
        dCustomController_c::shakeController(controllerID);
    }

    // map stick to D-pad
    if (gStatus->stick.x < -0.5 && !(kStatus->hold & WPAD_BUTTON_DOWN)) {
        kStatus->hold |= WPAD_BUTTON_UP;
    }
    if (gStatus->stick.x > 0.5 && !(kStatus->hold & WPAD_BUTTON_UP)) {
        kStatus->hold |= WPAD_BUTTON_DOWN;
    }
    if (gStatus->stick.y < -0.5 && !(kStatus->hold & WPAD_BUTTON_RIGHT)) {
        kStatus->hold |= WPAD_BUTTON_LEFT;
    }
    if (gStatus->stick.y > 0.5 && !(kStatus->hold & WPAD_BUTTON_LEFT)) {
        kStatus->hold |= WPAD_BUTTON_RIGHT;
    }

    kStatus->trigger = kStatus->hold & ~customController->lastPressed;
    kStatus->release = customController->lastPressed & ~kStatus->hold;
    kStatus->wpad_err = 0;

    customController->lastPressed = kStatus->hold;

    // for in-game but not for use in menus
    if (gStatus->hold & PAD_BUTTON_Y) {
        kStatus->hold |= WPAD_BUTTON_1;
    }
    if (gStatus->hold & PAD_BUTTON_X) {
        kStatus->hold |= WPAD_BUTTON_A;
    }

    // default tilt behavior
    // some actors are programmed to not use this value directly!
    float tilt = gStatus->ltrigger;
    tilt -= gStatus->rtrigger;

    kStatus->acc.z = tilt;
    kStatus->acc_vertical.y = -tilt;
}

// handle gc controller connection

extern u16 nowScene;
extern "C" s32 WPADProbe(s32 chan, u32 *type);

u32 dCustomController_c::checkForGCConnection(int channel) {
    u32 probe = SIProbe(channel);
    if (probe == SI_ERROR_NO_RESPONSE) {
        // no controller
        controllers[channel].flags |= CCFLAG_HAS_AUTOCONNECT;
        controllers[channel].flags &= ~CCFLAG_NO_AUTOCONNECT;
        return GCCType::GCCNone;
    } else if (probe & SI_GC_STANDARD) {
        if (PadStatus[channel].err != PAD_ERR_NO_CONTROLLER) {
            if (!(controllers[channel].flags & CCFLAG_NO_AUTOCONNECT)) {
                if (nowScene == BOOT && !(controllers[channel].flags & CCFLAG_HAS_AUTOCONNECT) && WPADProbe(channel, NULL) != -1) {
                    // don't detect a GC controller if connected on game boot, unless a button is pressed or there's no wiimote
                    OSReport("GC Controller detected on boot! ignoring for now...\n");
                    // don't connect GC controller on boot
                    controllers[channel].flags |= CCFLAG_NO_AUTOCONNECT;
                    return GCCType::GCCNone;
                }
                return GCCType::GCCStandard;
            } else { // gc is connected on boot, don't connect unless we press a button
                if (controllers[channel].padInfo.hold) {
                    OSReport("Pressing something! connecting the controller\n");
                    controllers[channel].flags |= CCFLAG_HAS_AUTOCONNECT;
                    controllers[channel].flags &= ~CCFLAG_NO_AUTOCONNECT;
                    return GCCType::GCCStandard;
                }
                return GCCType::GCCNone;
            }
        }
        // no controller
        controllers[channel].flags |= CCFLAG_HAS_AUTOCONNECT;
        controllers[channel].flags &= ~CCFLAG_NO_AUTOCONNECT;
        return GCCType::GCCNone;
    } else if (probe != SI_ERROR_BUSY && probe != SI_ERROR_UNKNOWN) {
        OSReport("invalid GC type in port %d\n", channel);
        return GCCType::GCCInvalid;
    }
    return GCCType::GCCNone;
}

// do gc rumble if connected

extern "C" void WPADControlMotor(int channel, int command);

void startMotor(EGG::CoreController *self) {
    if (getCustomController(self->channel)->controllerType == ControllerType::GameCube) {
        PADControlMotor(self->channel, PAD_MOTOR_RUMBLE);
    } else {
        WPADControlMotor(self->channel, 1);
    }
}

void stopMotor(EGG::CoreController *self) {
    if (getCustomController(self->channel)->controllerType == ControllerType::GameCube) {
        PADControlMotor(self->channel, PAD_MOTOR_STOP);
    } else {
        WPADControlMotor(self->channel, 0);
    }
}

// disable speaker if gc connected

extern "C" bool isForbidRemoteSE();

u32 remote_players[4] = {0x2, 0x4, 0x8, 0x10};

u32 getRemotePlayer(u32 playerID) {
    if (isForbidRemoteSE() || getCustomController(playerID)->flags & CCFLAG_NO_WIIMOTE) {
        return 0;
    } else {
        return remote_players[playerID];
    }
}

// always report full battery with gc

extern "C" s32 getBatteryLevels(int channel);

u32 accountForGCBattery(int channel) {
    if (getCustomController(channel)->flags & CCFLAG_NO_WIIMOTE) {
        // if a Wii remote is not connected, always return full battery
        if (getBatteryLevels(channel) < 0)
            return 4;
    }
    return getBatteryLevels(channel);
}

// read & reset gc inputs

void EGG::CoreControllerMgr::readPad() {
    // read gc inputs & clamp sticks
    PADRead(PadStatus);
    PADClamp(PadStatus);
    // set padInfo
    dCustomController_c::setPadInfo();
    return beginFrame();
}

void EGG::CoreControllerMgr::resetPad() {
    // call original function
    endFrame();

    u32 resetBits = 0;
    u32 padBit;
    for (int i = 0; i < 4; i++) {
        padBit = (PAD_CHAN0_BIT >> i);
        if (PadStatus[i].err == PAD_ERR_NO_CONTROLLER) {
            resetBits |= padBit;
        }

        // quick stupid fix to when this is checked later in remoconmg
        // it's hacky, but it works
        *(int *)(devTypePtr + i) = CoreControllers[i]->status.dev_type;
    }
    if (resetBits) {
        PADReset(resetBits);
    }
    return;
}

// controller screen

dWarningGameCube_c::dWarningGameCube_c() : layout(), acState(this, &StateID_StartWait) {
	layoutLoaded = 0;
	RemNumber = 0;
}

dWarningGameCube_c::~dWarningGameCube_c() {
	delete &layout;
	delete &acState;
}

CREATE_STATE(dWarningGameCube_c, StartWait);
CREATE_STATE(dWarningGameCube_c, OpenAnimeEndWait);
CREATE_STATE(dWarningGameCube_c, ExitWait);
CREATE_STATE(dWarningGameCube_c, ExitAnimeEndWait);

int dWarningGameCube_c::createLayout() {
	if(!layoutLoaded) {
		if (!layout.loadArc("extensionControllerGamecube.arc", false))
			return false;

		static const char *brlanNames[3] = {
			"extensionControllerGamecube_inWindow.brlan",
			"extensionControllerGamecube_loopWindow.brlan",
			"extensionControllerGamecube_outWindow.brlan"
		};

		static const char *groupNames[3] = {
			"A00_Window",
			"A00_Window",
			"A00_Window"
		};

		static const int groupIDs[3] = {
			0,
			1,
			2
		};

		layout.build("extensionControllerGamecube.brlyt");

		layout.loadAnimations(brlanNames, 3);
		layout.loadGroups(groupNames, groupIDs, 3);
		// layout.disableAllAnimations();

		rootPane = layout.getRootPane();

		T_player = layout.findTextBoxByName("T_player");
		W_extensionGC = layout.findPaneByName("W_extensionGC");

		static const char *bmgTextboxesNames[6] = {
            "T_gamecubeS_00",
            "T_gamecube_01",
			"T_guide_01",
			"T_guide_02",
			"T_guide_04",
			"T_guide_05"
		};

		static const int bmgTextboxesIDs[6] = {
			0x12,
			0x12,
			0x14,
			0x15,
			0x17,
			0x18
		};

		layout.setLangStrings(bmgTextboxesNames, bmgTextboxesIDs, 0x68, 6);

		layout.resetAnim(0);
		
		layout.update();
		layout.execAnimations();

		layout.disableAllAnimations();

		layout.drawOrder = 0x97;

		rootPane->flag &= 0xFE;

		isVisible = false;
		_1E7 = false;
		layoutLoaded = true;

		OSReport("Gamecube loaded!\n");
	}

	return true;
}

bool dWarningGameCube_c::doDelete() {
	return layout.free();
}

void dWarningGameCube_c::execute() {
	if(isVisible) {
		acState.execute();
		layout.execAnimations();
		layout.update();
	}
}

void dWarningGameCube_c::draw() {
	if(isVisible) {
		layout.scheduleForDrawing();
	}
}

void dWarningGameCube_c::setPlayerNo(u32 num) {
	RemNumber = num;
	isVisible = 1;
	_1E6 = 1;
}

void dWarningGameCube_c::WindowCloseSetup() {
	_1E6 = 1;
	_1E7 = 1;
}

void dWarningGameCube_c::beginState_StartWait() {
	rootPane->flag &= 0xFE;
}

void dWarningGameCube_c::executeState_StartWait() {
	WriteBMGToTextBox(T_player, GetBMG(), 0x68, RemNumber+4, 0);
	acState.setState(&StateID_OpenAnimeEndWait);
}

void dWarningGameCube_c::endState_StartWait() { }

void dWarningGameCube_c::beginState_OpenAnimeEndWait() {
	_1E6 = 1;
	rootPane->flag &= 0xFE;
	rootPane->flag |= 1;
	layout.enableNonLoopAnim(0);
}

void dWarningGameCube_c::executeState_OpenAnimeEndWait() {
	if(!layout.isAnimOn(0)) {
		acState.setState(&StateID_ExitWait);
	}
}

void dWarningGameCube_c::endState_OpenAnimeEndWait() {
	_1E6 = 0;
}

void dWarningGameCube_c::beginState_ExitWait() {
	_1E7 = 0;
	layout.enableNonLoopAnim(1);
}

void dWarningGameCube_c::executeState_ExitWait() {
	if(_1E7) {
		_1E7 = 0;
		acState.setState(&StateID_ExitAnimeEndWait);
	}
}

void dWarningGameCube_c::endState_ExitWait() {
	layout.disableAnim(1);
}

void dWarningGameCube_c::beginState_ExitAnimeEndWait() {
	_1E6 = 1;
	layout.enableNonLoopAnim(2);
}

void dWarningGameCube_c::executeState_ExitAnimeEndWait() {
	if(!layout.isAnimOn(2)) {
		acState.setState(&StateID_StartWait);
	}
}

void dWarningGameCube_c::endState_ExitAnimeEndWait() {
	isVisible = 0;
	_1E6 = 0;
}