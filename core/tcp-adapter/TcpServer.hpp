//
// Created by touster on 10. 10. 2025.
//

#ifndef ARGUINO_CORE_TCPSERVER_HPP
#define ARGUINO_CORE_TCPSERVER_HPP

#include <cstdint>
#include <memory>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>

namespace arguino::tcp {
    template<typename T>
    concept ConnectionHandler =
        std::derived_from<T, std::enable_shared_from_this<T>> &&
        requires (T handler) {
        { T::create() } -> std::convertible_to<std::shared_ptr<T>>;
        { handler.handle() } -> std::same_as<void>;
        { handler.socket() } -> std::same_as<boost::asio::ip::tcp::socket&>;
        };

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

    template <ConnectionHandler THandler>
    TcpServer<THandler>::TcpServer(uint16_t port) :
        _socket(_io_context),
        _endpoint(boost::asio::ip::tcp::v4(), port),
        _acceptor(_io_context, _endpoint),
        _port(port) {}

    template <ConnectionHandler THandler>
    void TcpServer<THandler>::launch() {
        _acceptor.listen();
        start_accepting();
        _io_context.run();
    }

    template <ConnectionHandler THandler>
    void TcpServer<THandler>::start_accepting() {
        std::shared_ptr<THandler> handler = THandler::create();

        _acceptor.async_accept(handler->socket(), [this, handler](auto error) {
            if (!error) {
                handler->handle();
            } else {
                //TODO log
            }
            start_accepting();
        });
    }
}
// arguino

#endif //ARGUINO_CORE_TCPSERVER_HPP