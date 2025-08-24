#pragma once

#include <common.h>

const wchar_t* getLevelNumber(int level);
const wchar_t* getWorldNumber(int world);

bool doesWorldExist(int world);

u32 getStartingHouseKind();