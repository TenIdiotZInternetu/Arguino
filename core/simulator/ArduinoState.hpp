//
// Created by TIZI on 08/12/2024.
//

#ifndef ARGUINO_PINOUTSTATE_HPP
#define ARGUINO_PINOUTSTATE_HPP

#include <array>
#include <atomic>
#include <mutex>

#include "Timer.hpp"

enum class PinMode { In, Out };

class ArduinoState {
   public:
    using pin_t = uint8_t;
    using digital_t = bool;
    using analog_t = float;

    static constexpr uint8_t ANALOG_PIN_COUNT = 6;
    static constexpr uint8_t DIGITAL_PIN_COUNT = 14;
    static constexpr uint8_t PIN_COUNT = ANALOG_PIN_COUNT + DIGITAL_PIN_COUNT;

    using analog_arr_t = std::array<analog_t, ANALOG_PIN_COUNT>;
    using digital_arr_t = std::array<digital_t, DIGITAL_PIN_COUNT>;
    using pinmode_arr_t = std::array<PinMode, PIN_COUNT>;

    void init_timer() { _timer.step(); }
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
    Timer _timer;

    analog_arr_t _analogPins = {};
    digital_arr_t _digitalPins = {};
    pinmode_arr_t _pinModes = {};
};

class CanonicalState {
   public:
    static void init();
    static void init(void* address);

    static ArduinoState& state() { return s_instance->_states[s_instance->_readStateIdx]; }
    static void update_state(const ArduinoState& newState);

   private:
    static CanonicalState* s_instance;
    ArduinoState _states[2];  // one for reading, one for writing
    std::atomic<uint8_t> _readStateIdx = 0;

    ArduinoState& get_write_state() { return _states[1 - _readStateIdx]; }
    void flip_states() { _readStateIdx = 1 - _readStateIdx; }
};


#endif  // ARGUINO_PINOUTSTATE_HPP
