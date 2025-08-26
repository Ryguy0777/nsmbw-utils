#pragma once
#include <game.h>

class dfukidashiInfo_c {
public:
	virtual ~dfukidashiInfo_c();

	m2d::EmbedLayout_c layout;				 // 0x4
	dStateWrapper_c<dfukidashiInfo_c> state; // 0x19C
	Vec *playerPos; 	// 0x1D8
	Vec posOffset;		// 0x1DC -- varies depending on player and powerup
	float _1E8;			// 0x1E8 -- set to 640
	float _1EC;			// 0x1EC -- set to 352
	int controllerType; // 0x1F0

	nw4r::lyt::Pane *rootPane;				// 0x1F4
	u32 _1F8;								// 0x1F8
	nw4r::lyt::Picture *P_afterImageUD1;	// 0x1FC
	nw4r::lyt::Picture *P_UpDownImage_01;	// 0x200
	nw4r::lyt::Picture *P_afterImageK1;		// 0x204
	nw4r::lyt::Picture *P_katamuki_Hand;	// 0x208
	nw4r::lyt::Picture *P_arrow_00;			// 0x20C
	nw4r::lyt::Picture *P_UpDownImage_00;	// 0x210
	nw4r::lyt::TextBox *T_guide_02;			// 0x214
	nw4r::lyt::TextBox *T_guide_01;			// 0x218

	int playerID;			// 0x21C
	int objectType;			// 0x220
	u32 _224;				// 0x224
	u32 _228;				// 0x228 -- set to 480
	int someAnimID;			// 0x22C
	u32 _230;				// 0x230 -- set to -1
	int basedOnObjectType;	// 0x234 -- -1 if object type is 2 or 20, otherwise 0
	bool layoutLoaded;		// 0x238
	bool visible;			// 0x239
	u8 pad[2];				// 0x23A

	void ControllerConnectCheck();

	void draw();
};