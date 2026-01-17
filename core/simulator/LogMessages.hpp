#include <format>
#include <string>

#include "ArduinoState.hpp"
#include "ILogger.hpp"

namespace arguino::simulator::message {

class Read {
   public:
    Read(ArduinoState::pin_t pin, ArduinoState::digital_t value)
        : _pin(pin), _value(value)
    {}

    std::string type() { return "ARDUINO:READ"; }
    std::string what() { return std::format("Reading value {} from pin {}", _pin, _value); }

   private:
    ArduinoState::pin_t _pin;
    ArduinoState::digital_t _value;
};

class Write {
   public:
    Write(ArduinoState::pin_t pin, ArduinoState::digital_t value)
        : _pin(pin), _value(value)
    {}

    std::string type() { return "ARDUINO:WRITE"; }
    std::string what() { return std::format("Writing value {} to pin {}", _pin, _value); }

   private:
    ArduinoState::pin_t _pin;
    ArduinoState::digital_t _value;
};
}  // namespace arguino::simulator::message