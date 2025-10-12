//
// Created by TIZI on 08/12/2024.
//

#ifndef ARGUINO_PINOUTSTATE_HPP
#define ARGUINO_PINOUTSTATE_HPP

#include <array>

namespace core::simulator {
    enum class PinMode { In, Out };

    class ArduinoState {
    public:
        using uint = unsigned int;
        using pin_t = uint;
        using digital_t = bool;
        using analog_t = float;

        static constexpr uint ANALOG_PIN_COUNT = 6;
        static constexpr uint DIGITAL_PIN_COUNT = 14;
        static constexpr uint PIN_COUNT = ANALOG_PIN_COUNT + DIGITAL_PIN_COUNT;

        using analog_arr_t = std::array<analog_t, ANALOG_PIN_COUNT>;
        using digital_arr_t = std::array<digital_t, DIGITAL_PIN_COUNT>;
        using pinmode_arr_t = std::array<PinMode, PIN_COUNT>;

        analog_arr_t& get_analog() {return _analogPins; }
        double get_analog(pin_t pin);
        bool get_analog(pin_t pin, analog_t value);

        digital_arr_t& get_digital() { return _digitalPins; }
        bool get_digital(pin_t pin);
        bool set_digital(pin_t pin, digital_t value);

        pinmode_arr_t& get_pin_mode() { return _pinModes; }
        PinMode get_pin_mode(pin_t pin);
        bool set_pin_mode(pin_t pin, PinMode mode);

    private:
        analog_arr_t _analogPins = {};
        digital_arr_t _digitalPins = {};
        pinmode_arr_t _pinModes = {};
    };

    static ArduinoState G_ARDUINO_STATE;
}


#endif //ARGUINO_PINOUTSTATE_HPP
