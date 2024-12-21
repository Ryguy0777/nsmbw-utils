#pragma once
#include <game.h>

class daFreezer_c : public dActorState_c {
    public:
        mHeapAllocator_c allocator;
        nw4r::g3d::ResFile resFile;
        m3d::mdl_c model;
        m3d::anmTexSrt_c anmSrt;
        u8 _45C, _45D, _45E, _45F;
        Physics phys;
        u8 unk[16];
        u32 touchedByFire;
        u32 setting;
        u8 unk2[24];
        EGG::Effect _574;
        u8 unk3[16];
        EGG::Effect _698;
        u8 unk5[16];
        EGG::Effect _7D0;
        u8 unk6[16];
        EGG::Effect _8F4;
        u8 unk7[16];
        EGG::Effect _A18;
        u8 unk8[16];
        EGG::Effect _B3C;
        u8 unk9[16];
        EGG::Effect _C60;
};