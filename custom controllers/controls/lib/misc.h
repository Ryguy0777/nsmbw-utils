#pragma once

#include <common.h>

// OSContext

struct OSContext2 {
  // General-purpose registers
  u32 gpr[32];

  u32 cr;
  u32 lr;
  u32 ctr;
  u32 xer;

  // Floating-point registers
  f64 fpr[32];

  u32 fpscr_pad;
  u32 fpscr;

  // Exception handling registers
  u32 srr0;
  u32 srr1;

  // Context mode
  u16 mode;  // since UIMM is 16 bits in PPC
  u16 state; // OR-ed OS_CONTEXT_STATE_*

  // Place Gekko regs at the end so we have minimal changes to
  // existing code
  u32 gqr[8];
  u32 psf_pad;
  f64 psf[32];
};

// OS stuff
extern "C" {

/* 0x801B1280 */
bool OSDisableInterrupts();

/* 0x801B12A0 */
bool OSEnableInterrupts();

/* 0x801B12C0 */
bool OSRestoreInterrupts(int);

/* 0x801ab300 */
void OSRegisterVersion(const char *);

/* 0x801ace80 */
void OSSetCurrentContext(OSContext2* context);

/* 0x801ad0c0 */
void OSClearContext(OSContext2* context);

typedef s64 OSTime;
typedef s32 OSTick;

/* 0x801B60C0 */
OSTime OSGetTime();

/* 0x801B60E0 */
OSTick OSGetTick();

} // extern "C"

inline s8 abs(s8 value) {
	return __fabs(value);
}
inline int abs(int value) {
	return __fabs(value);
}

// revolution/vi/vi.h

/* 0x8042AB30 */
extern s32 __VIDimmingFlag_SI_IDLE;

bool __VIResetSIIdle()
{
    __VIDimmingFlag_SI_IDLE = 0;
    return true;
}

#define cntlzw(_val) ({register u32 _rval; \
					  asm volatile("cntlzw %0, %1" : "=r"((_rval)) : "r"((_val))); _rval; })

#define __SIRegs         ((volatile u32 *)0xCD006400)

typedef void (*__OSInterruptHandler)(s16 interrupt, OSContext2* context);