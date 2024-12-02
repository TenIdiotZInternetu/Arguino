//
// Created by TIZI on 05/10/2024.
//

#ifndef BC_TCPCONNECTION_HPP
#define BC_TCPCONNECTION_HPP

using namespace boost::asio;

class TCPConnection {
public:
    TCPConnection();
private:
};


class TcpServer {
public:
    TcpServer(boost::asio::io_context ioContext);
private:
    ip::tcp::socket _socket;

};

class TcpClient {
public:
    TcpClient(boost::asio::io_context iocontext, const std::string& ipString, int port);
private:
    boost::asio::io_context _ioc;
    ip::tcp::socket _socket;
};


#endif //BC_TCPCONNECTION_HPP
