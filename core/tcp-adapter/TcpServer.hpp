//
// Created by touster on 10. 10. 2025.
//

#ifndef ARGUINO_CORE_TCPSERVER_HPP
#define ARGUINO_CORE_TCPSERVER_HPP

#include <cstdint>
#include <memory>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>

template<typename T>
concept ConnectionHandler =
    std::derived_from<T, std::enable_shared_from_this<T>> &&
    requires (T handler) {
        T::create() -> std::template shared_ptr<T>;
        handler.handle();
        handler.socket() -> std::template same_as<boost::asio::ip::tcp::socket&>;
    };

namespace arguino::tcp {
    template<ConnectionHandler THandler>
    class TcpServer {
    public:
        TcpServer(uint16_t port);
        void launch();
    private:
        boost::asio::io_context _io_context;
        boost::asio::ip::tcp::socket _socket;
        boost::asio::ip::tcp::endpoint _endpoint;
        boost::asio::ip::tcp::acceptor _acceptor;

        uint16_t _port;

        void start_accepting();
    };
} // tcp
// arguino

#endif //ARGUINO_CORE_TCPSERVER_HPP