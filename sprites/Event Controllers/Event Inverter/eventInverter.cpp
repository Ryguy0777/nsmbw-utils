#include <game.h>
#include <profile.h>

class daFlagInverter_c : public dStageActor_c {
    public:
        bool previousFlagState;
        
        int onExecute();

        static dActor_c* build();
};

dActor_c* daFlagInverter_c::build() {
	void *buffer = AllocFromGameHeap1(sizeof(daFlagInverter_c));
	return new(buffer) daFlagInverter_c;
}

const SpriteData eventInverterSpriteData = {ProfileId::AC_FLAG_INVERT, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x2};
Profile eventInverterProfile(&daFlagInverter_c::build, SpriteId::AC_FLAG_INVERT, &eventInverterSpriteData, ProfileId::AC_FLAGON, ProfileId::AC_FLAG_INVERT, "AC_FLAG_INVERT");

int daFlagInverter_c::onExecute() {
	bool target = dFlagMgr_c::instance->active(eventId2-1);
	bool flagActiveState = dFlagMgr_c::instance->active(eventId1-1);
	if (target != previousFlagState) {
        switch (settings & 0b11) {
            default:
                dFlagMgr_c::instance->set(eventId1-1, 0, !flagActiveState, false, false);
                break;
            case 1:
                if (target == true && previousFlagState == false) //only when event is enabled
                    dFlagMgr_c::instance->set(eventId1-1, 0, !flagActiveState, false, false);
                break;
            case 2:
                if (target == false && previousFlagState == true) //only when event is disabled
                    dFlagMgr_c::instance->set(eventId1-1, 0, !flagActiveState, false, false);
                break;
        }
	}
    previousFlagState = target;
	return true;
}