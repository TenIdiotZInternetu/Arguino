#ifndef ARGUINO_EVENTS_HPP
#define ARGUINO_EVENTS_HPP

#include <functional>

#include "SimulatorTypes.hpp"

namespace arguino::simulator {

class CanonicalState;

struct Event {
    size_t localVirtualTime;
    std::function<void()> action;
    std::function<void()> reverseAction;

    static Event write(pin_t pin, digital_t value);
    static Event set_pinmode(pin_t pin, PinMode mode);
    // TODO: Jump and reboot events
};

}  // namespace arguino::simulator


#endif