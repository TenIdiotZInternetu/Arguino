#ifdef ARGUINO_TCP

#include <thread>

#include "../encoder.hpp"
#include "../ipcAdapter.hpp"
#include "ConnectionHandler.hpp"
#include "Events.hpp"
#include "FileLogger.hpp"
#include "TcpServer.hpp"

using namespace arguino::simulator;

using logger_t = logger::FileLogger;
using tcp_server_t = arguino::tcp::TcpServer;

static const ProgramOptions* _options;
static std::shared_ptr<logger_t> _tcpLogger;
static std::unique_ptr<tcp_server_t> _tcpServer;


void on_event(const Event& event)
{
    auto message = encode_event(event);
    if (message == UNKNOWN_EVENT) {
        _tcpLogger->log_error("Encountered unknown event type; skipping.");
        return;
    }

    _tcpLogger->log_debug("Enqueuing message " + message);
    _tcpServer->post_message(message);
}

static void on_received_tcp_message(const std::string message)
{
    Event event;
    if (decode_event(message, event)) {
        Simulator::queue().enqueue_remote(event);
    }
    else {
        _tcpLogger->log_error("Could not decode event from message " + message);
    }
}

static void run_tcp()
{
    _tcpServer->launch();
}

void run_simulator_with_ipc(loop_fnct loopFunction, const ProgramOptions& options)
{
    _options = &options;
    auto loggerAbsolutePath = std::filesystem::absolute(_options->TcpLogPath);
    _tcpLogger = std::make_shared<logger_t>(loggerAbsolutePath);

    _tcpLogger->verbosityLevel = options.Verbosity;
    _tcpLogger->timerFunction = Simulator::real_time;

    _tcpServer = std::make_unique<tcp_server_t>(
        _options->TcpPort, on_received_tcp_message, _tcpLogger);

    std::thread tcp_thread(run_tcp);

    _tcpLogger->log_info("Tcp Server launched");

    loopFunction();
    tcp_thread.join();
}

#endif
