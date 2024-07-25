#include <asm_setup.S>

.text
.extern DFSetIcons__FP7dBase_cP9SaveBlock
.extern returnFScrap
.global FSLoadIcons
FSLoadIcons:
    stwu sp, -0x80 (sp) #Push stack, make space for 29 registers
    stmw r3, 0x8 (sp)

    mr r3, r30
    mr r4, r31
    bl DFSetIcons__FP7dBase_cP9SaveBlock

    lmw r3, 0x8 (sp)
    addi sp, sp, 0x80 #Pop stack

    b returnFScrap