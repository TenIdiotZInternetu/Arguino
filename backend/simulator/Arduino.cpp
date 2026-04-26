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
    if (Simulator::state().get_digital(pin) == val) return;

    Simulator::handle_event(            //
        Event::write(pin, val == HIGH)  //
    );
}

int digitalRead(uint8_t pin)
{
    // TODO: Create a synchronization barrier before read.
    // This is the only reasonable place where can the code branch.
    // Implement confirmation event produced by remote process, that will unblock this call.
    Simulator::handle_events();
    return Simulator::state().get_digital(pin);
}

unsigned long millis()
{
    return std::floor(Simulator::state().get_time() / 1000);
}

void pinMode(uint8_t pin, uint8_t mode)
{
    PinMode pinMode = mode == OUTPUT ? PinMode::Out : PinMode::In;
    if (Simulator::state().get_pin_mode(pin) == pinMode) return;

    Simulator::handle_event(              //
        Event::set_pinmode(pin, pinMode)  //
    );
}
#endif  // ARGUINO_ARDUINO_HPP
