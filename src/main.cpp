#include <cstdio>
#include "pico/stdlib.h"
#include "pico/time.h"
#include "USBinterface.h"
#include "tusb.h"
#include "Button.h"
#include "assets.h"

int main()
{
    board_init();
    stdio_init_all();
    
    USBinterface usb;
    Button leftBtn(PIN_LEFT_BUTTON, PIN_LEFT_LED);
    Button rightBtn(PIN_RIGHT_BUTTON, PIN_RIGHT_LED);

    usb.init();

    while (true)
    {
        usb.task();

        if(leftBtn.isPressed())
        {
            printf("Left\n");
            usb.sendKey(HID_KEY_A);
        }

        if(rightBtn.isPressed())
        {
            printf("Right\n");
            usb.sendKey(HID_KEY_B);
        }
    }
}