//
// Created by TIZI on 08/12/2024.
//

#include "ArduinoState.hpp"

namespace arguino::simulator {
std::shared_ptr<Timer> ArduinoState::init_timer()
{
    _timer->step();
    return _timer;
}


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

}  // namespace arguino::simulator