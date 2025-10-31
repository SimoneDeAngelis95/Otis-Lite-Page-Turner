#include <cstdio>
#include "pico/stdlib.h"
#include "pico/time.h"
#include "USBinterface.h"
#include "tusb.h"
#include "Button.h"
#include "assets.h"
#include "FlashStorage.h"

int main()
{
    board_init();
    stdio_init_all();
    
    USBinterface usb;
    Button leftBtn(PIN_LEFT_BUTTON, PIN_LEFT_LED);
    Button rightBtn(PIN_RIGHT_BUTTON, PIN_RIGHT_LED);
    Button modeBtn(PIN_MODE_BUTTON);

    bool mode = FlashStorage().read();                  // Read the persisted mode from flash (0=horizontal, 1=vertical)

    gpio_init(PIN_HORIZONTAL_MODE_LED);
    gpio_set_dir(PIN_HORIZONTAL_MODE_LED, GPIO_OUT);
    gpio_init(PIN_VERTICAL_MODE_LED);
    gpio_set_dir(PIN_VERTICAL_MODE_LED, GPIO_OUT);
    
    if(mode == 0)
        gpio_put(PIN_HORIZONTAL_MODE_LED, 1);
    else
        gpio_put(PIN_VERTICAL_MODE_LED, 1);


    usb.init();

    while (true)
    {
        usb.task();

        if(leftBtn.isPressed())
        {
            if(mode == 0)
            {
                usb.sendKey(HID_KEY_ARROW_LEFT);     // Horizontal mode -> left arrow key
            }
            else
            {
                usb.sendKey(HID_KEY_PAGE_UP);        // Vertical mode -> page up key
            }
        }

        if(rightBtn.isPressed())
        {
            if(mode == 0)
            {
                usb.sendKey(HID_KEY_ARROW_RIGHT);    // Horizontal mode -> right arrow key
                
            }
            else
            {
                usb.sendKey(HID_KEY_PAGE_DOWN);      // Vertical mode -> page down key
            }
        }

        if(modeBtn.isPressed())
        {
            mode = !mode;

            if(mode == 0)
            {
                gpio_put(PIN_HORIZONTAL_MODE_LED, 1);
                gpio_put(PIN_VERTICAL_MODE_LED, 0);
                FlashStorage().write(false);           // Persist horizontal mode
            }
            else
            {
                gpio_put(PIN_HORIZONTAL_MODE_LED, 0);
                gpio_put(PIN_VERTICAL_MODE_LED, 1);
                FlashStorage().write(true);            // Persist vertical mode
            }
            printf("Mode\n");
        }
    }
}