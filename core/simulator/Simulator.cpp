#include "Simulator.hpp"

namespace arguino::simulator {

Simulator* Simulator::s_instance;
std::function<void(const std::string&)> Simulator::s_log_func = [](const std::string&) {};

void Simulator::init()
{
    s_instance = new Simulator();
}

void Simulator::init(void* address)
{
    s_instance = new (address) Simulator();
}

// update_start is expected to be called from a single thread
void Simulator::update_state(const ArduinoState& newState)
{
    ArduinoState& writeState = s_instance->get_write_state();
    writeState.get_pin_mode() = state().get_pin_mode();
    writeState.get_digital() = state().get_digital();

    for (pin_t i = 0; i < ArduinoState::DIGITAL_PIN_COUNT; i++) {
        if (writeState.get_pin_mode(i) == PinMode::In) {
            writeState.set_digital(i, newState.get_digital(i));
        }
    }

    s_instance->flip_states();
}

};