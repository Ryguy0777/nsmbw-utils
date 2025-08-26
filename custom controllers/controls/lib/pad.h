#pragma once

#include <controls/lib/misc.h>

// source: https://github.com/riidefi/mkw/blob/master/lib/rvl/pad/pad.h

#define PAD_MOTOR_STOP      0
#define PAD_MOTOR_RUMBLE    1
#define PAD_MOTOR_STOP_HARD 2

#define PAD_CHAN0_BIT 0x80000000
#define PAD_CHAN1_BIT 0x40000000
#define PAD_CHAN2_BIT 0x20000000
#define PAD_CHAN3_BIT 0x10000000

#define PAD_BUTTON_LEFT     0x0001
#define PAD_BUTTON_RIGHT    0x0002
#define PAD_BUTTON_DOWN     0x0004
#define PAD_BUTTON_UP       0x0008
#define PAD_TRIGGER_Z       0x0010
#define PAD_TRIGGER_R       0x0020
#define PAD_TRIGGER_L       0x0040
#define PAD_BUTTON_A        0x0100
#define PAD_BUTTON_B        0x0200
#define PAD_BUTTON_X        0x0400
#define PAD_BUTTON_Y        0x0800
#define PAD_BUTTON_START    0x1000

#define PAD_ERR_NONE           0
#define PAD_ERR_NO_CONTROLLER -1
#define PAD_ERR_NOT_READY     -2
#define PAD_ERR_TRANSFER      -3

// https://github.com/devkitPro/libogc/blob/bc4b778d558915aa40676e33514c4c9ba2af66b8/gc/ogc/pad.h#L46
// Size: 0x0B (arrays padded to 0x0C)
struct PADStatus {
    // 0x00
    u16 button;
    // 0x02
    s8 stickX;
    // 0x03
    s8 stickY;
    // 0x04
    s8 substickX;
    // 0x05
    s8 substickY;
    // 0x06
    u8 triggerL;
    // 0x07
    u8 triggerR;
    // 0x08
    u8 analogA;
    // 0x09
    u8 analogB;
    // 0x0A
    s8 err;
};

// Size: 0x0A
struct PADClampRegion {
    u8 minTrigger;
    u8 maxTrigger;
    s8 minStick;
    s8 maxStick;
    s8 xyStick;
    s8 minSubstick;
    s8 maxSubstick;
    s8 xySubstick;
    s8 radStick;
    s8 radSubstick;
};

void PADClamp(PADStatus*);
void PADClampCircle(PADStatus*);
void PAD_UpdateOrigin(s32);
void PADOriginCallback(s32, u32, void*);
void PADOriginUpdateCallback(s32, u32, void*);
void PADProbeCallback(s32, u32, void*);
void PADTypeAndStatusCallback(s32, u32);
int PADReset(u32);
int PADRecalibrate(u32);
int PADInit(void);
u32 PADRead(PADStatus*);
void PADControlMotor(s32, u32);
int PAD_OnReset(int);
void PAD_SamplingHandler(s32, OSContext2*);
int __PADDisableRecalibration(int);
// PADSetSamplingCallback

void PADSetSpec(u32 model);
u32 PADGetSpec(void);

typedef void (*PADSamplingCallback)(void);