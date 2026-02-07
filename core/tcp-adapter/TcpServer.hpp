//
// Created by touster on 10. 10. 2025.
//

#ifndef ARGUINO_CORE_TCPSERVER_HPP
#define ARGUINO_CORE_TCPSERVER_HPP

#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <cstdint>
#include <memory>

#include "LogMessages.hpp"

namespace arguino::tcp {
template <typename T>
concept IConnectionHandler = std::derived_from<T, std::enable_shared_from_this<T>> &&
    requires(T handler, boost::asio::io_context& ioc) {
        // { T::create(ioc, logger) } -> std::convertible_to<std::shared_ptr<T>>;
        { handler.handle() } -> std::same_as<void>;
        { handler.socket() } -> std::same_as<boost::asio::ip::tcp::socket&>;
    };

template <IConnectionHandler THandler, logger::ILogger TLogger>
class TcpServer {
   public:
    TcpServer(uint16_t port, std::shared_ptr<TLogger> logger);
    void launch();

   private:
    boost::asio::io_context _ioContext;
    boost::asio::ip::tcp::endpoint _endpoint;
    boost::asio::ip::tcp::acceptor _acceptor;

    uint16_t _port;
    std::shared_ptr<TLogger> _logger;

    void start_accepting();
};

template <IConnectionHandler THandler, logger::ILogger TLogger>
TcpServer<THandler, TLogger>::TcpServer(uint16_t port, std::shared_ptr<TLogger> logger)
    : _endpoint(boost::asio::ip::tcp::v4(), port),
      _acceptor(_ioContext, _endpoint),
      _port(port),
      _logger(logger)
{}

template <IConnectionHandler THandler, logger::ILogger TLogger>
void TcpServer<THandler, TLogger>::launch()
{
    _acceptor.listen();
    start_accepting();
    _ioContext.run();
}

template <IConnectionHandler THandler, logger::ILogger TLogger>
void TcpServer<THandler, TLogger>::start_accepting()
{
    std::shared_ptr<THandler> handler = THandler::create(_ioContext, _logger);

    _acceptor.async_accept(handler->socket(), [this, handler](auto error) {
        if (!error) {
            _logger->log("Connection accepted!");
            handler->handle();
        }
        else {
            _logger->log(message::Error("Error occured while accepting new connection: ", error));
        }
        start_accepting();
    });
}
}  // namespace arguino::tcp
// arguino

#endif  // ARGUINO_CORE_TCPSERVER_HPP