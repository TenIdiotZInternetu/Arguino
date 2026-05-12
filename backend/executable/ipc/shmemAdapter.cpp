#include <thread>
#include <vector>

#include "../encoder.hpp"
#include "../ipcAdapter.hpp"
#include "TwoWayBuffer.hpp"

using namespace arguino::shmem;
using namespace arguino::simulator;


constexpr uint8_t MESSAGE_DELIMETER = ';';
static TwoWayBuffer* _shmem;

void on_event(const Event& event)
{
    auto message = encode_event(event);
    if (message == UNKNOWN_EVENT) {
        IpcLogger->log_error("Encountered unknown event type while encoding.");
        return;
    }

    if (!_shmem->write(message + ';')) {
        // TODO block
        IpcLogger->log_error(
            "Could not write to shared memory buffer, not enough available space.");
    }
    else {
        IpcLogger->log_debug("Written message to producer buffer: " + message);
    }
}

static void run_shmem_consumer()
{
    // TODO Use IPC semaphore
    while (true) {
        while (!_shmem->consumer().is_empty()) {
            std::vector<uint8_t> data = _shmem->consume_until(MESSAGE_DELIMETER);
            if (data.empty()) {
                IpcLogger->log_error(
                    "A span of incoming message could not be determined, skipping.");
                continue;
            }

            std::string message(data.begin(), data.end());
            IpcLogger->log_debug("Read message from consumer buffer: " + message);

            Event event;
            if (decode_event(message, event)) {
                Simulator::queue().enqueue_remote(event);
            }
            else {
                IpcLogger->log_error("Could not decode event from message " + message);
            }
        }
    }
}

void run_simulator_with_ipc(loop_fnct loopFunction, const ProgramOptions& options)
{
    init_ipc_logger(options);
    _shmem = new TwoWayBuffer(options.ShmemName, options.ShmemSizePages);

    std::thread consumer_thread(run_shmem_consumer);
    IpcLogger->log_info("Shared memory buffer intitialized.");

    loopFunction();
    consumer_thread.join();
}
