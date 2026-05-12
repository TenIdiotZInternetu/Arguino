#ifndef ARGUINO_IPC_ADAPTER_HPP
#define ARGUINO_IPC_ADAPTER_HPP

#include <functional>

#include "Events.hpp"
#include "FileLogger.hpp"
#include "Simulator.hpp"
#include "programOptions.hpp"

using logger_t = logger::FileLogger;

using init_fnct = std::function<void(arguino::simulator::Simulator::event_callback_fnct)>;
using loop_fnct = std::function<void()>;

static std::shared_ptr<logger_t> IpcLogger;

void run_simulator_with_ipc(loop_fnct loopFunction, const ProgramOptions& options);

void on_event(const arguino::simulator::Event& event);

inline void init_ipc_logger(const ProgramOptions& options)
{
    auto loggerAbsolutePath = std::filesystem::absolute(options.IcpLogPath);
    IpcLogger = std::make_shared<logger_t>(loggerAbsolutePath);

    IpcLogger->verbosityLevel = options.Verbosity;
    IpcLogger->timerFunction = arguino::simulator::Simulator::real_time;
}

#endif