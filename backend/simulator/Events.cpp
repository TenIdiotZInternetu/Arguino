#include "Events.hpp"

#include "Simulator.hpp"

namespace arguino::simulator {

size_t Event::s_nextEventId = 1;

template <typename... Args>
Event::Event(Type type, Args... arguments)
    : type(type), timestampMicros(Simulator::state().get_time()), _id(s_nextEventId)
{
    static_assert(sizeof...(arguments) <= MAX_ARGS);
    args = { static_cast<int>(arguments)... };
    ++s_nextEventId;
}

std::string type_string(Event::Type type)
{
    switch (type) {
        case Event::Type::Write:
            return "Write";
        case Event::Type::PinMode:
            return "Pin";
        default:
            return "Unknown";
    }
}

std::string Event::to_string()
{
    return std::format("{}[id:{} lvt:{} args:{} {} {}]",
        type_string(type),
        _id,
        localVirtualTime,
        args[0],
        args[1],
        args[2]);
}


Event Event::write(pin_t pin, digital_t value)
{
    Event event(Type::Write, pin, value);
    event.action = [=]() {
        Simulator::state().set_digital(pin, value);
        Simulator::log_info(std::format("Written value {} to pin {}", value, pin));
    };

    event.reverseAction = [=]() { Simulator::state().set_digital(pin, !value); };
    return event;
}

Event Event::set_pinmode(pin_t pin, PinMode mode)
{
    PinMode oppositeMode = mode == PinMode::In  //
        ? PinMode::Out
        : PinMode::In;

    Event event(Type::PinMode, pin, mode);
    event.action = [=]() {
        Simulator::state().set_pin_mode(pin, mode);
        Simulator::log_info(std::format("Set pin mode to {} on pin {}", pinmode_to_str(mode), pin));
    };

    event.reverseAction = [=]() { Simulator::state().set_pin_mode(pin, oppositeMode); };
    return event;
}

}  // namespace arguino::simulator