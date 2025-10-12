//
// Created by touster on 28. 9. 2025.
//

#ifndef ARGUINO_ARDUINO_HPP
#define ARGUINO_ARDUINO_HPP

#include "Arduino.h"

#include "ArduinoState.hpp"

namespace core::simulator {
    void digitalWrite(uint8_t pin, uint8_t val) {
        G_ARDUINO_STATE.set_digital(pin, val == HIGH);
    }


}



#endif //ARGUINO_ARDUINO_HPP
