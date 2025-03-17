#include <game.h>
#include <profile.h>

class daBahEvent_c : public dStageActor_c {
    public:
        u8 bahCount;

        int onCreate();
	    int onExecute();

        static dActor_c* build();
}; 

dActor_c* daBahEvent_c::build() {
	void *buffer = AllocFromGameHeap1(sizeof(daBahEvent_c));
	return new(buffer) daBahEvent_c;
}

const SpriteData BahpEventSpriteData = {ProfileId::AC_BAH_EVENT, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x2};
Profile BahpEventProfile(&daBahEvent_c::build, SpriteId::AC_BAH_EVENT, &BahpEventSpriteData, ProfileId::AC_FLAGON, ProfileId::AC_BAH_EVENT, "AC_BAH_EVENT");

int daBahEvent_c::onCreate() {
    bahCount = settings & 0xF;

	return true;
}

extern "C" bool CheckDanceValues_Bahps(u32 param);

int daBahEvent_c::onExecute() {
	bool dance = CheckDanceValues_Bahps(bahCount);
	bool flagActiveState = dFlagMgr_c::instance->active(eventId1-1);
	if (dance) {
		dFlagMgr_c::instance->set(eventId1-1, 0, !flagActiveState, false, false);
	}
	return true;
}