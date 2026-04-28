#ifndef ARGUINO_IPC_ADAPTER_HPP
#define ARGUINO_IPC_ADAPTER_HPP

#include <functional>

#include "Simulator.hpp"
#include "programOptions.hpp"

using loop_fnct = std::function<void(arguino::simulator::Simulator::event_callback_fnct)>;

void setup_simulator_with_ipc(loop_fnct event_loop, const ProgramOptions& options);

#endif