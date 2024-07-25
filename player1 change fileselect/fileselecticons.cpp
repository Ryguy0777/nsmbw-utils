#include <common.h>
#include <game.h>

void DFSetIcons(dBase_c *self, SaveBlock *block) {
    //grab layout
    m2d::EmbedLayout_c *layout = (m2d::EmbedLayout_c*)((int)self + 0x74);

    //player icons
    nw4r::lyt::Picture *P_iconM_00 = layout->findPictureByName("P_iconM_00");
    nw4r::lyt::Picture *P_iconL_00 = layout->findPictureByName("P_iconL_00");
    nw4r::lyt::Picture *P_iconY_00 = layout->findPictureByName("P_iconY_00");
    nw4r::lyt::Picture *P_iconB_00 = layout->findPictureByName("P_iconB_00");
    P_iconM_00->SetVisible(false);
    P_iconL_00->SetVisible(false);
    P_iconY_00->SetVisible(false);
    P_iconB_00->SetVisible(false);

    //handle data
    int player1Lives;
    switch (block->lastP1Character) {
        default: //mario
            P_iconM_00->SetVisible(true);
            player1Lives = block->player_lives[0];
            break;
        case 1: //luigi
            P_iconL_00->SetVisible(true);
            player1Lives = block->player_lives[1];
            break;
        case 2: //boad
            P_iconB_00->SetVisible(true);
            player1Lives = block->player_lives[2];
            break;
        case 3: //yoad
            P_iconY_00->SetVisible(true);
            player1Lives = block->player_lives[3];
            break;
    }

    //player lives
    nw4r::lyt::TextBox *T_zanki_00 = layout->findTextBoxByName("T_zanki_00");
    const int length = 2;
    WriteNumberToTextBox(&player1Lives, &length, T_zanki_00, 1);

    return;
}

extern "C" int CSMonExecute(dBase_c *self);

int CSMStorePlayer(dBase_c *self) {
    SaveBlock *saveBlock = GetSaveFile()->GetBlock(-1);

    if (saveBlock->lastP1Character != Player_ID[0]) {
		OSReport("Player 1 id:%d\n", Player_ID[0]);
		saveBlock->lastP1Character = Player_ID[0];
	}

    return CSMonExecute(self);
}