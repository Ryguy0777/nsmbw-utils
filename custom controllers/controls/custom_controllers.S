#include <asm_setup.S>

.text
.align 4

.extern continueFromAssigningID
.global assignGameKeyID
assignGameKeyID:
    stw r0, 0x8(r3)
    b continueFromAssigningID

.extern __ct__17dWarningClassic_cFv
.extern doneWithCtorClassic
.extern doneWithDtorClassic
.extern __dt__17dWarningClassic_cFv

.extern __ct__18dWarningGameCube_cFv
.extern __dt__18dWarningGameCube_cFv

.global CtorClassic
CtorClassic:
	addi r3, r30, 0xB94
	bl __ct__17dWarningClassic_cFv
	addi r3, r30, 0xD80
	bl __ct__18dWarningGameCube_cFv
	addi r3, r30, 0x8DC
	b doneWithCtorClassic


.global DtorClassic
DtorClassic:
	addi r3, r30, 0xB94
	li r4, -1
	bl __dt__17dWarningClassic_cFv
	addi r3, r30, 0xD80
	li r4, -1
	bl __dt__18dWarningGameCube_cFv
	addi r3, r30, 0x74
	b doneWithDtorClassic

# 2DPlayer only jumps with A button
.extern fix2DPlayerJumpButton__Fi
.extern dontJump
.extern doJump
.global fix2DPlayerJump
fix2DPlayerJump:
	mr r12, r3
	mr r3, r7
	bl fix2DPlayerJumpButton__Fi
	cmpwi r3, 1
	mr r3, r12
	beq shouldJump
	b dontJump

shouldJump:
	b doJump


.extern dontJump2
.extern doJump2
.global fix2DPlayerJump2
fix2DPlayerJump2:
	mr r12, r3
	mr r3, r5
	bl fix2DPlayerJumpButton__Fi
	cmpwi r3, 1
	mr r3, r12
	beq shouldJump2
	b dontJump2

shouldJump2:
	b doJump2


.extern handleTiltAdd__FPsssScs

.extern returnFromNiceBoatTilt
.global handleTiltingNiceBoat
handleTiltingNiceBoat:
	mr r12, r6
	mr r11, r7
	lbz r6, 0x430(r30)
	li r7, 0x4000
	bl handleTiltAdd__FPsssScs
	mr r6, r12
	mr r7, r11
	b returnFromNiceBoatTilt


.extern mapStickToPosHBM__FiP7Point2d
.extern doneSettingPointerData
.extern setPointerData
.global useClassicInHBM
useClassicInHBM:
	lbz r0, 0x5C(r3)
	cmpwi r0, 2
	bne notClassic

    mr r3, r25
	addi r4, r26, 0xE4
	bl mapStickToPosHBM__FiP7Point2d

	b doneSettingPointerData

notClassic:
	lbz r0, 0x5E(r3)
	b setPointerData