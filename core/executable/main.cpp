#include <filesystem>
#include <iostream>

#include "FileLogger.hpp"
#include "Simulator.hpp"
#include "ipcAdapter.hpp"
#include "programOptions.hpp"
#include "sketch.cpp"

using logger_t = logger::FileLogger;
using simulator_t = arguino::simulator::Simulator;

ProgramOptions options;

void run_simulator(simulator_t::event_callback_fnct eventCallback)
{
    using state_t = arguino::simulator::Simulator;
    auto logger = std::make_shared<logger_t>(std::filesystem::absolute(options.SimulatorLogPath));

    state_t::init(eventCallback);
    state_t::init_logger(logger);
    state_t::state().init_timer();

    setup();
    while (true) {
        loop();
    }
}

int main(int argc, char** argv)
{
    options = parse_arguments(argc, argv);
    setup_simulator_with_ipc(run_simulator, options);
}
