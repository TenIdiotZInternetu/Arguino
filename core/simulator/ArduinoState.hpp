//
// Created by TIZI on 08/12/2024.
//

#ifndef ARGUINO_PINOUTSTATE_HPP
#define ARGUINO_PINOUTSTATE_HPP

#include <array>
#include <atomic>
#include <functional>
#include <mutex>

#include "EventQueue.hpp"
#include "ILogger.hpp"
#include "SimulatorTypes.hpp"
#include "Timer.hpp"

namespace arguino::simulator {

class ArduinoState {
   public:
    static constexpr uint8_t ANALOG_PIN_COUNT = 6;
    static constexpr uint8_t DIGITAL_PIN_COUNT = 14;
    static constexpr uint8_t PIN_COUNT = ANALOG_PIN_COUNT + DIGITAL_PIN_COUNT;

    using analog_arr_t = std::array<analog_t, ANALOG_PIN_COUNT>;
    using digital_arr_t = std::array<digital_t, DIGITAL_PIN_COUNT>;
    using pinmode_arr_t = std::array<PinMode, PIN_COUNT>;

    void init_timer();
    float get_time() { return _timer.deltaTime(); }

    analog_arr_t& get_analog() { return _analogPins; }
    const analog_arr_t& get_analog() const { return _analogPins; }
    double get_analog(pin_t pin) const;
    bool set_analog(pin_t pin, analog_t value);

    digital_arr_t& get_digital() { return _digitalPins; }
    const digital_arr_t& get_digital() const { return _digitalPins; }
    bool get_digital(pin_t pin) const;
    bool set_digital(pin_t pin, digital_t value);

    pinmode_arr_t& get_pin_mode() { return _pinModes; }
    const pinmode_arr_t& get_pin_mode() const { return _pinModes; }
    PinMode get_pin_mode(pin_t pin) const;
    bool set_pin_mode(pin_t pin, PinMode mode);

   private:
    // TODO: Implement changing clock speed. Virtual and Real Clock
    Timer _timer;

    analog_arr_t _analogPins = {};
    digital_arr_t _digitalPins = {};
    pinmode_arr_t _pinModes = {};
};

}  // namespace arguino::simulator

#endif  // ARGUINO_PINOUTSTATE_HPP