#ifdef ARGUINO_TCP

#include <thread>

#include "ConnectionHandler.hpp"
#include "Events.hpp"
#include "FileLogger.hpp"
#include "StateEncoder.hpp"
#include "TcpServer.hpp"
#include "ipcAdapter.hpp"

using namespace arguino::simulator;

using logger_t = logger::FileLogger;
using encoder_t = arguino::tcp::StateEncoder;
using tcp_server_t = arguino::tcp::TcpServer<logger_t>;

static const ProgramOptions* _options;
static std::shared_ptr<logger_t> _tcpLogger;
static std::unique_ptr<tcp_server_t> _tcpServer;

static std::string encode_event(const Event& event)
{
    return std::format("Sending event {}", event.id());
}

static Event decode_event(const std::string& message) {}

static void on_send_event(const Event& event)
{
    auto message = encode_event(event);
    _tcpLogger->log("Enqueuing message " + message);
    _tcpServer->post_message(message);
}

static void on_received_tcp_message(const std::string message)
{
    _tcpLogger->log("Message recieved " + message);
    auto event = decode_event(message);
    Simulator::queue().enqueue_remote(event);
}

static void run_tcp()
{
    _tcpServer->launch();
}

void setup_simulator_with_ipc(loop_fnct simulatorLoop, const ProgramOptions& options)
{
    _options = &options;
    auto loggerAbsolutePath = std::filesystem::absolute(_options->TcpLogPath);
    _tcpLogger = std::make_shared<logger_t>(loggerAbsolutePath);

    _tcpServer = std::make_unique<tcp_server_t>(
        _options->TcpPort, on_received_tcp_message, _tcpLogger);

    std::thread tcp_thread(run_tcp);

    _tcpLogger->log("Tcp Server launched");

    simulatorLoop(on_send_event);
    tcp_thread.join();
}

#endif
