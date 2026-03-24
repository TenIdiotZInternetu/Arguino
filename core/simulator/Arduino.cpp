//
// Created by touster on 28. 9. 2025.
//

#ifndef ARGUINO_ARDUINO_HPP
#define ARGUINO_ARDUINO_HPP

#include "Events.hpp"
#include "Simulator.hpp"

// Arduino.h must be the last included header because it defines macros like INPUT and OUTPUT that
// spoil windows.h

#include "Arduino.h"

using namespace arguino::simulator;

void digitalWrite(uint8_t pin, uint8_t val)
{
    if (Simulator::state().get_digital(pin) != val) {
        Simulator::handle_event(            //
            Event::write(pin, val == HIGH)  //
        );
        Simulator::log(std::format("Written value {} to pin {}", val, pin));
    }
}

int digitalRead(uint8_t pin)
{
    return Simulator::state().get_digital(pin);
}

unsigned long millis()
{
    return std::floor(Simulator::state().get_time() / 1000);
}

void pinMode(uint8_t pin, uint8_t mode)
{
    if (mode == OUTPUT) {
        Simulator::state().set_pin_mode(pin, PinMode::Out);
    }
    if (mode == INPUT) {
        Simulator::state().set_pin_mode(pin, PinMode::In);
    }
}
#endif  // ARGUINO_ARDUINO_HPP
