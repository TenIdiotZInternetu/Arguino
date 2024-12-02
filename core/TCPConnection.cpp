//
// Created by TIZI on 05/10/2024.
//

#include <iostream>
#include "boost/asio.hpp"
#include "TCPConnection.hpp"


TCPConnection::TCPConnection() {
    std::cout << "i live";
}

TcpClient::TcpClient(boost::asio::io_context iocontext, const std::string& ipString, int port) :
        _socket(iocontext) {
    ip::address ip = ip::address::from_string(ipString);
    auto endpoint = ip::tcp::endpoint(ip, port);
    _socket.connect(endpoint);
}
