#ifdef ARGUINO_TCP

#include <thread>

#include "../encoder.hpp"
#include "../ipcAdapter.hpp"
#include "ConnectionHandler.hpp"
#include "FileLogger.hpp"
#include "TcpServer.hpp"

using namespace arguino::simulator;

using tcp_server_t = arguino::tcp::TcpServer;

static std::unique_ptr<tcp_server_t> _tcpServer;


void on_event(const Event& event)
{
    auto message = encode_event(event);
    if (message == UNKNOWN_EVENT) {
        IpcLogger->log_error("Encountered unknown event type; skipping.");
        return;
    }

    IpcLogger->log_debug("Enqueuing message " + message);
    _tcpServer->post_message(message);
}

static void on_received_tcp_message(const std::string message)
{
    Event event;
    if (decode_event(message, event)) {
        Simulator::queue().enqueue_remote(event);
    }
    else {
        IpcLogger->log_error("Could not decode event from message " + message);
    }
}

static void run_tcp()
{
    _tcpServer->launch();
}

void run_simulator_with_ipc(loop_fnct loopFunction, const ProgramOptions& options)
{
    init_ipc_logger(options);

    _tcpServer = std::make_unique<tcp_server_t>(
        options.TcpPort, on_received_tcp_message, IpcLogger);

    std::thread tcp_thread(run_tcp);

    IpcLogger->log_info("Tcp Server launched");

    loopFunction();
    tcp_thread.join();
}

#endif
