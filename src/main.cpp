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
    Button modeBtn(PIN_MODE_BUTTON);

    bool mode = 0;                                      // 0 for horizontal mode and 1 for vertical mode

    gpio_init(PIN_HORIZONTAL_MODE_LED);
    gpio_set_dir(PIN_HORIZONTAL_MODE_LED, GPIO_OUT);
    gpio_init(PIN_VERTICAL_MODE_LED);
    gpio_set_dir(PIN_VERTICAL_MODE_LED, GPIO_OUT);


    usb.init();

    while (true)
    {
        usb.task();

        if(leftBtn.isPressed())
        {
            printf("Left\n");
            //usb.sendKey(HID_KEY_A);
        }

        if(rightBtn.isPressed())
        {
            printf("Right\n");
            //usb.sendKey(HID_KEY_B);
        }

        if(modeBtn.isPressed())
        {
            mode = !mode;

            if(mode == 0)
            {
                gpio_put(PIN_HORIZONTAL_MODE_LED, 1);
                gpio_put(PIN_VERTICAL_MODE_LED, 0);
            }
            else
            {
                gpio_put(PIN_HORIZONTAL_MODE_LED, 0);
                gpio_put(PIN_VERTICAL_MODE_LED, 1);
            }
            printf("Mode\n");
        }
    }
}