#include "../encoder.hpp"
#include "../ipcAdapter.hpp"
#include "TwoWayBuffer.hpp"

using namespace arguino::shmem;
using namespace arguino::simulator;

TwoWayBuffer* _shmem;

void run_simulator_with_ipc(loop_fnct loopFunction, const ProgramOptions& options)
{
    _shmem = new TwoWayBuffer(options.ShmemName, options.ShmemSizePages);
    loopFunction();
}

void on_event(const Event& event)
{
    auto message = encode_event(event);
    if (message == UNKNOWN_EVENT) {
        return;
    }

    _shmem->write(message + ';');
}