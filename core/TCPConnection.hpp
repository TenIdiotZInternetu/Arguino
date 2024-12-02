//
// Created by TIZI on 05/10/2024.
//

#ifndef BC_TCPCONNECTION_HPP
#define BC_TCPCONNECTION_HPP

using namespace boost;

class TcpServer {
public:
    TcpServer(asio::io_context& ioContext);
private:
    asio::ip::tcp::socket _socket;

};

class TcpClient {
public:
    TcpClient(asio::io_context& iocontext, const std::string& ipString, int port);
    std::string ReadMessage();
private:
    static constexpr int BUFFER_SIZE = 64;

    asio::io_context& _ioc;
    asio::ip::tcp::socket _socket;
};


#endif //BC_TCPCONNECTION_HPP
