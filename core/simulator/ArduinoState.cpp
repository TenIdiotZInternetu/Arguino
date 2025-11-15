//
// Created by TIZI on 08/12/2024.
//

#include "ArduinoState.hpp"

ArduinoState* G_ARDUINO_STATE_PTR = nullptr;

bool ArduinoState::set_digital(pin_t pin, digital_t value)
{
    _digitalPins[pin] = value;
    return true;
}

bool ArduinoState::set_pin_mode(pin_t pin, PinMode mode)
{
    _pinModes[pin] = mode;
    return true;
}
