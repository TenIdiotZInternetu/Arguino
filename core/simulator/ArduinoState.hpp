//
// Created by TIZI on 08/12/2024.
//

#ifndef ARGUINO_PINOUTSTATE_HPP
#define ARGUINO_PINOUTSTATE_HPP

#include <array>

namespace core::simulator {
    enum class PinMode { In, Out };

    class ArduinoState {
    private:
        using uint = unsigned int;
    public:
        double getAnalog(uint pin);
        bool getAnalog(uint pin, bool value);
        bool getDigital(uint pin);
        bool setDigital(uint pin, bool value);
        bool setPinMode(uint pin, PinMode mode);
    private:
        static constexpr uint ANALOG_PIN_COUNT = 6;
        static constexpr uint DIGITAL_PIN_COUNT = 14;
        static constexpr uint PIN_COUNT = ANALOG_PIN_COUNT + DIGITAL_PIN_COUNT;

        std::array<double, ANALOG_PIN_COUNT> _analogPins = {};
        std::array<bool, DIGITAL_PIN_COUNT> _digitalPins = {};
        std::array<PinMode,PIN_COUNT> _pinModes = {};
    };

}


#endif //ARGUINO_PINOUTSTATE_HPP
