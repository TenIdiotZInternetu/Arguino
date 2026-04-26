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
    auto logPath = std::filesystem::absolute(options.SimulatorLogPath);
    auto logger = std::make_shared<logger_t>(logPath);

    simulator_t::init(eventCallback, logger);
    auto timer = simulator_t::state().init_timer();

    logger->timer = timer;
    logger->verbosityLevel = options.Verbosity;

    logger->log_info("Simulator initialized.");

    setup();
    logger->log_info("setup() finished successfully, starting loop().");

    while (true) {
        loop();
    }
}

int main(int argc, char** argv)
{
    options = parse_arguments(argc, argv);
    setup_simulator_with_ipc(run_simulator, options);
}
