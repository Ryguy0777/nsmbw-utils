#pragma once
#include <game.h>

// Size: 0xD3C
class dSelectCursor_c : public dBase_c {
public:
	// Size: 0x264
	class Layout_c {
	public:
		m2d::EmbedLayout_c layout;		 // 0x00
		nw4r::lyt::Pane *rootPane;		 // 0x198
		nw4r::lyt::Picture *P_cursor_00; // 0x19C
		nw4r::lyt::Picture *P_cursor_01; // 0x1A0
		nw4r::lyt::Picture *P_cursor_02; // 0x1A4
		nw4r::lyt::Picture *P_cursor_03; // 0x1A8
		nw4r::lyt::Pane *N_cursor_00;	 // 0x1AC
		nw4r::lyt::Pane *N_LU_00;		 // 0x1B0
		nw4r::lyt::Pane *N_RU_00;		 // 0x1B4
		nw4r::lyt::Pane *N_LD_00;		 // 0x1B8
		nw4r::lyt::Pane *N_RD_00;		 // 0x1BC
		bool isActive;					 // 0x1C0
		char targetName[17];			 // 0x1C1
		u8 unk[86];						 // 0x1D2
		Vec2 targetSize;				 // 0x228
		float _230;						 // 0x230
		float _234;						 // 0x234
		Vec2 targetScale;				 // 0x238
		Vec2 targetTrans;				 // 0x240
		u8 unk2[4];						 // 0x248
		Vec2 addedToTargetTrans;		 // 0x24C
		float _254;						 // 0x254
		u8 origin;						 // 0x258
		u8 origin_2;					 // 0x259
		u8 pad[2];						 // 0x25A
		int relatedToAlpha;				 // 0x25C
		u8 _260;						 // 0x260
		u8 pad2[3];						 // 0x261
	};

	m2d::ResAccLoader_c resLoader;	// 0x70
	Layout_c cursors[5];			// 0x144
	bool layoutLoaded;				// 0xD38
	u8 pad[3];						// 0xD39

	int onExecute();
	void setPosition(int cursorIdx); // 0x8010C620
};