#include "../encoder.hpp"
#include "../ipcAdapter.hpp"
#include "ProducerConsumerWrapper.hpp"

using namespace arguino::shmem;
using namespace arguino::simulator;

ProducerConsumerWrapper* _shmem;

void run_simulator_with_ipc(loop_fnct loopFunction, const ProgramOptions& options)
{
    _shmem = new ProducerConsumerWrapper(options.ShmemName, options.ShmemSizePages);
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