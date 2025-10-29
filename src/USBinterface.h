#pragma once
#include <string.h>
#include "bsp/board.h"
#include "tusb.h"

class USBinterface
{
public:
    USBinterface();
    ~USBinterface();

    void init();                         // Initialize the TinyUSB device stack; call once after the board is ready.
    void task();                         // Pump TinyUSB's internal state machine; invoke regularly from the main loop to maintain connectivity.
    void sendKey(uint8_t keycode);       // Send a single HID key press (press + release) to the connected host.
};
