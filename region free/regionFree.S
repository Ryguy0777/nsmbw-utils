#include <asm_setup.S>

.text
.align 4

# skip region check in save file
.extern saveFileReadSuccess # 800cf8cc
.global regionFreeSaveFile
regionFreeSaveFile:
    b saveFileReadSuccess


# load title layout from Layout with no region in file names
.extern loadArc__Q23m2d13EmbedLayout_cFPCcb
.extern loadAnimations__Q23m2d13EmbedLayout_cFPPCci
.extern loadGroups__Q23m2d13EmbedLayout_cFPPCcPCii
.extern getRootPane__Q23m2d17EmbedLayoutBase_cFv
.extern getTextBoxes__Q23m2d13EmbedLayout_cCFPPCcPPQ34nw4r3lyt7TextBoxi
.extern setLangStrings__Q23m2d13EmbedLayout_cFPPCcPCiii
.extern getPanes__Q23m2d13EmbedLayout_cCFPPCcPPQ34nw4r3lyt4Panei

.global regionFreeTitleScreen
regionFreeTitleScreen:
    stwu r1, -0x10(r1)
	mflr r0
	stw r0, 0x14(r1)
	stw r31, 0xC(r1)
	mr r29, r3

    addi r3, r29, 0x70
    lis r4, OpeningTitleArc@h
	ori r4, r4, OpeningTitleArc@l
    li r5, 0
	bl loadArc__Q23m2d13EmbedLayout_cFPCcb

    cmpwi r3, 0
    bne loadedArc
    li r3, 0
    b return

loadedArc:
    addi r3, r29, 0x70
	lis r4, OpeningTitleBrlyt@h
	ori r4, r4, OpeningTitleBrlyt@l
	li r5, 0
	lwz r12, 8(r3)
	lwz r12, 0x14(r12)
	mtctr r12
	bctrl

    addi r3, r29, 0x70
    lis r4, OpeningTitleAnims@h
	ori r4, r4, OpeningTitleAnims@l
    li r5, 4
    bl loadAnimations__Q23m2d13EmbedLayout_cFPPCci
    
    addi r3, r29, 0x70
	lis r4, OpeningTitleGroups@h
	ori r4, r4, OpeningTitleGroups@l
	lis r5, OpeningTitleGroupIDs@h
	ori r5, r5, OpeningTitleGroupIDs@l
	li r6, 4
	bl loadGroups__Q23m2d13EmbedLayout_cFPPCcPCii

	addi r3, r29, 0x70
    bl getRootPane__Q23m2d17EmbedLayoutBase_cFv
    stw r3, 0x244(r29)

	addi r3, r29, 0x70
	lis r4, OpeningTitleTextBoxes@h
	ori r4, r4, OpeningTitleTextBoxes@l
	addi r5, r29, 0x248
	li r6, 1
	bl getTextBoxes__Q23m2d13EmbedLayout_cCFPPCcPPQ34nw4r3lyt7TextBoxi

    addi r3, r29, 0x70
    lis r4, OpeningTitleLangBoxes1@h
	ori r4, r4, OpeningTitleLangBoxes1@l
    lis r5, OpeningTitleMsgIds@h
	ori r5, r5, OpeningTitleMsgIds@l
    li r6, 0x3e9
    li r7, 1
    bl setLangStrings__Q23m2d13EmbedLayout_cFPPCcPCiii

    addi r3, r29, 0x70
    lis r4, OpeningTitleLangBoxes2@h
	ori r4, r4, OpeningTitleLangBoxes2@l
    lis r5, OpeningTitleMsgIds@h
	ori r5, r5, OpeningTitleMsgIds@l
    li r6, 0x32
    li r7, 1
    bl setLangStrings__Q23m2d13EmbedLayout_cFPPCcPCiii

    addi r3, r29, 0x70
    lis r4, OpeningTitlePanes@h
	ori r4, r4, OpeningTitlePanes@l
    addi r5, r29, 0x24c
    li r6, 1
    bl getPanes__Q23m2d13EmbedLayout_cCFPPCcPPQ34nw4r3lyt4Panei

	li r3, 1
return:
	lwz r0, 0x14(r1)
	lwz r31, 0xC(r1)
	mtlr r0
	addi r1, r1, 0x10
	blr


.data
OpeningTitleArc: .string "openingTitle.arc"
OpeningTitleBrlyt: .string "openingTitle_00.brlyt"
OTinTitle: .string "openingTitle_00_inTitle.brlan"
OTinPress: .string "openingTitle_00_inPress.brlan"
OTloopPress: .string "openingTitle_00_loopPress.brlan"
OToutPress: .string "openingTitle_00_outPress.brlan"
A00_title: .string "A00_title"
B00_press: .string "B00_press"
T_E3verCheck: .string "T_E3verCheck"
T_2009nintendo: .string "T_2009nintendo"
T_Press_00: .string "T_Press_00"
N_proportionC_00: .string "N_proportionC_00"
.align 4
OpeningTitleAnims: .long OTinTitle, OTinPress, OTloopPress, OToutPress
OpeningTitleGroups: .long A00_title, B00_press, B00_press, B00_press
OpeningTitleGroupIDs: .long 0, 1, 2, 3
OpeningTitleTextBoxes: .long T_E3verCheck
OpeningTitleLangBoxes1: .long T_2009nintendo
OpeningTitleLangBoxes2: .long T_Press_00
OpeningTitleMsgIds: .long 0
OpeningTitlePanes: .long N_proportionC_00