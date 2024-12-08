//
// Created by TIZI on 08/12/2024.
//

#ifndef ARGUINO_PINOUTSTATE_HPP
#define ARGUINO_PINOUTSTATE_HPP

#include <array>

class PinoutState {
private:
    using uint = unsigned int;
public:
    double GetAnalgo(uint pin);
    bool SetAnalog(uint pin, bool value);
    bool GetDigital(uint pin);
    bool SetDigital(uint pin, bool);
private:
    static constexpr uint ANALOG_PIN_COUNT = 6;
    static constexpr uint DIGITAL_PIN_COUNT = 14;

    std::array<double, ANALOG_PIN_COUNT> _analogPins;
    std::array<bool, DIGITAL_PIN_COUNT> _digitalPins;
};


#endif //ARGUINO_PINOUTSTATE_HPP
