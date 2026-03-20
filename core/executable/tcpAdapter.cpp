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

static std::string encode_event(const Event& event) {}

static Event decode_event(const std::string& message) {}

static void on_send_event(const Event& event)
{
    auto message = encode_event(event);
    _tcpLogger->log("Sending message " + message);
    // TODO: write to current handler's writeQueue
}

static void on_received_tcp_message(const std::string message)
{
    _tcpLogger->log("Message recieved " + message);
    auto event = decode_event(message);
    Simulator::queue().enqueue_remote(event);
}

static void run_tcp()
{
    tcp_server_t server(_options->TcpPort, on_received_tcp_message, _tcpLogger);
    server.launch();
}

void setup_simulator_with_ipc(loop_fnct simulatorLoop, const ProgramOptions& options)
{
    _options = &options;
    auto loggerAbsolutePath = std::filesystem::absolute(_options->TcpLogPath);
    _tcpLogger = std::make_shared<logger_t>(loggerAbsolutePath);

    std::thread tcp_thread(run_tcp);
    _tcpLogger->log("Tcp Server launched");

    simulatorLoop(on_send_event);
    tcp_thread.join();
}

#endif
