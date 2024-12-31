#include <asm_setup.S>
#include <asm_profiles.S>

.extern continueToMeltIce
.extern dontMeltTheIce

.text
.align 4

.global AddNipperFire
AddNipperFire: # 807f8104
    cmpwi r4, 0x13
    beq meltIce #broken check? not sure
    cmpwi r4, AC_PAKKUN_PUCHI_FIRE
    bne dontMelt

meltIce:
    b continueToMeltIce

dontMelt:
    b dontMeltTheIce
    