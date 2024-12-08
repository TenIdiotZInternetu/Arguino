#include <iostream>
#include <thread>
#include "TCPConnection.hpp"

#include "boost/asio.hpp"

void testClient() {
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

void testServer() {
    boost::asio::io_context ioContext;
    TcpServer server(ioContext, 2400);

    while (true) {
        server.WriteMessage("I'm alive:<>:");
        std::this_thread::sleep_for(std::chrono::seconds(3));
    }
}

int main() {
    std::thread t(testServer);
    t.join();
    return 0;
}