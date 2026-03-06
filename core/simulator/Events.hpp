#ifndef ARGUINO_EVENTS_HPP
#define ARGUINO_EVENTS_HPP

#include <functional>

#include "ArduinoState.hpp"

namespace arguino::simulator {

struct Event {
    size_t localVirtualTime;
    std::function<void()> action;
    std::function<void()> reverseAction;
};

namespace event {

// TODO: Reset timer for reverse actions

Event write(pin_t pin, digital_t value)
{
    return {
        .action = [=]() { CanonicalState::state().set_digital(pin, value); },         //
        .reverseAction = [=]() { CanonicalState::state().set_digital(pin, !value); }  //
    };
}

Event set_pinmode(pin_t pin, PinMode mode)
{
    PinMode oppositeMode = mode == PinMode::In  //
        ? PinMode::Out
        : PinMode::In;

    return {
        .action = [=]() { CanonicalState::state().set_pin_mode(pin, mode); },                //
        .reverseAction = [=]() { CanonicalState::state().set_pin_mode(pin, oppositeMode); }  //
    };
}

// TODO: Jump and reboot events

}  // namespace event

}  // namespace arguino::simulator


#endif