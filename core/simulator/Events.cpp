#include "Events.hpp"

#include "Simulator.hpp"

namespace arguino::simulator {

size_t Event::s_nextEventId = 1;

// TODO: Reset timer for reverse actions

template <typename... Args>
Event::Event(Type type, Args... arguments)
    : type(type), timestampMicros(Simulator::state().get_time()), _id(s_nextEventId)
{
    static_assert(sizeof...(arguments) <= MAX_ARGS);
    args = { static_cast<int>(arguments)... };
    ++s_nextEventId;
}

Event Event::write(pin_t pin, digital_t value)
{
    Event event(Type::Write, pin, value);
    event.action = [=]() { Simulator::state().set_digital(pin, value); };
    event.reverseAction = [=]() { Simulator::state().set_digital(pin, !value); };
    return event;
}

Event Event::set_pinmode(pin_t pin, PinMode mode)
{
    PinMode oppositeMode = mode == PinMode::In  //
        ? PinMode::Out
        : PinMode::In;

    Event event(Type::PinMode, pin, mode);
    event.action = [=]() { Simulator::state().set_pin_mode(pin, mode); };
    event.reverseAction = [=]() { Simulator::state().set_pin_mode(pin, oppositeMode); };
    return event;
}

}  // namespace arguino::simulator