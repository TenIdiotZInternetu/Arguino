#ifndef ARGUINO_EVENTS_HPP
#define ARGUINO_EVENTS_HPP

#include <chrono>
#include <functional>
#include <variant>

#include "SimulatorTypes.hpp"

namespace arguino::simulator {

struct Event {
   public:
    enum class Type { Write, PinMode };

    Type type;
    int64_t timestampMicros;
    size_t localVirtualTime;
    std::function<void()> action;
    std::function<void()> reverseAction;

    // TODO: Use std::variant instead
    static constexpr int MAX_ARGS = 3;
    std::array<int, MAX_ARGS> args;

    template <typename... Args>
    Event(Type type, Args... arguments);

    size_t id() const { return _id; }

    static Event write(pin_t pin, digital_t value);
    static Event set_pinmode(pin_t pin, PinMode mode);
    // TODO: Jump and reboot events

   private:
    static size_t s_nextEventId;
    size_t _id;
};

}  // namespace arguino::simulator


#endif