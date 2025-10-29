#pragma once
#include "pico/stdlib.h"

#define DEBOUNCE_DELAY 200   // ms

class Button
{
public:
    Button(uint btnPin, uint ledPin);
    bool isPressed();

private:
    uint btnPin;
    uint ledPin;
    bool ledState;
    bool released;

    uint64_t t;
};