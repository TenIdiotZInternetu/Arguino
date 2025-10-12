#include <iostream>

#include "sketch.cpp"
#include "ArguinoConnectionHandler.hpp"
#include "StateEncoder.hpp"
#include "TcpServer.hpp"

using encoder_t = StateEncoder;
using connection_handler_t = ArguinoConnectionHandler<encoder_t>;

int main() {
    arguino::tcp::TcpServer<connection_handler_t> server(8888);
    setup();
    while (true) {
        loop();
        std::cout << "looping" << std::endl;
    }
}
