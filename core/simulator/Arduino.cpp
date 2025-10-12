//
// Created by touster on 28. 9. 2025.
//

#ifndef ARGUINO_ARDUINO_HPP
#define ARGUINO_ARDUINO_HPP

#include "Arduino.h"

#include <iostream>

void digitalWrite(uint8_t pin, uint8_t val) {
    std::cout << "pin: " << pin << ", val: " << val;
}



#endif //ARGUINO_ARDUINO_HPP
