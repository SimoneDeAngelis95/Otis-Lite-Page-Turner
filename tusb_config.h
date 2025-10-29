#pragma once

#ifdef __cplusplus
extern "C" {
#endif

// Root hub configuration: single device port running at default speed.
#define CFG_TUSB_RHPORT0_MODE      (OPT_MODE_DEVICE | OPT_MODE_FULL_SPEED)
#define CFG_TUSB_RHPORT0_MAX_SPEED OPT_MODE_FULL_SPEED

// Endpoint zero size (must be 8, 16, 32 or 64 for full speed devices).
#define CFG_TUD_ENDPOINT0_SIZE     64

// Memory alignment helpers for DMA transfers.
#define CFG_TUSB_MEM_SECTION
#define CFG_TUSB_MEM_ALIGN         __attribute__((aligned(4)))

// Enable only the HID class – everything else stays disabled by default.
#define CFG_TUD_HID                1
#define CFG_TUD_HID_EP_BUFSIZE     16
#define CFG_TUD_HID_POLL_INTERVAL  5

#define CFG_TUD_CDC                0
#define CFG_TUD_MSC                0
#define CFG_TUD_MIDI               0
#define CFG_TUD_VENDOR             0
#define CFG_TUD_ECM_RNDIS          0
#define CFG_TUD_DFU                0
#define CFG_TUD_BTH                0

#ifdef __cplusplus
}
#endif
