#include <asm_setup.S>

.text
.extern SCMWorldAndLevelNumbers__FPQ34nw4r3lyt7TextBoxPQ34nw4r3lyt7TextBoxPQ34nw4r3lyt7TextBoxPQ34nw4r3lyt4Paneii
.extern handleFileters__FiiPQ23m2d13EmbedLayout_c
.extern returnSCMcrap
.global SCMWorldAndLevelNumber
SCMWorldAndLevelNumber:
	lwz r4,0x274(r30) #T_corseNum_00
	lwz r5,0x270(r30) #T_pictureFont_00
	lwz r6,0x244(r30) #some pane I think
	lwz r7,0x27c(r30) # world number
	lwz r8,0x280(r30) # level number
    bl SCMWorldAndLevelNumbers__FPQ34nw4r3lyt7TextBoxPQ34nw4r3lyt7TextBoxPQ34nw4r3lyt7TextBoxPQ34nw4r3lyt4Paneii

	lwz r3,0x27c(r30) # world number
	lwz r4,0x280(r30) # level number
	addi r5,r30,0x70  # layout
	bl handleFileters__FiiPQ23m2d13EmbedLayout_c

    b returnSCMcrap