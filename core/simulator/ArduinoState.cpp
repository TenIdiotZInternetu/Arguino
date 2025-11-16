//
// Created by TIZI on 08/12/2024.
//

#include "ArduinoState.hpp"

CanonicalState* CanonicalState::s_instance;

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

void CanonicalState::init()
{
    s_instance = new CanonicalState();
}

void CanonicalState::init(void* address)
{
    s_instance = new (address) CanonicalState();
}

void CanonicalState::update_state(const ArduinoState& newState)
{
    uint8_t writeState = 1 - s_instance->_readState;
    s_instance->_states[writeState] = newState;
    s_instance->_readState = writeState;
}
