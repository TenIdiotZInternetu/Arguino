//
// Created by touster on 28. 9. 2025.
//

#ifndef ARGUINO_ARDUINO_HPP
#define ARGUINO_ARDUINO_HPP

#include "Arduino.h"

#include "ArduinoState.hpp"

void digitalWrite(uint8_t pin, uint8_t val) {
    G_ARDUINO_STATE.set_digital(pin, val == HIGH);
}

unsigned long millis() {
    return std::floor(G_ARDUINO_STATE.get_time() / 1000);
}

void pinMode(uint8_t pin, uint8_t mode) {
    if (mode == OUTPUT) {
        G_ARDUINO_STATE.set_pin_mode(pin, PinMode::Out);
    }
    if (mode == INPUT) {
        G_ARDUINO_STATE.set_pin_mode(pin, PinMode::In);
    }
}
#endif //ARGUINO_ARDUINO_HPP
