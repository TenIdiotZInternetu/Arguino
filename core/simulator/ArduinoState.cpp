//
// Created by TIZI on 08/12/2024.
//

#include "ArduinoState.hpp"

CanonicalState* CanonicalState::s_instance;
std::function<void(const std::string&)> CanonicalState::s_log_func = [](const std::string&) {};


bool ArduinoState::get_digital(pin_t pin) const
{
    return _digitalPins[pin];
}

bool ArduinoState::set_digital(pin_t pin, digital_t value)
{
    _digitalPins[pin] = value;
    return true;
}

PinMode ArduinoState::get_pin_mode(pin_t pin) const
{
    return _pinModes[pin];
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

// update_start is expected to be called from a single thread
void CanonicalState::update_state(const ArduinoState& newState)
{
    ArduinoState& writeState = s_instance->get_write_state();
    writeState.get_pin_mode() = state().get_pin_mode();
    writeState.get_digital() = state().get_digital();

    for (ArduinoState::pin_t i = 0; i < ArduinoState::DIGITAL_PIN_COUNT; i++) {
        if (writeState.get_pin_mode(i) == PinMode::In) {
            writeState.set_digital(i, newState.get_digital(i));
        }
    }

    s_instance->flip_states();
}