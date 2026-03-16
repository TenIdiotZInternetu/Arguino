#ifndef ARGUINO_SIMULATOR_HPP
#define ARGUINO_SIMULATOR_HPP

#include <atomic>
#include <memory>

#include "ArduinoState.hpp"
#include "EventQueue.hpp"
#include "ILogger.hpp"

namespace arguino::simulator {

class Simulator {
   public:
    static void init();
    static void init(void* address);

    static ArduinoState& state() { return s_instance->_states[s_instance->_readStateIdx]; }
    static void update_state(const ArduinoState& newState);

    static EventQueue& queue() { return s_instance->_eventQueue; }

    template <logger::ILogger TLogger>
    static void init_logger(std::shared_ptr<TLogger> logger_ptr);
    static void log(const std::string& str) { s_log_func(str); }

   private:
    static Simulator* s_instance;
    ArduinoState _states[2];  // one for reading, one for writing
    std::atomic<uint8_t> _readStateIdx = 0;

    EventQueue _eventQueue;

    ArduinoState& get_write_state() { return _states[1 - _readStateIdx]; }
    void flip_states() { _readStateIdx = 1 - _readStateIdx; }

    static std::function<void(const std::string&)> s_log_func;
};

template <logger::ILogger TLogger>
inline void Simulator::init_logger(std::shared_ptr<TLogger> logger_ptr)
{
    s_log_func = [=](const std::string& str) {
        if (!logger_ptr) {
            return;
        }

        logger_ptr->log(str);
    };
}

}  // namespace arguino::simulator


#endif