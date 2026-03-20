#include "Events.hpp"

#include "Simulator.hpp"

namespace arguino::simulator {

size_t Event::s_nextEventId = 1;

// TODO: Reset timer for reverse actions

Event::Event()
    : _id(s_nextEventId)
{
    ++s_nextEventId;
}

Event Event::write(pin_t pin, digital_t value)
{
    Event event;
    event.action = [=]() { Simulator::state().set_digital(pin, value); };
    event.reverseAction = [=]() { Simulator::state().set_digital(pin, !value); };
    return event;
}

Event Event::set_pinmode(pin_t pin, PinMode mode)
{
    PinMode oppositeMode = mode == PinMode::In  //
        ? PinMode::Out
        : PinMode::In;

    Event event;
    event.action = [=]() { Simulator::state().set_pin_mode(pin, mode); };
    event.reverseAction = [=]() { Simulator::state().set_pin_mode(pin, oppositeMode); };
    return event;
}

}  // namespace arguino::simulator