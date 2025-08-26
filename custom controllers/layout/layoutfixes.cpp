#include <game.h>
#include <layout/dSelectCursor_c.h>

bool WM_ITEM_preExecute(dBase_c *self) {
	*((bool*)((u8*)self+0x20A)) = true; // needsToBeDrawnThisFrame
	return !QueryGlobal5758(0xFFFFFFFF);
}

bool SEQUENCE_BG_preExecute(dBase_c *self) {
	return !QueryGlobal5758(0xFFFFFFFF);
}

int dSelectCursor_c::onExecute() {
	for (int idx = 0; idx < 5; idx++) {
		if (this->cursors[idx].isActive) {
			this->setPosition(idx);
			// only stops for warning pauses that don't use a cursor
			if (!QueryGlobal5758(0xFFFFFFFE) || (this->cursors[idx].layout.drawOrder == 152)) {
				this->cursors[idx].layout.execAnimations();
			}
			this->cursors[idx].layout.update();
		}
	}
	return true;
}