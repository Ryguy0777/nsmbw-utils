#include <asm_setup.S>
#include <asm_profiles.S>

.extern checkForKoopas
.extern addActorToFence

.extern checkForKoopas2
.extern addActorToFence2

.extern continueWithNetExecute
.extern dontFlip

.text
.align 4

# The game does a hardcoded check on 3 different actors when a flip panel is flipped,
# Amps and the two Climbing Koopas
# We add a check for the fence shyguys
.global PatchWireIDs # 80aca5b8
PatchWireIDs:
    cmplwi r3, EN_HEIHO_NET # check for fence shyguy
    beq isFenceActor
    addis r3, r3, 0x1 # original instruction
    b checkForKoopas

isFenceActor:
    b addActorToFence

# This check does... something, not really sure what
.global PatchWireIDs2 # 80868710
PatchWireIDs2:
    cmplwi r3, EN_HEIHO_NET # check for fence shyguy
    beq isFenceActor2
    addis r3, r3, 0x1 # original instruction
    b checkForKoopas2

isFenceActor2:
    b addActorToFence2

# here, we fix the infamous bug with frozen climbing Koopas
# daNetEnemy_c::execute() controls weather net enemies should 
# stop moving or go into the flipping state when a flip panel
# is hit, however it never checks for if the enemy is iced
# leading to the bug
.global fixIceBug # 80044994 
fixIceBug:
    lwz r0, 0x490(r3) # load dIceMg.isFrozen
    cmpwi r0, 0 # check if we're not frozen
    bne isIced
    lwzu r12, 0x394(r3) #original instruction
    b continueWithNetExecute

isIced:
    b dontFlip