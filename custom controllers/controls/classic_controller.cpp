#include <controls/custom_controllers.h>
#include <controls/dWarningClassic_c.h>

// dRemoconMng_c stuff

extern u32 __rvl_p_wpadcb;

CREATE_STATE(dRemoconMng_c::dConnect_c::dExtension_c, Classic);

void dRemoconMng_c::dConnect_c::dExtension_c::beginState_Classic() {
    OSReport("classic controller detected in port %d!\n", controllerID+1);
    extensionType = ControllerType::Classic;
    dCustomControllerInfo *cont = getCustomController(controllerID);
    cont->flags |= CCFLAG_HAS_CUSTOM;
    cont->flags |= CCFLAG_NO_ACC;
    cont->flags &= ~CCFLAG_NO_WIIMOTE;
    cont->controllerType = ControllerType::Classic;
    
    // check for pro controller
    int wpadData = (&__rvl_p_wpadcb)[controllerID];
    u8 subType = **(u8 **)(wpadData + 0x974);
    OSReport("subType: %d\n", subType);
    // idk why it works like this here and not in ____wpadGetExtType
    if (subType == 0) {
        cont->flags |= CCFLAG_IS_PROCON;
    } else {
        cont->flags &= ~CCFLAG_IS_PROCON;
    }
    return;
}

void dRemoconMng_c::dConnect_c::dExtension_c::executeState_Classic() {
    u32 extension = *&(CoreControllers[controllerID]->status).dev_type;
    if (extension != WPAD_DEV_CLASSIC) {
        dCustomController_c::changeRemoconMgrState(this, extension);
    }
    return;
}

void dRemoconMng_c::dConnect_c::dExtension_c::endState_Classic() { }

// controller mappings

void dCustomController_c::mapClassicButtons(EGG::CoreController *controller, int controllerID) {
    KPADStatus *kStatus = &controller->status;
    KPADEXStatus_cl *cStatus = &controller->status.ex_status.cl;
    dCustomControllerInfo *customController = getCustomController(controllerID);

#ifdef DEBUG_INPUTS
    OSReport("Classic: %08x %08x %08x\n", cStatus->hold, cStatus->trig, cStatus->release);
    OSReport("Classic L: %f %f, %f R: %f %f, %f\n", cStatus->lstick.x, cStatus->lstick.y, cStatus->ltrigger, cStatus->rstick.x, cStatus->rstick.y, cStatus->rtrigger);
#endif

    clearWiimotedata(kStatus, true);

    // convert to sideways Wii Remote buttons
    if (cStatus->hold & WPAD_CL_BUTTON_LEFT) {
        kStatus->hold |= WPAD_BUTTON_UP;
    }
    if (cStatus->hold & WPAD_CL_BUTTON_RIGHT) {
        kStatus->hold |= WPAD_BUTTON_DOWN;
    }
    if (cStatus->hold & WPAD_CL_BUTTON_DOWN) {
        kStatus->hold |= WPAD_BUTTON_LEFT;
    }
    if (cStatus->hold & WPAD_CL_BUTTON_UP) {
        kStatus->hold |= WPAD_BUTTON_RIGHT;
    }

    if (cStatus->hold & WPAD_CL_BUTTON_A) {
        kStatus->hold |= WPAD_BUTTON_2;
    }
    if (cStatus->hold & WPAD_CL_BUTTON_B) {
        kStatus->hold |= WPAD_BUTTON_1;
    }
    if (cStatus->hold & WPAD_CL_BUTTON_Y) {
        kStatus->hold |= WPAD_BUTTON_C;
    }
    if (cStatus->hold & WPAD_CL_BUTTON_PLUS) {
        kStatus->hold |= WPAD_BUTTON_PLUS;
    }
    if (cStatus->hold & WPAD_CL_BUTTON_MINUS) {
        kStatus->hold |= WPAD_BUTTON_MINUS;
    }
    if (cStatus->hold & WPAD_CL_TRIGGER_ZL || cStatus->hold & WPAD_CL_TRIGGER_ZR) {
        //shake the damn thing
        dCustomController_c::shakeController(controllerID);
    }
    if (customController->flags & CCFLAG_IS_PROCON) {
        // only use L+R for shake if pro controller/SNES mini controller
        if (cStatus->hold & WPAD_CL_TRIGGER_L && cStatus->hold & WPAD_CL_TRIGGER_R) {
            //shake the damn thing
            dCustomController_c::shakeController(controllerID);
        }
    }
    if (cStatus->hold & WPAD_CL_BUTTON_HOME) {
        kStatus->hold |= WPAD_BUTTON_HOME;
    }

    // map stick to D-pad
    if (cStatus->lstick.x < -0.32 && !(kStatus->hold & WPAD_BUTTON_DOWN)) {
        kStatus->hold |= WPAD_BUTTON_UP;
    }
    if (cStatus->lstick.x > 0.32 && !(kStatus->hold & WPAD_BUTTON_UP)) {
        kStatus->hold |= WPAD_BUTTON_DOWN;
    }
    if (cStatus->lstick.y < -0.32 && !(kStatus->hold & WPAD_BUTTON_RIGHT)) {
        kStatus->hold |= WPAD_BUTTON_LEFT;
    }
    if (cStatus->lstick.y > 0.32 && !(kStatus->hold & WPAD_BUTTON_LEFT)) {
        kStatus->hold |= WPAD_BUTTON_RIGHT;
    }

    kStatus->trigger = kStatus->hold & ~customController->lastPressed;
    kStatus->release = customController->lastPressed & ~kStatus->hold;
    kStatus->wpad_err = 0;

    customController->lastPressed = kStatus->hold;

    // for in-game but not for use in menus
    if (cStatus->hold & WPAD_CL_BUTTON_B) {
        kStatus->hold |= WPAD_BUTTON_2;
    }
    if (cStatus->hold & WPAD_CL_BUTTON_Y) {
        kStatus->hold |= WPAD_BUTTON_1;
    }
    if (cStatus->hold & WPAD_CL_BUTTON_X) {
        kStatus->hold |= WPAD_BUTTON_1;
    }
    if (cStatus->hold & WPAD_CL_BUTTON_MINUS) {
        kStatus->hold |= WPAD_BUTTON_A;
    }

    // default tilt behavior
    // some actors are programmed to not use this value directly!
    float tilt = cStatus->ltrigger;
    tilt -= cStatus->rtrigger;

    kStatus->acc.z = tilt;
    kStatus->acc_vertical.y = -tilt;
}

// map home menu pointer to classic controller

static const f32 stickMoveCoefficent = 2048.0f/72.0f;

void mapStickToPosHBM(int channel, Vec2 *pos) {
    KPADEXStatus_cl *cStatus = &CoreControllers[channel]->status.ex_status.cl;
    Vec2 stick;
    stick.x = cStatus->lstick.x / stickMoveCoefficent;
    stick.y = cStatus->lstick.y / stickMoveCoefficent;
    // don't let cursor go off screen edge
    pos->x = clamp<float>(pos->x + stick.x, -1.0, 1.0);
    pos->y = clamp<float>(pos->y - stick.y, -1.0, 1.0);
}

// fix bugs with player buttons

// replaces 8005e590
u16 dAcPyKey_c::newTriggerOne() {
    KPADEXStatus_cl *cStatus = &CoreControllers[playerID]->status.ex_status.cl;
    switch (GameKeyInstance->remocons[playerID]->controllerType) {
        case ControllerType::Classic:
            if (flags & 0x181) return 0; // player is in demo state
            return cStatus->trig & (WPAD_CL_BUTTON_Y | WPAD_CL_BUTTON_X);
        case ControllerType::Nunchuck:
            return nowPressed & WPAD_BUTTON_B;
        default:
            return nowPressed & WPAD_BUTTON_1;
    }
}

// replaces 8005e5d0
u16 dAcPyKey_c::newButtonOne() {
    KPADEXStatus_cl *cStatus = &CoreControllers[playerID]->status.ex_status.cl;
    switch (GameKeyInstance->remocons[playerID]->controllerType) {
        case ControllerType::Classic:
            if (flags & 0x181) return 0; // player is in demo state
            return cStatus->hold & (WPAD_CL_BUTTON_Y | WPAD_CL_BUTTON_X);
        case ControllerType::Nunchuck:
            return heldButtons & WPAD_BUTTON_B;
        default:
            return heldButtons & WPAD_BUTTON_1;
    }
}

// replaces 8005e610
u16 dAcPyKey_c::newButtonDush() {
    KPADEXStatus_cl *cStatus = &CoreControllers[playerID]->status.ex_status.cl;
    switch (GameKeyInstance->remocons[playerID]->controllerType) {
        case ControllerType::Classic:
            if (flags & 0x181) return 0; // player is in demo state
            return cStatus->hold & (WPAD_CL_BUTTON_Y | WPAD_CL_BUTTON_X);
        default:
            return heldButtons & WPAD_BUTTON_1;
    }
}

bool fix2DPlayerJumpButton(int playerID) {
    KPADEXStatus_cl *cStatus = &CoreControllers[playerID]->status.ex_status.cl;
    int nowPressed = GameKeyInstance->remocons[playerID]->newly_pressed;
    switch (GameKeyInstance->remocons[playerID]->controllerType) {
        case ControllerType::Classic:
            return cStatus->trig & WPAD_CL_BUTTON_A;
        default:
            return nowPressed & WPAD_BUTTON_2;
    }
}

// controller screen

dWarningClassic_c::dWarningClassic_c() : layout(), acState(this, &StateID_StartWait) {
	layoutLoaded = 0;
	RemNumber = 0;
}

dWarningClassic_c::~dWarningClassic_c() {
	delete &layout;
	delete &acState;
}

CREATE_STATE(dWarningClassic_c, StartWait);
CREATE_STATE(dWarningClassic_c, OpenAnimeEndWait);
CREATE_STATE(dWarningClassic_c, ExitWait);
CREATE_STATE(dWarningClassic_c, ExitAnimeEndWait);

int dWarningClassic_c::createLayout() {
	if(!layoutLoaded) {
		if (!layout.loadArc("extensionControllerClassic.arc", false))
			return false;

		static const char *brlanNames[3] = {
			"extensionControllerClassic_inWindow.brlan",
			"extensionControllerClassic_loopWindow.brlan",
			"extensionControllerClassic_outWindow.brlan"
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

		layout.build("extensionControllerClassic.brlyt");

		layout.loadAnimations(brlanNames, 3);
		layout.loadGroups(groupNames, groupIDs, 3);
		// layout.disableAllAnimations();

		rootPane = layout.getRootPane();

		T_player = layout.findTextBoxByName("T_player");
		W_extensionCC = layout.findPaneByName("W_extensionCC");

		static const char *bmgTextboxesNames[5] = {
            "T_classicS_00",
            "T_classic_01",
			"T_guide_00",
			"T_guide_01",
			"T_guide_02",
		};

		static const int bmgTextboxesIDs[5] = {
			0xD,
			0xD,
			0xE,
			0xF,
			0x10,
		};

		layout.setLangStrings(bmgTextboxesNames, bmgTextboxesIDs, 0x68, 5);

		layout.resetAnim(0);
		
		layout.update();
		layout.execAnimations();

		layout.disableAllAnimations();

		layout.drawOrder = 0x97;

		rootPane->flag &= 0xFE;

		isVisible = false;
		_1E7 = false;
		layoutLoaded = true;

		OSReport("Classic loaded!\n");
	}

	return true;
}

bool dWarningClassic_c::doDelete() {
	return layout.free();
}

void dWarningClassic_c::execute() {
	if(isVisible) {
		acState.execute();
		layout.execAnimations();
		layout.update();
	}
}

void dWarningClassic_c::draw() {
	if(isVisible) {
		layout.scheduleForDrawing();
	}
}

void dWarningClassic_c::setPlayerNo(u32 num) {
	RemNumber = num;
	isVisible = 1;
	_1E6 = 1;
}

void dWarningClassic_c::WindowCloseSetup() {
	_1E6 = 1;
	_1E7 = 1;
}

void dWarningClassic_c::beginState_StartWait() {
	rootPane->flag &= 0xFE;
}

void dWarningClassic_c::executeState_StartWait() {
	WriteBMGToTextBox(T_player, GetBMG(), 0x68, RemNumber+4, 0);
	acState.setState(&StateID_OpenAnimeEndWait);
}

void dWarningClassic_c::endState_StartWait() { }

void dWarningClassic_c::beginState_OpenAnimeEndWait() {
	_1E6 = 1;
	rootPane->flag &= 0xFE;
	rootPane->flag |= 1;
	layout.enableNonLoopAnim(0);
}

void dWarningClassic_c::executeState_OpenAnimeEndWait() {
	if(!layout.isAnimOn(0)) {
		acState.setState(&StateID_ExitWait);
	}
}

void dWarningClassic_c::endState_OpenAnimeEndWait() {
	_1E6 = 0;
}

void dWarningClassic_c::beginState_ExitWait() {
	_1E7 = 0;
	layout.enableNonLoopAnim(1);
}

void dWarningClassic_c::executeState_ExitWait() {
	if(_1E7) {
		_1E7 = 0;
		acState.setState(&StateID_ExitAnimeEndWait);
	}
}

void dWarningClassic_c::endState_ExitWait() {
	layout.disableAnim(1);
}


void dWarningClassic_c::beginState_ExitAnimeEndWait() {
	_1E6 = 1;
	layout.enableNonLoopAnim(2);
}

void dWarningClassic_c::executeState_ExitAnimeEndWait() {
	if(!layout.isAnimOn(2)) {
		acState.setState(&StateID_StartWait);
	}
}

void dWarningClassic_c::endState_ExitAnimeEndWait() {
	isVisible = 0;
	_1E6 = 0;
}