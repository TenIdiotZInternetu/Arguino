//
// Created by TIZI on 05/10/2024.
//

#include <iostream>
#include "TCPConnection.hpp"

using namespace boost;

TcpClient::TcpClient(asio::io_context& iocontext, const std::string& ipString, int port) :
        _ioc(iocontext), _socket(_ioc), _endpoint() {
    asio::ip::address ip = asio::ip::address::from_string(ipString);
    _endpoint = asio::ip::tcp::endpoint(ip, port);
}

std::string TcpClient::ReadMessage() {
    asio::streambuf buffer(BUFFER_SIZE);
    boost::system::error_code error;

    std::size_t bytes_read = asio::read_until(_socket, buffer, '\n', error);

    if (error) {
        throw system::system_error(error);
    }

    buffer.commit(bytes_read);
    std::istream is(&buffer);

    std::string message;
    is >> message;
    return message;
}

void TcpClient::Connect(system::error_code& error) {
    _socket.connect(_endpoint, error);
}
