#include <filesystem>
#include <iostream>
#include <thread>

#include "FileLogger.hpp"
#include "Simulator.hpp"
#include "programOptions.cpp"
#include "sketch.cpp"

using logger_t = logger::FileLogger;

ProgramOptions options;

#ifdef ARGUINO_TCP

#include "ConnectionHandler.hpp"
#include "StateEncoder.hpp"
#include "TcpServer.hpp"

using encoder_t = arguino::tcp::StateEncoder;
using tcp_server_t = arguino::tcp::TcpServer<logger_t>;


void test(const std::string msg)
{
    arguino::simulator::Simulator::log("Message recieved " + msg);
}

void run_tcp()
{
    auto logger = std::make_shared<logger_t>(std::filesystem::absolute(options.TcpLogPath));
    tcp_server_t server(options.TcpPort, test, logger);
    logger->log("Tcp Server initialized");
    server.launch();
}
#endif

void run_simulator()
{
    using state_t = arguino::simulator::Simulator;
    auto logger = std::make_shared<logger_t>(std::filesystem::absolute(options.SimulatorLogPath));

    state_t::init();
    state_t::init_logger(logger);
    state_t::state().init_timer();

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
    run_simulator();
    tcp_thread.join();
#endif

#ifdef ARGUINO_SHARED_MEMORY
    std::cout << "Shared memory implementation" << std::endl;
    run_simulator();
#endif
}
