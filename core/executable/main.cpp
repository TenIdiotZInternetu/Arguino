#include <filesystem>
#include <iostream>
#include <thread>

#include "ArduinoState.hpp"
#include "FileLogger.hpp"
#include "programOptions.cpp"
#include "sketch.cpp"

using logger_t = logger::FileLogger;

ProgramOptions options;

#ifdef ARGUINO_TCP

#include "ArguinoConnectionHandler.hpp"
#include "StateEncoder.hpp"
#include "TcpServer.hpp"

using encoder_t = arguino::tcp::StateEncoder;
using connection_handler_t = arguino::tcp::ArguinoConnectionHandler<encoder_t, logger_t>;
using tcp_server_t = arguino::tcp::TcpServer<connection_handler_t, logger_t>;


void run_tcp()
{
    auto logger = std::make_shared<logger_t>(std::filesystem::absolute(options.TcpLogPath));
    tcp_server_t server(options.TcpPort, logger);
    logger->log("Tcp Server initialized");
    server.launch();
}
#endif

void run_simulator()
{
    auto logger = std::make_shared<logger_t>(std::filesystem::absolute(options.SimulatorLogPath));
    CanonicalState::init();
    CanonicalState::init_logger(logger);
    CanonicalState::state().init_timer();

    setup();
    while (true) {
        loop();
    }
}

int main(int argc, char** argv)
{
    options = parse_arguments(argc, argv);

#ifdef ARGUINO_TCP
    std::thread tcp_thread(run_tcp);
    tcp_thread.join();
#endif

#ifdef ARGUINO_SHARED_MEMORY
    std::cout << "Shared memory implementation" << std::endl;
#endif

    run_simulator();
}
