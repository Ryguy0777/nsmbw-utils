#include <controls/custom_controllers.h>

// dRemoconMng_c stuff

CREATE_STATE(dRemoconMng_c::dConnect_c::dExtension_c, Guitar);

void dRemoconMng_c::dConnect_c::dExtension_c::beginState_Guitar() {
    OSReport("guitar controller detected in port %d!\n", controllerID+1);
    extensionType = ControllerType::Guitar;
    dCustomControllerInfo *cont = getCustomController(controllerID);
    cont->flags |= CCFLAG_HAS_CUSTOM;
    cont->flags &= ~CCFLAG_NO_ACC;
    cont->flags &= ~CCFLAG_NO_WIIMOTE;
    cont->controllerType = ControllerType::Guitar;
    return;
}

void dRemoconMng_c::dConnect_c::dExtension_c::executeState_Guitar() {
    u32 extension = *&(CoreControllers[controllerID]->status).dev_type;
    if (extension != WPAD_DEV_GUITAR) {
        dCustomController_c::changeRemoconMgrState(this, extension);
    }
    return;
}

void dRemoconMng_c::dConnect_c::dExtension_c::endState_Guitar() { }

// controller mappings

void dCustomController_c::mapGuitarButtons(EGG::CoreController *controller, int controllerID) {
    KPADStatus *kStatus = &controller->status;
    KPADEXStatus_gh *gStatus = &controller->status.ex_status.gh;
    dCustomControllerInfo *customController = getCustomController(controllerID);

#ifdef DEBUG_INPUTS
    OSReport("Guitar: %08x %08x %08x\n", gStatus->hold, gStatus->trig, gStatus->release);
    OSReport("Guitar Stick: %f %f, Slider: %f, Whammy: %f\n", gStatus->stick.x, gStatus->stick.y, gStatus->slider, gStatus->whammy);
#endif

    // flip wiimote dpad
    u32 kpadInput = kStatus->hold & 0x9F10;
    if (kStatus->hold & WPAD_BUTTON_UP) {
        kpadInput |= WPAD_BUTTON_RIGHT;
    }
    if (kStatus->hold & WPAD_BUTTON_DOWN) {
        kpadInput |= WPAD_BUTTON_LEFT;
    }
    if (kStatus->hold & WPAD_BUTTON_RIGHT) {
        kpadInput |= WPAD_BUTTON_UP;
    }
    if (kStatus->hold & WPAD_BUTTON_LEFT) {
        kpadInput |= WPAD_BUTTON_DOWN;
    }
    kStatus->hold = kpadInput;

    // convert to sideways Wii Remote buttons
    if (gStatus->hold & GH3_BUTTON_STRUM_DOWN && !(kStatus->hold & WPAD_BUTTON_DOWN)) {
        kStatus->hold |= WPAD_BUTTON_UP;
    }
    if (gStatus->hold & GH3_BUTTON_STRUM_UP && !(kStatus->hold & WPAD_BUTTON_UP)) {
        kStatus->hold |= WPAD_BUTTON_DOWN;
    }
    if (gStatus->hold & GH3_BUTTON_BLUE && !(kStatus->hold & WPAD_BUTTON_RIGHT)) {
        kStatus->hold |= WPAD_BUTTON_LEFT;
    }
    if (gStatus->hold & GH3_BUTTON_YELLOW && !(kStatus->hold & WPAD_BUTTON_LEFT)) {
        kStatus->hold |= WPAD_BUTTON_RIGHT;
    }

    if (gStatus->hold & GH3_BUTTON_GREEN) {
        kStatus->hold |= WPAD_BUTTON_2;
    }
    if (gStatus->hold & GH3_BUTTON_RED) {
        kStatus->hold |= WPAD_BUTTON_1;
    }
    if (gStatus->hold & GH3_BUTTON_ORANGE) {
        kStatus->hold |= WPAD_BUTTON_A;
    }
    if (gStatus->hold & GH3_BUTTON_PLUS) {
        kStatus->hold |= WPAD_BUTTON_PLUS;
    }
    if (gStatus->hold & GH3_BUTTON_MINUS) {
        kStatus->hold |= WPAD_BUTTON_MINUS;
    }

    // map stick to D-pad
    if (gStatus->stick.y < -0.5 && !(kStatus->hold & WPAD_BUTTON_DOWN)) {
        kStatus->hold |= WPAD_BUTTON_UP;
    }
    if (gStatus->stick.y > 0.5 && !(kStatus->hold & WPAD_BUTTON_UP)) {
        kStatus->hold |= WPAD_BUTTON_DOWN;
    }
    if (gStatus->stick.x > 0.5 && !(kStatus->hold & WPAD_BUTTON_RIGHT)) {
        kStatus->hold |= WPAD_BUTTON_LEFT;
    }
    if (gStatus->stick.x < -0.5 && !(kStatus->hold & WPAD_BUTTON_LEFT)) {
        kStatus->hold |= WPAD_BUTTON_RIGHT;
    }

    // shake if whammy
    if (gStatus->whammy > 0.6) {
        //shake the damn thing
        dCustomController_c::shakeController(controllerID);
    }

    kStatus->trigger = kStatus->hold & ~customController->lastPressed;
    kStatus->release = customController->lastPressed & ~kStatus->hold;
    kStatus->wpad_err = 0;

    customController->lastPressed = kStatus->hold;
}