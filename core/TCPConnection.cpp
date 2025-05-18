//
// Created by TIZI on 05/10/2024.
//

#include <iostream>
#include "TCPConnection.hpp"

using namespace boost;

TcpServer::TcpServer(asio::io_context& iocontext, int port) :
        _ioc(iocontext),
        _socket(_ioc),
        _endpoint(asio::ip::tcp::v4(), port),
        _acceptor(_ioc, _endpoint) {
    _acceptor.accept(_socket);
}

boost::system::error_code TcpServer::WriteMessage(const std::string& message) {
    boost::system::error_code error;
    asio::write(_socket, asio::buffer(message), error);
    return error;
}

TcpClient::TcpClient(asio::io_context& iocontext, int port) :
        _ioc(iocontext), _socket(_ioc), _endpoint() {
    asio::ip::address ip = asio::ip::make_address(LOCAL_HOST);
    _endpoint = asio::ip::tcp::endpoint(ip, port);
}

TcpClient::TcpClient(asio::io_context& iocontext, const std::string& ipString, int port) :
        _ioc(iocontext), _socket(_ioc), _endpoint() {
    asio::ip::address ip = asio::ip::make_address(ipString);
    _endpoint = asio::ip::tcp::endpoint(ip, port);
}

std::string TcpClient::ReadMessage(const std::string& messageDelimeter) {
    boost::system::error_code error;

    std::size_t bytesRead = asio::read_until(
    _socket, asio::dynamic_buffer(_buffer), messageDelimeter, error
    );

    if (error) {
        throw system::system_error(error);
    }

    std::size_t messageLength = bytesRead - messageDelimeter.size();

    std::string message = _buffer.substr(0, messageLength);
    _buffer.erase(0, bytesRead);
    return message;
}

void TcpClient::Connect(system::error_code& error) {
    _socket.connect(_endpoint, error);
}
