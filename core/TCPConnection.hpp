//
// Created by TIZI on 05/10/2024.
//

#ifndef BC_TCPCONNECTION_HPP
#define BC_TCPCONNECTION_HPP

#include "boost/asio.hpp"

class TcpServer {
public:
    TcpServer(boost::asio::io_context& ioContext);
private:
    boost::asio::ip::tcp::socket _socket;

};

class TcpClient {
public:
    TcpClient(boost::asio::io_context& iocontext, const std::string& ipString, int port);
    void Connect(boost::system::error_code& error);
    std::string ReadMessage();
private:
    static constexpr int BUFFER_SIZE = 64;

    boost::asio::io_context& _ioc;
    boost::asio::ip::tcp::socket _socket;
    boost::asio::ip::tcp::endpoint _endpoint;
};


#endif //BC_TCPCONNECTION_HPP
