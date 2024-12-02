//
// Created by TIZI on 05/10/2024.
//

#include <iostream>
#include "boost/asio.hpp"

#include "TCPConnection.hpp"


TCPConnection::TCPConnection() {
    std::cout << "i live";
}

TcpClient::TcpClient(asio::io_context iocontext, const std::string& ipString, int port) :
        _socket(iocontext) {
    asio::ip::address ip = asio::ip::address::from_string(ipString);
    auto endpoint = asio::ip::tcp::endpoint(ip, port);
    _socket.connect(endpoint);
}

std::string TcpClient::ReadMessage() {
    std::string message;
    std::array<char, BUFFER_SIZE> buffer = {};
    boost::system::error_code error;

    while (true) {
        size_t messageLength = _socket.read_some(asio::buffer(buffer), error);

        if (error == asio::error::eof) {
            break;
        } else if (error) {
            throw system::system_error(error);
        }

        message.append(buffer.data(), messageLength);
    }
    return message;
}
