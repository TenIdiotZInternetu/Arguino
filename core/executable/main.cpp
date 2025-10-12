#include "sketch.cpp"
#include "../tcp-adapter/ArguinoConnectionHandler.hpp"
#include "../tcp-adapter/TcpServer.hpp"

int main() {
    arguino::tcp::TcpServer<ArguinoConnectionHandler> server(8888);
    setup();
    while (true) {
        loop();
    }
}
