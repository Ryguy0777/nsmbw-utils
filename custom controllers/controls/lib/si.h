#pragma once

#include <controls/lib/misc.h>

#define SI_MAX_CHAN 4
#define SI_MAX_TYPE 4

#define SI_COMCSR_IDX 13
#define SI_STATUS_IDX 14

#define SI_COMCSR_TCINT_MASK       (1 << 31)
#define SI_COMCSR_TCINTMSK_MASK    (1 << 30)
#define SI_COMCSR_COMERR_MASK      (1 << 29)
#define SI_COMCSR_RDSTINT_MASK     (1 << 28)
#define SI_COMCSR_RDSTINTMSK_MASK  (1 << 27)

// 4 bits of padding
#define SI_COMCSR_OUTLNGTH_MASK    (1 << 22) \
                                 | (1 << 21) \
                                 | (1 << 20) \
                                 | (1 << 19) \
                                 | (1 << 18) \
                                 | (1 << 17) \
                                 | (1 << 16)

// 1 bit of padding
#define SI_COMCSR_INLNGTH_MASK     (1 << 14) \
                                 | (1 << 13) \
                                 | (1 << 12) \
                                 | (1 << 11) \
                                 | (1 << 10) \
                                 | (1 << 9)  \
                                 | (1 << 8)

// 5 bits of padding
#define SI_COMCSR_CHANNEL_MASK     (1 << 2) \
                                 | (1 << 1)

#define SI_COMCSR_TSTART_MASK      (1 << 0)

#define SI_ERROR_UNDER_RUN 0x0001
#define SI_ERROR_OVER_RUN 0x0002
#define SI_ERROR_COLLISION 0x0004
#define SI_ERROR_NO_RESPONSE 0x0008
#define SI_ERROR_WRST 0x0010
#define SI_ERROR_RDST 0x0020
#define SI_ERROR_UNKNOWN 0x0040
#define SI_ERROR_BUSY 0x0080

#define SI_TYPE_MASK 0x18000000u
#define SI_TYPE_N64 0x00000000u
#define SI_TYPE_DOLPHIN 0x08000000u
#define SI_TYPE_GC SI_TYPE_DOLPHIN

#define SI_GC_WIRELESS 0x80000000
#define SI_GC_NOMOTOR 0x20000000
#define SI_GC_STANDARD 0x01000000

#define SI_WIRELESS_RECEIVED 0x40000000
#define SI_WIRELESS_IR 0x04000000
#define SI_WIRELESS_STATE 0x02000000
#define SI_WIRELESS_ORIGIN 0x00200000
#define SI_WIRELESS_FIX_ID 0x00100000
#define SI_WIRELESS_TYPE 0x000f0000
#define SI_WIRELESS_LITE_MASK 0x000c0000
#define SI_WIRELESS_LITE 0x00040000
#define SI_WIRELESS_CONT_MASK 0x00080000
#define SI_WIRELESS_CONT 0x00000000
#define SI_WIRELESS_ID 0x00c0ff00
#define SI_WIRELESS_TYPE_ID (SI_WIRELESS_TYPE | SI_WIRELESS_ID)

#define SI_N64_CONTROLLER (SI_TYPE_N64 | 0x05000000)
#define SI_N64_MIC (SI_TYPE_N64 | 0x00010000)
#define SI_N64_KEYBOARD (SI_TYPE_N64 | 0x00020000)
#define SI_N64_MOUSE (SI_TYPE_N64 | 0x02000000)
#define SI_GBA (SI_TYPE_N64 | 0x00040000)

#define SI_GC_CONTROLLER (SI_TYPE_GC | SI_GC_STANDARD)
#define SI_GC_RECEIVER (SI_TYPE_GC | SI_GC_WIRELESS)
#define SI_GC_WAVEBIRD (SI_TYPE_GC | SI_GC_WIRELESS | SI_GC_STANDARD | SI_WIRELESS_STATE | SI_WIRELESS_FIX_ID)
#define SI_GC_KEYBOARD (SI_TYPE_GC | 0x00200000)
#define SI_GC_STEERING (SI_TYPE_GC | 0x00000000)

typedef void (*SICallback)(s32, u32, void*);
typedef void (*SITypeCallback)(s32 chan, u32 type);

struct SIControl {
  s32 chan;
  u32 poll;
  u32 inputBytes;
  void* input;
  SICallback callback;
};

struct SIPacket {
  s32 chan;
  void* output;
  u32 outputBytes;
  void* input;
  u32 inputBytes;
  SICallback callback;
  OSTime fire;
};

BOOL SIBusy();
BOOL SIIsChanBusy(s32 chan);
BOOL SIUnregisterPollingHandler(__OSInterruptHandler handler);
u32 SIGetStatus(s32 chan);
void SISetCommand(s32 chan, u32 command);
void SITransferCommands();
u32 SIEnablePolling(u32 poll);
u32 SIDisablePolling(u32 poll);

/* 0x801BB390 */
extern "C" bool SITransfer(
  s32 chan, void* output, u32 outputBytes, void* input, u32 inputBytes, SICallback callback, OSTime delay
);

/* 0x801BBA60 */
extern "C" void SIRefreshSamplingRate();

bool SIGetResponse(s32 chan, void* data);

/* 0x801BB7C0 */
extern "C" u32 SIGetType(s32 chan);

u32 SIGetTypeAsync(s32 chan, SITypeCallback callback);

u32 SIDecodeType(u32 type);

u32 SIProbe(u32 chan);