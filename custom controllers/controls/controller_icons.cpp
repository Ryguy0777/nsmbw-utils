#include <controls/controller_icons.h>

// controller button icons

extern "C" int TagProcessor_MsgIDSet(void *TagProcessor_c, dScript::Res_c *MsgRes, ulong category, ulong id);

// "menu" buttons
int addNewButtonsToBMG1(void *TagProcessor_c, dScript::Res_c *MsgRes, char *param) {
    char parameter = *param;
    ulong messageID;
    ulong category = 0; // in order to use custom entries I've added a category variable that is always set to 0 normally
    if (parameter == '\0') {
        messageID = 6;
    } else if (parameter == '\x01') {
        messageID = 7;
    } else if (parameter == '\x02') {
        messageID = 8;
    } else if (parameter == '\x03') {
        messageID = 2;
    } else if (parameter == '\x04') {
        messageID = 4;
    } else if (parameter == '\x05') {
        messageID = 0xB;
    } else if (parameter == '\x06') { // classic y
        messageID = 0;
        category = 0xf0;
    } else if (parameter == '\x07') { // classic b
        messageID = 1;
        category = 0xf0;
    } else if (parameter == '\x08') { // classic ZL
        messageID = 2;
        category = 0xf0;
    } else if (parameter == '\x09') { // classic ZR
        messageID = 3;
        category = 0xf0;
    } else if (parameter == '\x0A') { // classic joystick
        messageID = 4;
        category = 0xf0;
    } else if (parameter == '\x0B') { // classic a
        messageID = 5;
        category = 0xf0;
    } else if (parameter == '\x0C') { // gamecube start
        messageID = 0xD;
        category = 0xf0;
    } else if (parameter == '\x0D') { // gamecube a
        messageID = 9;
        category = 0xf0;
    } else if (parameter == '\x0E') { // gamecube y
        messageID = 0xB;
        category = 0xf0;
    } else if (parameter == '\x0F') { // gamecube z
        messageID = 0x10;
        category = 0xf0;
    } else if (parameter == '\x10') { // gamecube L
        messageID = 0xE;
        category = 0xf0;
    } else if (parameter == '\x11') { // gamecube R
        messageID = 0xF;
        category = 0xf0;
    } else if (parameter == '\x12') { // gamecube stick
        messageID = 6;
        category = 0xf0;
    } else if (parameter == '\x13') { // gamecube c-stick
        messageID = 8;
        category = 0xf0;
    } else if (parameter == '\x14') { // classic L
        messageID = 0x11;
        category = 0xf0;
    } else if (parameter == '\x15') { // classic R
        messageID = 0x12;
        category = 0xf0;
    }
    return TagProcessor_MsgIDSet(TagProcessor_c, MsgRes, category, messageID);
}

// TODO: get strings that use these to update every frame. also use 200/300 for strings that should always show 1/2
// 1/2 being replaced with b/a
int addNewButtonsToBMG2(void *TagProcessor_c, dScript::Res_c *MsgRes, char *param, ulong messageID) {
    char parameter = *param;
    ulong category = 0;
    if (GameMgrP->currentControllerType == ControllerType::Classic) {
        if (parameter == '\0') { // classic a
            category = 0xf0;
            messageID = 5;
        } else if (parameter == '\x01') { // classic b
            category = 0xf0;
            messageID = 1;
        } else if (parameter == '\x04') { // classic y
            category = 0xf0;
            messageID = 0;
        }
    } else if (GameMgrP->currentControllerType == ControllerType::GameCube) {
        if (parameter == '\0') { // gamecube a
            category = 0xf0;
            messageID = 9;
        } else if (parameter == '\x01') { // gamecube b
            category = 0xf0;
            messageID = 0xA;
        } else if (parameter == '\x04') { // gamecube y
            category = 0xf0;
            messageID = 0xB;
        }
    } else if (GameMgrP->currentControllerType == ControllerType::Nunchuck) {
        if (parameter == '\0') {
            messageID = 2;
        } else if (parameter == '\x01') {
            messageID = 4;
        } else if (parameter == '\x04') {
            messageID = 4;
        }
    } else { // wiimote
        if (parameter == '\0') {
            messageID = 1;
        } else if (parameter == '\x01') {
            messageID = 3;
        } else if (parameter == '\x04') {
            messageID = 3;
        }
    }
    if (parameter == '\x02') {
        messageID = 1;
    } else if (parameter == '\x03') {
        messageID = 3;
    }
    return TagProcessor_MsgIDSet(TagProcessor_c, MsgRes, category, messageID);
}

// worldmap hud
void addNewButtonsToBMG3(void *TagProcessor_c, dScript::Res_c *MsgRes, char *param, ulong messageID) {
    char parameter = *param;
    ulong category = 0;
    if (parameter == '\0') {
        if (GameMgrP->currentControllerType == ControllerType::Nunchuck) {
            messageID = 10;
        } else if (GameMgrP->currentControllerType == ControllerType::Classic) { // classic y
            category = 0xf0;
            messageID = 0;
        } else if (GameMgrP->currentControllerType == ControllerType::GameCube) { // gamecube y
            category = 0xf0;
            messageID = 0xB;
        } else {
            messageID = 2;
        }
    } else if (parameter == '\x01') {
        if (GameMgrP->currentControllerType == ControllerType::GameCube) { // gamecube z
            messageID = 0x10;
            category = 0xf0;
        } else {
            messageID = 8;
        }
    } else if (parameter == '\x02') {
        if (GameMgrP->currentControllerType == ControllerType::GameCube) { // gamecube start
            category = 0xf0;
            messageID = 0xD;
        } else {
            messageID = 7;
        }
    } else if (parameter == '\x03') {
        if (GameMgrP->currentControllerType == ControllerType::Classic) { // classic b
            category = 0xf0;
            messageID = 1;
        } else if (GameMgrP->currentControllerType == ControllerType::GameCube) { // gamecube b
            category = 0xf0;
            messageID = 0xA;
        } else {
            messageID = (GameMgrP->currentControllerType == 1) + 3;
        }
    }
    TagProcessor_MsgIDSet(TagProcessor_c, MsgRes, category, messageID);
    return;
}

// d-pad icon, for the jukebox
int addNewButtonsToBMG4(void *TagProcessor_c, dScript::Res_c *MsgRes) {
    ulong messageID;
    ulong category = 0;
    if (GameMgrP->currentControllerType == ControllerType::GameCube) { // gamecube d-pad
        messageID = 7;
        category = 0xf0;
    } else { // wiimote/classic d-pad
        messageID = 5;
    }
    return TagProcessor_MsgIDSet(TagProcessor_c, MsgRes, category, messageID);
}


// update button prompts
void CourseSelectGuide_updateButtons(CourseSelectGuide *csg) {
    u32 currentController = GameKeyInstance->remocons[0]->controllerType;
    // set controller type
    csg->lastControllerTypeUsed = currentController;
    dScript::Res_c *bmg = GetBMG();
    // set map view pane
    if (csg->isMapViewExitVisible == 0) {
        WriteBMGToTextBox(csg->T_guideViewLS_00, bmg, 4, 10, 0);
    } else {
        WriteBMGToTextBox(csg->T_guideViewLS_00, bmg, 4, 0xE, 0);
    }
    // items pane
    WriteBMGToTextBox(csg->T_guideViewL_01, bmg, 4, 0xF, 0);
    nw4r::lyt::TextBox *T_guideViewC_00 = csg->layout.findTextBoxByName("T_guideViewC_00");
    nw4r::lyt::TextBox *T_guideViewR_00 = csg->layout.findTextBoxByName("T_guideViewR_00");
    WriteBMGToTextBox(T_guideViewC_00, bmg, 4, 0xB, 0);
    WriteBMGToTextBox(T_guideViewR_00, bmg, 4, 0xC, 0);
}

extern "C" int dStockItem_cExecute(dStockItem_c* self);

int StockItem_updateButtons(dStockItem_c* self) {
    int orig_val = dStockItem_cExecute(self);
    nw4r::lyt::TextBox *T_titleUse_01 = self->layout.findTextBoxByName("T_titleUse_01");
    WriteBMGToTextBox(T_titleUse_01, GetBMG(), 0x2, 0x2F, 0);
    return orig_val;
}

extern "C" int dCourseSelectMenu_c__Execute(void *self);

int CourseSelectMenu_updateButtons(void *self) {
    int orig_val = dCourseSelectMenu_c__Execute(self);
    m2d::EmbedLayout_c *layout = (m2d::EmbedLayout_c*)(((u8*)self)+0x70);
    nw4r::lyt::TextBox *T_back = layout->findTextBoxByName("T_back");
    WriteBMGToTextBox(T_back, GetBMG(), 0x2, 0x6, 0);
    return orig_val;
}

extern "C" int dCollectionCoin_c__Execute(void *self);

int CollectionCoin_updateButtons(void *self) {
    int orig_val = dCollectionCoin_c__Execute(self);
    m2d::EmbedLayout_c *layout = (m2d::EmbedLayout_c*)(((u8*)self)+0xE0);
    nw4r::lyt::TextBox *T_back_00 = layout->findTextBoxByName("T_back_00");
    WriteBMGToTextBox(T_back_00, GetBMG(), 0x2, 0x3A, 0);
    return orig_val;
}

extern "C" int dWorldSelectGuide_c__Execute(WorldSelectGuide *self);

int WorldSelectGuide_updateButtons(WorldSelectGuide *self) {
    int orig_val = dWorldSelectGuide_c__Execute(self);
    nw4r::lyt::TextBox *T_back_00 = self->layout.findTextBoxByName("T_back_00");
    nw4r::lyt::TextBox *T_back_01 = self->layout.findTextBoxByName("T_back_01");
    WriteBMGToTextBox(T_back_00, GetBMG(), 0x3, 0x1, 0);
    WriteBMGToTextBox(T_back_01, GetBMG(), 0x3, 0x1, 0);

    nw4r::lyt::Picture *P_juji_s_00 = self->layout.findPictureByName("P_juji_s_00");
    nw4r::lyt::Picture *P_gcPad_s_00 = self->layout.findPictureByName("P_gcPad_s_00");
    bool isCtrlGC = GameMgrP->currentControllerType == ControllerType::GameCube;
    P_juji_s_00->SetVisible(!isCtrlGC);
    P_gcPad_s_00->SetVisible(isCtrlGC);
    return orig_val;
}

extern "C" int dFileSelect_c__Execute(void *self);

int FileSelect_updateButtons(void *self) {
    int orig_val = dFileSelect_c__Execute(self);
    m2d::EmbedLayout_c *layout = (m2d::EmbedLayout_c*)(((u8*)self)+0x74);
    nw4r::lyt::TextBox *T_back_00 = layout->findTextBoxByName("T_back_00");
    nw4r::lyt::TextBox *T_copy_00 = layout->findTextBoxByName("T_copy_00");
    nw4r::lyt::TextBox *T_delete_01 = layout->findTextBoxByName("T_delete_01");
    WriteBMGToTextBox(T_back_00,   GetBMG(), 0x2, 0x1D, 0);
    WriteBMGToTextBox(T_copy_00,   GetBMG(), 0x32, 0x1, 0);
    WriteBMGToTextBox(T_delete_01, GetBMG(), 0x32, 0x3, 0);
    return orig_val;
}

extern "C" int dEventOpeningTitle_c__Execute(void *self);

int EventOpeningTitle_updateButtons(void *self) {
    int orig_val = dEventOpeningTitle_c__Execute(self);
    m2d::EmbedLayout_c *layout = (m2d::EmbedLayout_c*)(((u8*)self)+0x70);
    nw4r::lyt::TextBox *T_Press_00 = layout->findTextBoxByName("T_Press_00");
    WriteBMGToTextBox(T_Press_00, GetBMG(), 0x32, 0x0, 0);
    return orig_val;
}

extern "C" int dSelectPlayer_c__Execute(void *self);

int SelectPlayer_updateButtons(void *self) {
    int orig_val = dSelectPlayer_c__Execute(self);
    m2d::EmbedLayout_c *layout = (m2d::EmbedLayout_c*)(((u8*)self)+0xAC);
    nw4r::lyt::TextBox *T_back = layout->findTextBoxByName("T_back");
    WriteBMGToTextBox(T_back, GetBMG(), 0x2, 0x1D, 0);
    return orig_val;
}

extern "C" int dNumberOfPeopleChange_c__Execute(void *self);
extern u16 nowScene;

int NumberOfPeopleChange_updateButtons(void *self) {
    int orig_val = dNumberOfPeopleChange_c__Execute(self);
    m2d::EmbedLayout_c *layout = (m2d::EmbedLayout_c*)(((u8*)self)+0x404);
    nw4r::lyt::TextBox *T_guideS_02 = layout->findTextBoxByName("T_guideS_02");
    if (nowScene == WORLD_MAP) {
        WriteBMGToTextBox(T_guideS_02, GetBMG(), 0x2, 0xE, 0);
    } else {
        WriteBMGToTextBox(T_guideS_02, GetBMG(), 0x2, 0x1D, 0);
    }
    return orig_val;
}

extern "C" int dCharacterChangeIndicator_c__Execute(CharacterChangeIndicator *self);

int CharacterChangeIndicator_updateButtons(CharacterChangeIndicator *self) {
    int orig_val = dCharacterChangeIndicator_c__Execute(self);
    int messageIDs[2] = {0xA, 0xD};

    u32 currentPlrController;
    if (self->currentState == 0 && self->isFlash == 0) {
        // if player is not registered, use wiimote buttons
        currentPlrController = 0;
    } else {
        currentPlrController = GameKeyInstance->remocons[self->playerIndex]->controllerType;
    }
    
    GameMgrP->currentControllerType = currentPlrController;
    WriteBMGToTextBox(self->T_button_01, GetBMG(), 0x2, messageIDs[self->currentState == 1], 0);
    WriteBMGToTextBox(self->T_button_00, GetBMG(), 0x2, messageIDs[self->currentState == 1], 0);
    GameMgrP->currentControllerType = GameKeyInstance->remocons[0]->controllerType; // restore original value
    return orig_val;
}

extern "C" int dMultiCourseSelect_c__Execute(void *self);

int MultiCourseSelect_updateButtons(void *self) {
    int orig_val = dMultiCourseSelect_c__Execute(self);
    m2d::EmbedLayout_c *layout = (m2d::EmbedLayout_c*)(((u8*)self)+0x70);
    nw4r::lyt::TextBox *T_guideViewC_00 = layout->findTextBoxByName("T_guideViewC_00");
    WriteBMGToTextBox(T_guideViewC_00, GetBMG(), 0x32, 0x3D, 0);
    return orig_val;
}

extern "C" int dMiniGameWire_c__Execute(void *self);

int MiniGameWire_updateButtons(void *self) {
    int orig_val = dMiniGameWire_c__Execute(self);
    m2d::EmbedLayout_c *layout = (m2d::EmbedLayout_c*)(((u8*)self)+0x70);
    nw4r::lyt::TextBox *T_infoS_00 = layout->findTextBoxByName("T_infoS_00");
    nw4r::lyt::TextBox *T_info_00 = layout->findTextBoxByName("T_info_00");
    int messageID = (GetActivePlayerCount() > 1) ? 0x5 : 0x0;
    WriteBMGToTextBox(T_infoS_00, GetBMG(), 0x12D, messageID, 0);
    WriteBMGToTextBox(T_info_00,  GetBMG(), 0x12D, messageID, 0);
    return orig_val;
}

void dfukidashiInfo_c::ControllerConnectCheck() {
    int type = GameKeyInstance->remocons[playerID]->controllerType;
    if (type != this->controllerType) {
        this->controllerType = type;
        GameMgrP->currentControllerType = type;
        WriteBMGToTextBox(this->T_guide_01, GetBMG(), 0x0, 0x5A, 0);
        GameMgrP->currentControllerType = GameKeyInstance->remocons[0]->controllerType; // restore original value
    }
}

void dfukidashiInfo_c::draw() {
    if (!visible || getCustomController(playerID)->flags & CCFLAG_HAS_CUSTOM) {
        return;
    }
    layout.scheduleForDrawing();
}