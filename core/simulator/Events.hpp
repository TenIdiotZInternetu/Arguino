#ifndef ARGUINO_EVENTS_HPP
#define ARGUINO_EVENTS_HPP

#include <functional>

#include "SimulatorTypes.hpp"

namespace arguino::simulator {

struct Event {
   public:
    size_t localVirtualTime;
    std::function<void()> action;
    std::function<void()> reverseAction;

    Event();

    size_t id();

    static Event write(pin_t pin, digital_t value);
    static Event set_pinmode(pin_t pin, PinMode mode);
    // TODO: Jump and reboot events

   private:
    static size_t s_nextEventId;
    size_t _id;
};

}  // namespace arguino::simulator


#endif