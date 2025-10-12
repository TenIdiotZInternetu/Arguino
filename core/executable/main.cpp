#include <iostream>
#include <thread>

#include "sketch.cpp"
#include "ArguinoConnectionHandler.hpp"
#include "StateEncoder.hpp"
#include "TcpServer.hpp"

using encoder_t = StateEncoder;
using connection_handler_t = ArguinoConnectionHandler<encoder_t>;

int main() {
    std::thread t([] {
        arguino::tcp::TcpServer<connection_handler_t> server(8888);
        server.launch();
    });

    G_ARDUINO_STATE.init_timer();
    setup();
    while (true) {
        loop();
    }
}
