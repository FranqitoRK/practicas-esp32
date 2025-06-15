#ifndef DATA_H
#define DATA_H

#include <Arduino.h>

enum LedCommand {
    LED_RED,
    LED_GREEN,
    LED_BLUE,
    LED_BLINK
};

LedCommand randomCommand();

#endif 