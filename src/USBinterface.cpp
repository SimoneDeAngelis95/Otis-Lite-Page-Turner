#include "USBinterface.h"

// -----------------------------------------------------------------------------
// USB descriptors (TinyUSB expects these callbacks to return static data)
// -----------------------------------------------------------------------------
// Use an anonymous namespace so all descriptor objects stay local to this file.
namespace
{
    constexpr uint16_t kVendorId  = 0xCAFE;
    constexpr uint16_t kProductId = 0x4000;
    constexpr uint16_t kBcdDevice = 0x0100;

    enum InterfaceId : uint8_t
    {
        kInterfaceHid,
        kInterfaceCount
    };

    constexpr uint8_t kEndpointHidIn = 0x81;
    constexpr uint16_t kConfigTotalLength = TUD_CONFIG_DESC_LEN + TUD_HID_DESC_LEN;

    // HID report descriptor describing a simple boot keyboard.
    const uint8_t kHidReportDescriptor[] =
    {
        TUD_HID_REPORT_DESC_KEYBOARD()
    };

    const tusb_desc_device_t kDeviceDescriptor =
    {
        .bLength            = sizeof(tusb_desc_device_t),
        .bDescriptorType    = TUSB_DESC_DEVICE,
        .bcdUSB             = 0x0200,
        .bDeviceClass       = TUSB_CLASS_UNSPECIFIED,
        .bDeviceSubClass    = 0x00,
        .bDeviceProtocol    = 0x00,
        .bMaxPacketSize0    = CFG_TUD_ENDPOINT0_SIZE,
        .idVendor           = kVendorId,
        .idProduct          = kProductId,
        .bcdDevice          = kBcdDevice,
        .iManufacturer      = 0x01,
        .iProduct           = 0x02,
        .iSerialNumber      = 0x03,
        .bNumConfigurations = 0x01,
    };

    const uint8_t kConfigurationDescriptor[] =
    {
        TUD_CONFIG_DESCRIPTOR(1, kInterfaceCount, 0, kConfigTotalLength,
                              TUSB_DESC_CONFIG_ATT_REMOTE_WAKEUP, 100),
        TUD_HID_DESCRIPTOR(kInterfaceHid, 0, HID_ITF_PROTOCOL_KEYBOARD,
                           sizeof(kHidReportDescriptor), kEndpointHidIn,
                           CFG_TUD_HID_EP_BUFSIZE, CFG_TUD_HID_POLL_INTERVAL)
    };

    const char* const kStringDescriptors[] =
    {
        "Otis Lite",
        "USB Page Turner",
        "0001"
    };

    uint16_t const* makeStringDescriptor(uint8_t index)
    {
        static uint16_t desc_str[32];

        if (index == 0)
        {
            desc_str[0] = (TUSB_DESC_STRING << 8) | (2 + 2);
            desc_str[1] = 0x0409;
            return desc_str;
        }

        index -= 1;
        if (index >= (sizeof(kStringDescriptors) / sizeof(kStringDescriptors[0])))
        {
            return nullptr;
        }

        const char* str = kStringDescriptors[index];
        uint8_t count = static_cast<uint8_t>(strlen(str));
        if (count > 31) count = 31;

        desc_str[0] = (TUSB_DESC_STRING << 8) | (2 + count * 2);
        for (uint8_t i = 0; i < count; ++i)
        {
            desc_str[1 + i] = str[i];
        }

        return desc_str;
    }

    bool g_releasePending = false;
} // namespace

// -----------------------------------------------------------------------------
// USBinterface class implementation
// -----------------------------------------------------------------------------
USBinterface::USBinterface()
{
}

USBinterface::~USBinterface()
{
}

void USBinterface::init()
{   
    tusb_init();                                           // Bring up the TinyUSB device stack; board_init() must have run already.
}

void USBinterface::task()
{
    tud_task();                                            // Keep the handshake alive. Call frequently from the main loop. (not more than 1ms interval)
}

void USBinterface::sendKey(uint8_t keycode)
{
    if (!tud_hid_ready())                                  // Check if the host is ready to receive.
        return;

    if (g_releasePending)                                  // Avoid sending a new keypress until previous release completes.
        return;

    uint8_t keycode_array[6] = { keycode, 0, 0, 0, 0, 0 }; // a keycode array is a 6-byte array that can hold up to 6 simultaneous key presses. Standard keyboards do not support more than 6 keys pressed at once.
                                                           // Here we only send one key press at a time, so the rest of the array is filled with zeros.

                                                           // Send the key press followed by a release using an empty report.                                                    
    if (tud_hid_keyboard_report(0, 0, keycode_array))      // Send the key press.
    {
        g_releasePending = true;                           // Defer release until the host has consumed the key press.
    }
}

//--------------------------------------------------------------------+
// TinyUSB Callbacks
//--------------------------------------------------------------------+
// MUST BE HERE or nothing works
// the TinyUSB is written in C and expects these symbols to be defined with C linkage

extern "C" uint8_t const* tud_descriptor_device_cb(void)
{
    return reinterpret_cast<uint8_t const*>(&kDeviceDescriptor);
}

extern "C" uint8_t const* tud_descriptor_configuration_cb(uint8_t index)
{
    (void)index;
    return kConfigurationDescriptor;
}

extern "C" uint16_t const* tud_descriptor_string_cb(uint8_t index, uint16_t langid)
{
    (void)langid;
    return makeStringDescriptor(index);
}

extern "C" uint8_t const* tud_hid_descriptor_report_cb(uint8_t instance)
{
    (void)instance;
    return kHidReportDescriptor;
}

// TinyUSB expects these C-linkage callbacks; we provide no-op stubs for now.
extern "C" void tud_hid_report_complete_cb(uint8_t instance, uint8_t const* report, uint16_t len)
{
    (void)instance;
    (void)report;
    (void)len;

    if (g_releasePending)
    {
        static constexpr uint8_t kEmptyReport[6] = {};
        if (tud_hid_keyboard_report(0, 0, kEmptyReport))
        {
            g_releasePending = false;
        }
    }
}

// HID Descriptor callbacks
extern "C" uint16_t tud_hid_get_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t* buffer, uint16_t reqlen)
{
    (void)instance;
    (void)report_id;
    (void)report_type;
    (void)buffer;
    (void)reqlen;
    return 0;  // Returning zero signals "no report" back to the host.
}

extern "C" void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t const* buffer, uint16_t bufsize)
{
    (void)instance;
    (void)report_id;
    (void)report_type;
    (void)buffer;
    (void)bufsize;
}
