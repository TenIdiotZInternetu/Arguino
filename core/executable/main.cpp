#include <filesystem>
#include <iostream>
#include <thread>

#include "ArguinoConnectionHandler.hpp"
#include "FileLogger.hpp"
#include "StateEncoder.hpp"
#include "TcpServer.hpp"
#include "sketch.cpp"
#include "upp_shrmem.hpp"

using logger_t = logger::FileLogger;
using encoder_t = arguino::tcp::StateEncoder;
using connection_handler_t = arguino::tcp::ArguinoConnectionHandler<encoder_t, logger_t>;
using tcp_server_t = arguino::tcp::TcpServer<connection_handler_t, logger_t>;

void run_tcp()
{
    auto logger = std::make_shared<logger_t>(std::filesystem::absolute("./core_tcp.log"));
    tcp_server_t server(8888, logger);
    logger->log("Tcp Server initialized");
    server.launch();
}

void run_simulator()
{
    auto logger = std::make_shared<logger_t>(std::filesystem::absolute("./core.log"));
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
    std::thread tcp_thread(run_tcp);
    run_simulator();
    tcp_thread.join();
}
