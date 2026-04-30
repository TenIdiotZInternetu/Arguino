#include <ipcAdapter.hpp>
#include <shared-memory-adapter/IpcHandler.hpp>

void setup_shared_memory()
{
    arguino::shmem::IpcHandler shmem(options.ShmemName, options.ShmemSizePages);

    shmem.write(std::vector<uint32_t>{7, 2, 7});

    for (uint64_t i = 0; i < 1024; ++i) {
        shmem.write(i);
    }

    std::cout << "INitneded" << std::endl;
}

void run_simulator_with_ipc(loop_fnct loopFunction, const ProgramOptions& options) {}

void on_event(const arguino::simulator::Event& event) {}