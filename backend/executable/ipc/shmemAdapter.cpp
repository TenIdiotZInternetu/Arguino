#include "../ipcAdapter.hpp"
#include "IpcHandler.hpp"

using namespace arguino::shmem;

IpcHandler _shmem;

void run_simulator_with_ipc(loop_fnct loopFunction, const ProgramOptions& options)
{
    _shmem = IpcHandler(options.ShmemName, options.ShmemSizePages);
    loopFunction();
}

void on_event(const Event& event)
{
    auto message = encode_event(event);
    if (message == UNKNOWN_EVENT) {
        _tcpLogger->log_error("Encountered unknown event type; skipping.");
        return;
    }

    _tcpServer->post_message(message);
}