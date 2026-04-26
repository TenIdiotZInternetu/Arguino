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
    using logger_ptr = std::shared_ptr<logger::ILogger>;

    static void init(event_callback_fnct eventCallback, logger_ptr logger = nullptr);

    static ArduinoState& state() { return s_instance->_state; }
    static EventQueue& queue() { return s_instance->_eventQueue; }

    static void handle_event(Event event);
    static void handle_events();

    static void log_debug(std::string&& str) { s_instance->_logger->log_debug(std::move(str)); }
    static void log_info(std::string&& str) { s_instance->_logger->log_info(std::move(str)); }
    static void log_warning(std::string&& str) { s_instance->_logger->log_warning(std::move(str)); }
    static void log_error(std::string&& str) { s_instance->_logger->log_error(std::move(str)); }

   private:
    static Simulator* s_instance;

    event_callback_fnct f_eventCallback;
    logger_ptr _logger;

    ArduinoState _state;
    EventQueue _eventQueue;
};

}  // namespace arguino::simulator


#endif