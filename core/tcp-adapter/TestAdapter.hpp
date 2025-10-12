//
// Created by TIZI on 05/10/2024.
//

#ifndef BC_TCPCONNECTION_HPP
#define BC_TCPCONNECTION_HPP

#include "boost/asio.hpp"

class TestServer {
public:
    TestServer(boost::asio::io_context& iocontext, int port);
    boost::system::error_code WriteMessage(const std::string& message);

private:
    boost::asio::io_context& _ioc;
    boost::asio::ip::tcp::socket _socket;
    boost::asio::ip::tcp::endpoint _endpoint;
    boost::asio::ip::tcp::acceptor _acceptor;
};

class TestClient {
public:
    TestClient(boost::asio::io_context& iocontext, int port);
    TestClient(boost::asio::io_context& iocontext, const std::string& ipString, int port);
    void Connect(boost::system::error_code& error);
    std::string ReadMessage(const std::string& messageDelimeter);

private:
    static constexpr int BUFFER_SIZE = 64;
    static constexpr std::string LOCAL_HOST = "127.0.0.1";

    boost::asio::io_context& _ioc;
    boost::asio::ip::tcp::socket _socket;
    boost::asio::ip::tcp::endpoint _endpoint;

    std::string _buffer;
};


#endif //BC_TCPCONNECTION_HPP
