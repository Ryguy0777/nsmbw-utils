#include <controls/custom_controllers.h>

// dRemoconMng_c stuff

void newInitializeNone(dRemoconMng_c::dConnect_c::dExtension_c *self) {
    OSReport("sideways wiimote detected in port %d!\n", self->controllerID+1);
    self->extensionType = ControllerType::Wiimote;
    dCustomControllerInfo *cont = getCustomController(self->controllerID);
    cont->flags &= ~CCFLAG_HAS_CUSTOM;
    cont->flags &= ~CCFLAG_NO_ACC;
    cont->flags &= ~CCFLAG_NO_WIIMOTE;
    cont->controllerType = ControllerType::Wiimote;
    return;
}

void newExecuteNone(dRemoconMng_c::dConnect_c::dExtension_c *self) {
    u32 extension = *&(CoreControllers[self->controllerID]->status).dev_type;
    if (extension != WPAD_DEV_CORE) {
        dCustomController_c::changeRemoconMgrState(self, extension);
    }
    return;
}

void newInitializeFreestyle(dRemoconMng_c::dConnect_c::dExtension_c *self) {
    OSReport("nunchuck detected in port %d!\n", self->controllerID+1);
    self->extensionType = ControllerType::Nunchuck;
    dCustomControllerInfo *cont = getCustomController(self->controllerID);
    cont->flags &= ~CCFLAG_HAS_CUSTOM;
    cont->flags &= ~CCFLAG_NO_ACC;
    cont->flags &= ~CCFLAG_NO_WIIMOTE;
    cont->controllerType = ControllerType::Nunchuck;
    return;
}

void newExecuteFreestyle(dRemoconMng_c::dConnect_c::dExtension_c *self) {
    u32 extension = *&(CoreControllers[self->controllerID]->status).dev_type;
    if (extension != WPAD_DEV_FREESTYLE) {
        dCustomController_c::changeRemoconMgrState(self, extension);
    }
    return;
}

void newInitializeOther(dRemoconMng_c::dConnect_c::dExtension_c *self) {
    OSReport("invalid controller detected in port %d!\n", self->controllerID+1);
    self->extensionType = ControllerType::Invalid;
    dCustomControllerInfo *cont = getCustomController(self->controllerID);
    cont->flags &= ~CCFLAG_HAS_CUSTOM;
    cont->flags &= ~CCFLAG_NO_ACC;
    cont->flags &= ~CCFLAG_NO_WIIMOTE;
    cont->controllerType = ControllerType::Invalid;
    return;
}

void newExecuteOther(dRemoconMng_c::dConnect_c::dExtension_c *self) {
    u32 extension = *&(CoreControllers[self->controllerID]->status).dev_type;
    if (extension != WPAD_DEV_FUTURE && extension !=WPAD_DEV_GCN_FUTURE) {
        dCustomController_c::changeRemoconMgrState(self, extension);
    }
    return;
}

void newInitializeWait(dRemoconMng_c::dConnect_c::dExtension_c *self) {
    self->extensionType = ControllerType::None;
    dCustomControllerInfo *cont = getCustomController(self->controllerID);
    cont->flags &= ~CCFLAG_HAS_CUSTOM;
    cont->flags &= ~CCFLAG_NO_ACC;
    cont->flags &= ~CCFLAG_NO_WIIMOTE;
    cont->controllerType = ControllerType::None;
    return;
}

void newExecuteWait(dRemoconMng_c::dConnect_c::dExtension_c *self) {
    u32 extension = *&(CoreControllers[self->controllerID]->status).dev_type;
    if (extension != WPAD_DEV_NOT_FOUND) {
        dCustomController_c::changeRemoconMgrState(self, extension);
    }
    return;
}