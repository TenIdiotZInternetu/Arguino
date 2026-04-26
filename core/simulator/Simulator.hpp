#ifndef ARGUINO_SIMULATOR_HPP
#define ARGUINO_SIMULATOR_HPP

#include <atomic>
#include <memory>

#include "ArduinoState.hpp"
#include "EventQueue.hpp"
#include "Events.hpp"
#include "ILogger.hpp"

namespace arguino::simulator {

class Simulator {
   public:
    using event_callback_fnct = std::function<void(const Event&)>;
    static void init(event_callback_fnct eventCallback);

    static ArduinoState& state() { return s_instance->_state; }
    static EventQueue& queue() { return s_instance->_eventQueue; }

    static void handle_event(Event event);
    static void handle_events();

    template <logger::ILogger TLogger>
    static void init_logger(std::shared_ptr<TLogger> loggerPtr);
    static void log(std::string&& str) { s_instance->f_log(std::move(str)); }

   private:
    static Simulator* s_instance;

    event_callback_fnct f_eventCallback;
    std::function<void(std::string&&)> f_log;

    ArduinoState _state;
    EventQueue _eventQueue;
};

template <logger::ILogger TLogger>
inline void Simulator::init_logger(std::shared_ptr<TLogger> loggerPtr)
{
    s_instance->f_log = [=](std::string&& str) {
        if (!loggerPtr) {
            return;
        }

        loggerPtr->log(std::move(str));
    };
}

}  // namespace arguino::simulator


#endif