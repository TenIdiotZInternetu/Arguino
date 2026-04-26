#include <filesystem>
#include <iostream>

#include "FileLogger.hpp"
#include "Simulator.hpp"
#include "ipcAdapter.hpp"
#include "programOptions.hpp"
#include "sketch.cpp"

using logger_t = logger::FileLogger;
using namespace arguino::simulator;

ProgramOptions options;

void init_simulator(Simulator::event_callback_fnct eventCallback)
{
    auto logPath = std::filesystem::absolute(options.SimulatorLogPath);
    auto logger = std::make_shared<logger_t>(logPath);

    Simulator::init(eventCallback, logger);

    logger->timerFunction = Simulator::simulation_time;
    logger->verbosityLevel = options.Verbosity;
}

void simulator_loop()
{
    Simulator::state().init_timer();
    Simulator::log_info("Simulator initialized.");
    setup();
    Simulator::log_info("setup() finished successfully. Running loop().");

    while (true) {
        loop();
    }
}

int main(int argc, char** argv)
{
    options = parse_arguments(argc, argv);
    init_simulator(on_event);
    run_simulator_with_ipc(simulator_loop, options);
}
