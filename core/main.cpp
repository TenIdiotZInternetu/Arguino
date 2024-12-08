#include <iostream>
#include <thread>
#include "TCPConnection.hpp"

#include "boost/asio.hpp"

void bla() {
    boost::asio::io_context ioContext;
    TcpClient client(ioContext, 2401);
    boost::system::error_code connectionError;

    client.Connect(connectionError);

    while (connectionError) {
        std::cout << "Waiting for connection..." << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(3));
        client.Connect(connectionError);
    }

    std::cout << "Listening..." << std::endl;

    while (true) {
        std::cout << client.ReadMessage(":<>:") << std::endl;
    }
}

int main() {
    std::thread t(bla);
    t.join();
    return 0;
}