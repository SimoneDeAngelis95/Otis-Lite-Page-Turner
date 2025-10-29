#include "Button.h"

Button::Button(uint btnPin, uint ledPin)
{
    this->btnPin = btnPin;
    this->ledPin = ledPin;
    t = 0;
    ledState = false;
    released = true;

    gpio_init(btnPin);
    gpio_set_dir(btnPin, GPIO_IN);

    gpio_init(ledPin);
    gpio_set_dir(ledPin, GPIO_OUT);
}

bool Button::isPressed()
{
    uint64_t currentTime = to_ms_since_boot(get_absolute_time()); // Get current time in milliseconds, not microseconds
    if ((currentTime - t) > DEBOUNCE_DELAY)
    {   
        if(gpio_get(btnPin))
        {
            if(released)
            {
                t = currentTime;
                released = false;
                return true;
            }
        }
        else
        {
            if(!released)
                released = true;
        }
    }
    
    return false;
}