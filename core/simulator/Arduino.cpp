//
// Created by touster on 28. 9. 2025.
//

#ifndef ARGUINO_ARDUINO_HPP
#define ARGUINO_ARDUINO_HPP

#include "ArduinoState.hpp"

// Arduino.h must be the last included header because it defines macros like INPUT and OUTPUT that
// spoil windows.h

#include "Arduino.h"

void digitalWrite(uint8_t pin, uint8_t val)
{
    CanonicalState::state().set_digital(pin, val == HIGH);
}

int digitalRead(uint8_t pin)
{
    return CanonicalState::state().get_digital(pin);
}

unsigned long millis()
{
    return std::floor(CanonicalState::state().get_time() / 1000);
}

void pinMode(uint8_t pin, uint8_t mode)
{
    if (mode == OUTPUT) {
        CanonicalState::state().set_pin_mode(pin, PinMode::Out);
    }
    if (mode == INPUT) {
        CanonicalState::state().set_pin_mode(pin, PinMode::In);
    }
}
#endif  // ARGUINO_ARDUINO_HPP
