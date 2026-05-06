#ifndef ARGUINO_IPC_ADAPTER_HPP
#define ARGUINO_IPC_ADAPTER_HPP

#include <functional>

#include "Events.hpp"
#include "Simulator.hpp"
#include "programOptions.hpp"

using init_fnct = std::function<void(arguino::simulator::Simulator::event_callback_fnct)>;
using loop_fnct = std::function<void()>;

void run_simulator_with_ipc(loop_fnct loopFunction, const ProgramOptions& options);

void on_event(const arguino::simulator::Event& event);

#endif