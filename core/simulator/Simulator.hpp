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
    static void init(std::function<void(const Event&)> eventCallback);

    static ArduinoState& state() { return s_instance->_state; }
    static EventQueue& queue() { return s_instance->_eventQueue; }

    static void handle_event(Event event);

    template <logger::ILogger TLogger>
    static void init_logger(std::shared_ptr<TLogger> loggerPtr);
    static void log(const std::string& str) { s_instance->f_log(str); }

   private:
    static Simulator* s_instance;

    std::function<void(const std::string&)> f_log;
    std::function<void(const Event&)> f_eventCallback;

    ArduinoState _state;
    EventQueue _eventQueue;
};

template <logger::ILogger TLogger>
inline void Simulator::init_logger(std::shared_ptr<TLogger> loggerPtr)
{
    s_instance->f_log = [=](const std::string& str) {
        if (!loggerPtr) {
            return;
        }

        loggerPtr->log(str);
    };
}

}  // namespace arguino::simulator


#endif