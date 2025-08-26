#pragma once

#include <game.h>

//size 0x1EC
class dWarningGameCube_c {
public:
    dWarningGameCube_c();
    ~dWarningGameCube_c();

    u32 vtable;
    m2d::EmbedLayout_c layout;
    dStateWrapper_c<dWarningGameCube_c> acState;

    nw4r::lyt::Pane *rootPane;
    nw4r::lyt::TextBox *T_player;
    nw4r::lyt::Pane *W_extensionGC;

    u8 layoutLoaded, isVisible, _1E6, _1E7, RemNumber;

    int createLayout();
    bool doDelete();
    void execute();
    void draw();

    void setPlayerNo(u32 num);
    void WindowCloseSetup();

    USING_STATES(dWarningGameCube_c);
    DECLARE_STATE(StartWait);
    DECLARE_STATE(OpenAnimeEndWait);
    DECLARE_STATE(ExitWait);
    DECLARE_STATE(ExitAnimeEndWait);
};