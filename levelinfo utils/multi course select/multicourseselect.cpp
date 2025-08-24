#include <stage.h>
#include <utils.h>
#include "levelinfo.h"
#include "multicourseselect.h"

#define COIN_BATTLE 0x40
extern u32 GameFlag;
extern LevelNumber newGetCourseNum(int worldNum, int levelNum);

// types: 0 = world, 1 = recommended
void dMultiCourseSelect_c::loadTitleFromLI(int tb, int type) {
	int wSlot = GameMgrP->numberToInsertInThing[0]-1;
	if (type == 1) {
		wSlot = 98 - ((GameFlag & COIN_BATTLE) == 0);
	}
	dLevelInfo_c::entry_s *liWorld = dLevelInfo_c::s_info.searchBySlot(wSlot, 38);
	if (liWorld) {
		const char *levelname = dLevelInfo_c::s_info.getNameForLevel(liWorld);
		wchar_t lbuffer[64];
		for (int i = 0; i < 64; i++) {
			lbuffer[i] = (unsigned short)levelname[i];
		}

		(&T_titleMulti_00)[tb]->SetString(lbuffer);
	} else {
		(&T_titleMulti_00)[tb]->SetString(L"World ?");
	}
}

void dMultiCourseSelect_c::setupTitle(int tb) {
	GameMgrP->numberToInsertInThing10 = (u32)this->N_scissor_00;
	GameMgrP->numberToInsertInThing11 = (u32)&this->layout.drawInfo;
	GameMgrP->numberToInsertInThing[5] = (tb != 1) ? 2 : 1;
	u32 textBoxToUpdate = tb != 1;
	int page = this->currentPage;
	int type = 0;

	if (page == 10) {
		type = 2;
	} else if (page == 0) {
		type = 1;
	} else {
		type = 0;
	}

	if (type == 2) {
		WriteBMGToTextBox((&T_titleMulti_00)[textBoxToUpdate], GetBMG(), 0x32, 0x1B, 0);
	} else {
		GameMgrP->numberToInsertInThing[0] = page;
		loadTitleFromLI(textBoxToUpdate, type);
	}
	colorTitleText(textBoxToUpdate, type);
}

void dMultiCourseSelectContents_c::loadInfo() {
	T_corseNum_00->SetVisible(false);
	T_corsePic_00->SetVisible(false);

	T_W_corseNum_00->SetVisible(true);
	T_W_corseNum_01->SetVisible(true);
	T_W_corsePic_00->SetVisible(false);

	dLevelInfo_c::entry_s *level = dLevelInfo_c::s_info.searchBySlot(worldNumber, levelNumber);
	if (level) {
		const wchar_t *worldName, *levelName;
		worldName = getWorldNumber(level->displayWorld);
		levelName = getLevelNumber(level->displayLevel);

		T_W_corseNum_00->SetString(worldName);
		if (level->displayLevel > 19) {
			T_W_corseNum_02->SetVisible(false);
			T_W_corsePic_00->SetVisible(true);
			T_W_corsePic_00->SetString(levelName);
		} else {
			T_W_corsePic_00->SetVisible(false);
			T_W_corseNum_02->SetVisible(true);
			T_W_corseNum_02->SetString(levelName);
		}
	} else {
		T_W_corseNum_00->SetString(L"?");
		T_W_corsePic_00->SetVisible(false);
		T_W_corseNum_02->SetVisible(true);
		T_W_corseNum_02->SetString(L"?");
	}
}

const char *dMultiCourseSelectContents_c::getTextureName(int world, int level) {
	static char buf[32];
	if (level == 19) {
		sprintf(buf, "msk_coin%02d.tpl", world+1);
	} else if (level > 19) {
		const char *string = "dummy";
		switch (level) {
			case 20: string = "obake"; break;
			case 21: string = "toride"; break;
			case 22: string = "toride2"; break; //new, tower 2
			case 23: string = "shiro"; break;
			case 24: string = "shiro2"; break; //new, castle 2
			case 37: string = "hikousen"; break;
		}
		sprintf(buf, "im_W%d_%s.tpl", world+1, string);
	} else {
		sprintf(buf, "im_W%d_%d.tpl", world+1, level+1);
	}
	return buf;
}


int getWorldForButton(int page, int button) {
	dLevelInfo_c::entry_s *liWorld = dLevelInfo_c::s_info.searchBySlot(page, 38);
	if (liWorld) {
		dLevelInfo_c::section_s *section = dLevelInfo_c::s_info.getSectionByIndex(page);
		for (int i = 0; i < section->levelCount; i++) {
			dLevelInfo_c::entry_s *level = &section->levels[i];
			if (level->levelSlot == newGetCourseNum(page, button)) {
				return level->worldSlot;
			}
		}
	}
	return 10;
}

int getRecommendedWorld(bool getFreeMode, int button) {
	int secIndex = dLevelInfo_c::s_info.sectionCount() - (getFreeMode + 1);
	dLevelInfo_c::section_s *section = dLevelInfo_c::s_info.getSectionByIndex(secIndex);
	if (section) {
		dLevelInfo_c::entry_s *level = &section->levels[button+2];
		return (section->levelCount <= button+2) ? 10 : level->worldSlot;
	} else {
		return 10;
	}
}

int getRecommendedLevel(bool getFreeMode, int button) {
	int secIndex = dLevelInfo_c::s_info.sectionCount() - (getFreeMode + 1);
	dLevelInfo_c::section_s *section = dLevelInfo_c::s_info.getSectionByIndex(secIndex);
	if (section) {
		dLevelInfo_c::entry_s *level = &section->levels[button+2];
		return (section->levelCount <= button+2) ? Stage_Invalid : level->levelSlot;
	} else {
		return Stage_Invalid;
	}
}

void dMultiCourseSelect_c::getLevelID(u8 *world, u8 *level, int button) {
	int page = this->currentPage;
	if (button > 9) {
		button -= 10;
	}

	if (page == 0) {
		*world = getRecommendedWorld((GameFlag & COIN_BATTLE) == 0, button);
		*level = getRecommendedLevel((GameFlag & COIN_BATTLE) == 0, button);
	}

	else if (page == 10) {
		*world = this->favorites[button].worldNum;
		*level = this->favorites[button].levelNum;
	}

	else {
		u8 worldNum = getWorldForButton(page-1, button);
		*world = worldNum;
		*level = newGetCourseNum(worldNum, button);
	}
}

void dMultiCourseSelect_c::setButtonLevelID(int button, int maxButtonCount) {
	u8 world, level;
	while (true) {
		if (maxButtonCount <= button) {
			return;
		}
		this->contents[button]->previousPage = this->currentPage;
		getLevelID(&world, &level, button);

		this->contents[button]->worldNumber = world;
		this->contents[button]->levelNumber = level;
		button += 1;
	}
}