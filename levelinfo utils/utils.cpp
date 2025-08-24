#include <common.h>
#include <utils.h>
#include "levelinfo.h"
#include <levelnumber.h>

static const wchar_t *levelNumbers[] = {
	//mariofont
	L"0",
	L"1",
	L"2",
	L"3",
	L"4",
	L"5",
	L"6",
	L"7",
	L"8",
	L"9",
	L"A", //10
	L"B", //11
	L"C", //12
	L"D", //13
	L"E", //14
	L"F", //15
	L".", //16
	L":", //17
	L"!", //18
	L"?", //19
	//picturefont
	L"G", //20, coin
	L"0", //21, ghost house
	L"/", //22, tower
	L"/", //23, tower 2
	L".", //24, castle
	L"=", //25, bowser castle
	L"9", //26, green house
	L"3", //27, red house
	L"8", //28, star house
	L"3", //29, red house
	L"@", //30, ?-block
	L"A", //31, red block
	L"B", //32, outlined block
	L">", //33, ambush
	L">", //34, ambush
	L">", //35, ambush
	L"1", //36, cannon
	L"?", //37, anchor
	L"2", //38, airship
	L"5", //39, up arrow
	L"4", //40, right arrow
	L"7", //41, peach castle
	L"6", //42, dot
};

static const wchar_t *worldNumbers[] = {
	L"0",
	L"1",
	L"2",
	L"3",
	L"4",
	L"5",
	L"6",
	L"7",
	L"8",
	L"9",
	L"A",
	L"B",
	L"C",
	L"D",
	L"E",
	L"F",
};

const wchar_t* getLevelNumber(int level) {
	if ((level >= 39) && (level <= 40)) {
		switch(getStartingHouseKind()) {
			case 0: //arrow
				return levelNumbers[level];
				break;
			case 1: //yellow
			case 4:
				return levelNumbers[28];
				break;
			case 2: //red
			case 5:
				return levelNumbers[29];
				break;
			case 3: //green
			case 6:
				return levelNumbers[26];
				break;
		}
	}
	return levelNumbers[level];
}

const wchar_t* getWorldNumber(int world) {
	return worldNumbers[world];
}

LevelNumber newGetCourseNum(int worldNum, int levelNum) {
	if ((u32)worldNum > 8) {
		return Stage_Invalid;
	}
	if ((u32)levelNum < 12) {
		//does our world exist?
		dLevelInfo_c::entry_s *liWorld = dLevelInfo_c::s_info.searchBySlot(worldNum, 38);
		if (liWorld) {
			//if so, grab the corresponding section
			dLevelInfo_c::section_s *section = dLevelInfo_c::s_info.getSectionByIndex(worldNum);

			//create list of empty slots
			int levelNumList[12];
			for (int i = 0; i < 12; i++) {
				levelNumList[i] = 42;
			}

			//fill the list with our level slots
			int index = 0;
			for (int i = 0; i < section->levelCount; i++) {
				dLevelInfo_c::entry_s *level = &section->levels[i];
				if (level->flags & 2) {
					levelNumList[index] = level->levelSlot;
					index++;
				}
			}
			//return the level number
			return (LevelNumber)levelNumList[levelNum];
		} else {
			return Stage_Invalid;
		}
	}
	return Stage_Invalid;
}

extern "C" bool isHomeCourseClear(int world); // 800fcb30
extern "C" u8 getStartKinokoKind(); // 800fb460
extern u8 CurrentWorldNumForWorldMap;
u32 getStartingHouseKind() {
	if (!isHomeCourseClear(CurrentWorldNumForWorldMap)) {
		return 0;
	} else {
		return getStartKinokoKind();
	}
}