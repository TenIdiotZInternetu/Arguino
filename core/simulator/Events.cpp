#include "Events.hpp"

#include "ArduinoState.hpp"

namespace arguino::simulator {

// TODO: Reset timer for reverse actions

Event Event::write(pin_t pin, digital_t value)
{
    return {
        .action = [=]() { CanonicalState::state().set_digital(pin, value); },         //
        .reverseAction = [=]() { CanonicalState::state().set_digital(pin, !value); }  //
    };
}

Event Event::set_pinmode(pin_t pin, PinMode mode)
{
    PinMode oppositeMode = mode == PinMode::In  //
        ? PinMode::Out
        : PinMode::In;

    return {
        .action = [=]() { CanonicalState::state().set_pin_mode(pin, mode); },                //
        .reverseAction = [=]() { CanonicalState::state().set_pin_mode(pin, oppositeMode); }  //
    };
}

}  // namespace arguino::simulator