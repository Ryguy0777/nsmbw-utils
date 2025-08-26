#include <controls/custom_controllers.h>
#include <sfx.h>

// allow game to recognize certain extensions if enabled
// add the support defines to your project yaml defines to recognize them

BOOL isGuitarEnabled() {
#ifdef SUPPORT_GUITAR
    return TRUE;
#else
    return FALSE;
#endif
}

// custom controller manager class

extern "C" void *operator_new(ulong size);

void createCustomControllerClass() {
    OSReport("making dCustomController_c!\n");
	dCustomController_c *c = (dCustomController_c *)operator_new(sizeof(dCustomController_c));
    Vec2 zeroVec2 = {0,0};
    if (c != NULL) {
        for (int i = 0; i < 4; i++) {
            c->controllers[i] = (dCustomControllerInfo){
                ControllerType::None,
                0,
                0,
                (dPADInfo)
                {0, 0, 0, zeroVec2, zeroVec2, 0.0, 0.0}
            };
        }
        c->isHBMOpen = false;
        dCustomController_c::instance = c;
    }
    OSReport("Done! dCustomController_c at %p\n", dCustomController_c::instance);
    PADInit();
    OSReport("PADInit() called!\n");
}

void dCustomController_c::changeRemoconMgrState(dRemoconMng_c::dConnect_c::dExtension_c *self, u32 extension) {
    switch (extension) {
        case WPAD_DEV_CORE: // sideways wiimote
            self->state.setState(&dRemoconMng_c::dConnect_c::dExtension_c::StateID_None);
            return;
        case WPAD_DEV_FREESTYLE: // nunchuck
            self->state.setState(&dRemoconMng_c::dConnect_c::dExtension_c::StateID_Freestyle);
            return;
        case WPAD_DEV_CLASSIC: // classic
            self->state.setState(&dRemoconMng_c::dConnect_c::dExtension_c::StateID_Classic);
            return;
        case WPAD_DEV_GAMECUBE: // gamecube
            self->state.setState(&dRemoconMng_c::dConnect_c::dExtension_c::StateID_GameCube);
            return;
        case WPAD_DEV_GUITAR: // guitar
            self->state.setState(&dRemoconMng_c::dConnect_c::dExtension_c::StateID_Guitar);
            return;
        case WPAD_DEV_FUTURE: // unsupported
        case WPAD_DEV_GCN_FUTURE: // unsupported gc
            self->state.setState(&dRemoconMng_c::dConnect_c::dExtension_c::StateID_Other);
            return;
    }
}

void dCustomController_c::setPadInfo() {
    for (int i = 0; i < 4; i++) {
        // get raw inputs
        PADStatus *padStatus = &PadStatus[i];
        dCustomControllerInfo *cont = getCustomController(i);

        // buttons
        u32 lastButton = cont->padInfo.hold;
        u32 gc_held = padStatus->button;

        cont->padInfo.hold = gc_held;
        cont->padInfo.trig = gc_held & ~lastButton;
        cont->padInfo.release = lastButton & gc_held;

        // triggers
        // apparently OEM controllers never report a full analog value
        // so we report a full press if the digital input is being pressed
        cont->padInfo.ltrigger = (gc_held & PAD_TRIGGER_L) ? 1.0f : (float) padStatus->triggerL / 255;
        cont->padInfo.rtrigger = (gc_held & PAD_TRIGGER_R) ? 1.0f : (float) padStatus->triggerR / 255;

        // sticks
        cont->padInfo.stick.x = (float) padStatus->stickX / 110;
        cont->padInfo.stick.y = (float) padStatus->stickY / 110;
        cont->padInfo.substick.x = (float) padStatus->substickX / 110;
        cont->padInfo.substick.y = (float) padStatus->substickY / 110;
    }
}

void dCustomController_c::shakeController(int channel) {
    // only shake if replay is not active
    if (replayClasses[channel] == NULL) {
        GameKeyInstance->remocons[channel]->_8D = true;
    }
}

void dCustomController_c::clearWiimotedata(KPADStatus *kStatus, bool disablePointer) {
    Vec zeroVec = {0,0,0};
    Vec2 zeroVec2 = {0,0};

    if (!disablePointer) {
        if (!isHBMOpen) {
            kStatus->hold &= WPAD_BUTTON_HOME;
            kStatus->trigger &= WPAD_BUTTON_HOME;
            kStatus->release &= WPAD_BUTTON_HOME;
        }
    } else {
        kStatus->hold = 0;
        kStatus->trigger = 0;
        kStatus->release = 0;
    }
    kStatus->acc = zeroVec;
    kStatus->acc_magnitude = 0;
    kStatus->acc_variation = 0;
    if (disablePointer) {
        kStatus->pos = zeroVec2;
        kStatus->vec = zeroVec2;
        kStatus->speed = 0;
        kStatus->horizon = zeroVec2;
        kStatus->hori_vec = zeroVec2;
        kStatus->hori_speed = 0;
        kStatus->dist = 0;
        kStatus->dist_vec = 0;
        kStatus->dist_speed = 0;
    }
    kStatus->acc_vertical = zeroVec2;
}

// map inputs

void EGG::CoreController::mapButtons() {
    // do original function
    beginFrame();

#ifdef DEBUG_INPUTS
    OSReport("Wiimote: %08x %08x %08x\n", status.hold, status.trigger, status.release);
#endif

    // assign dInfo_c controller id
    if (channel == 0) {
        GameMgrP->currentControllerType = GameKeyInstance->remocons[0]->controllerType;
    }

    // assign wpad device type based on gc port type
    u32 gcctype = dCustomController_c::instance->checkForGCConnection(channel);
    switch (gcctype) {
        case GCCType::GCCInvalid:
            status.dev_type = WPAD_DEV_GCN_FUTURE;
            break;
        case GCCType::GCCStandard:
            status.dev_type = WPAD_DEV_GAMECUBE;
            break;
    }

    if (gcctype != GCCType::GCCNone) {
        // register controller as present
        flag |= 1;
    }

    // custom controller inputs
    switch (status.dev_type) {
        case WPAD_DEV_CLASSIC:
            return dCustomController_c::instance->mapClassicButtons(this, channel);
        case WPAD_DEV_GAMECUBE:
            return dCustomController_c::instance->mapGameCubeButtons(this, channel);
        case WPAD_DEV_GUITAR:
            return dCustomController_c::instance->mapGuitarButtons(this, channel);
    }
}

// bool to see if hbm is open
extern "C" void enterHBM(void *);
extern "C" void exitHBM(void *);

void HBMOpen() {
    enterHBM(_8042A788);
    OSReport("opening HBM!\n");
    dCustomController_c::instance->isHBMOpen = true;
}

void HBMClose() {
    exitHBM(_8042A788);
    OSReport("closing HBM!\n");
    dCustomController_c::instance->isHBMOpen = false;
}

// trick worldmap into using sideways buttons

bool dWmLib_isYokoCon(int player) {
    dCustomControllerInfo *cont = getCustomController(player);
    if (cont->flags & CCFLAG_NO_ACC || cont->controllerType == ControllerType::Nunchuck)
        return false;
    return true;
}

// don't recognize holding B

bool newCheckCancel(int player) {
    KPADEXStatus_cl *cStatus = &CoreControllers[player]->status.ex_status.cl;
    dCustomControllerInfo *cont = getCustomController(player);
    switch (cont->controllerType) {
        case ControllerType::GameCube:
            return cont->padInfo.trig & PAD_BUTTON_B;
        case ControllerType::Classic:
            return cStatus->trig & WPAD_CL_BUTTON_B;
        case ControllerType::Nunchuck:
            return CoreControllers[player]->downTrigger(WPAD_BUTTON_1 | WPAD_BUTTON_B);
        default:
            return CoreControllers[player]->downTrigger(WPAD_BUTTON_1);
    }
}

// don't play the propeller spin effects every frame the player is holding the shake button

extern "C" void daPlBase_c_playSound(daPlBase_c *self, SFX sound, long);

void playSpinSound(dAcPy_c *self) {
    daPlBase_c_playSound(self, SE_PLY_PRPL_LETDOWN_SPIN, 0);
    if (*(u32 *)(((u8*)self)+0x1a10) == 0)
        ((u32*)self)[0x684] = 1;
}

u8 spinSfxDelays[4] = {0, 0, 0, 0};

void setPropelFallSpinEffect(dAcPy_c *self) {
    u32 something = *(u32 *)(((u8*)self)+0x1a10);
    dCustomControllerInfo *cont = getCustomController(self->input.playerID, 1);
    if (cont == NULL) {
        playSpinSound(self);
        return;
    }
    switch (cont->controllerType) {
        case ControllerType::Guitar:
        case ControllerType::GameCube:
        case ControllerType::Classic:
            if (spinSfxDelays[self->input.playerID] < 1 || something == 0) {
                spinSfxDelays[self->input.playerID] = 10;
                playSpinSound(self);
            } else {
                spinSfxDelays[self->input.playerID]--;
            }
            break;
        default:
            playSpinSound(self);
            break;
    }
}

// tilt controlled objects
// instead of using tilt, these will use shoulder buttons

s16 getLRShort(u32 pid) {
    dGameKeyCore_c *input = GameKeyInstance->remocons[pid];
    // return tilt value as a usable s16
    return (s16)(int)(((input->acc_vert_x).y / 30.0) * 16384.0);
}

// for objects where L and R values simply add to the object's rotation

bool handleTiltAdd(s16 *rot, s16 target, s16 increment, s8 player, s16 max) {
    if (player < 0)
        return SmoothRotation(rot, target, increment);
    
    // if no accelerometer, return
    dCustomControllerInfo *cont = getCustomController(player, 1);
    if (cont == NULL || ((cont->flags & CCFLAG_NO_ACC) == 0))
        return SmoothRotation(rot, target, increment);
    
    // add current L/R values to object rotation
    s16 tiltVal = getLRShort(player);
    s16 newTarget = (*rot + tiltVal);
    if (max != 0)
        newTarget = clamp<s16>(newTarget, -max, max);
    
    return SmoothRotation(rot, newTarget, increment);
}

// 1-up blast cannon

bool handleRemoCannonTilt(void *cannon, short target) {
    return handleTiltAdd((s16*)((u8*)cannon + 0x7a4), target, 400, *(u32*)(((u8*)cannon)+0x778), 0x3520);
}

// tilt controlled wire fence

extern "C" s16 remoWireGetTilt(void *);

s16 fixRemoWireDirection(void *remoWire) {
    // get tilt amount
    s16 tilt = remoWireGetTilt(remoWire);
    if (tilt < 0x1a2c) {
        if (tilt < -0x1a2b)
            tilt = 0xffffe5d4;
    } else {
        tilt = 0x1a2c;
    }

    // if no accelerometer, return
    dCustomControllerInfo *cont = getCustomController((int)*(s8*)((u8*)remoWire+0x63d), 1);
    if (cont == NULL || ((cont->flags & CCFLAG_NO_ACC) == 0))
        return tilt;
    
    // otherwise return the inverse of the tilt value
    return -tilt;
}

// tilt controlled girder

extern "C" s16 getRemoTiltValue(int, float, float);

s16 seesawRots[4] = {0, 0, 0, 0};

s16 handleRemoSeesawTiltStart(void *remoSeesaw) {
    int player = (int)*(char *)((u8*)remoSeesaw+0x430);
    s16 tiltVal;
    // if no accelerometer, return
    dCustomControllerInfo *cont = getCustomController(player, 1);
    if (cont == NULL || ((cont->flags & CCFLAG_NO_ACC) == 0)) {
        tiltVal = getRemoTiltValue(player,5.0,1.0);
    } else {
        s16 LR = getLRShort(player);
        s16 currentRot = *(short *)((u8*)remoSeesaw+0x54a);
        seesawRots[player] = clamp<s16>(seesawRots[player] + LR, -0x1a2c, 0x1a2c);
        tiltVal = seesawRots[player];
        // called when we gain control, so reset the tilt value
        seesawRots[player] = 0;
    }
    return tiltVal;
}

s16 handleRemoSeesawTilt(void *remoSeesaw) {
    int player = (int)*(char *)((u8*)remoSeesaw+0x430); 
    s16 tiltVal;
    // if no accelerometer, return
    dCustomControllerInfo *cont = getCustomController(player, 1);
    if (cont == NULL || ((cont->flags & CCFLAG_NO_ACC) == 0)) {
        tiltVal = getRemoTiltValue(player,5.0,1.0);
    } else {
        s16 LR = getLRShort(player);
        s16 currentRot = *(short *)((u8*)remoSeesaw+0x54a);
        seesawRots[player] = clamp<s16>(seesawRots[player] + LR, -0x1a2c, 0x1a2c);
        tiltVal = seesawRots[player];
    }
    if (tiltVal < 0x1a2c) {
        if (tiltVal < -0x1a2b) {
            tiltVal = -0x1a2c;
        }
    } else {
        tiltVal = 0x1a2c;
    }
    return tiltVal;
}