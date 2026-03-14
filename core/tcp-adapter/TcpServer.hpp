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

template <logger::ILogger TLogger>
class TcpServer {
   public:
    using handler_t = ConnectionHandler<TLogger>;

    TcpServer(
        uint16_t port, handler_t::message_funct messageHandler, std::shared_ptr<TLogger> logger);

    void launch();

   private:
    boost::asio::io_context _ioContext;
    boost::asio::ip::tcp::endpoint _endpoint;
    boost::asio::ip::tcp::acceptor _acceptor;
    uint16_t _port;

    std::shared_ptr<handler_t> _connectionHandler;
    handler_t::message_funct _messageHandler;

    std::shared_ptr<TLogger> _logger;

    void start_accepting();
};

template <logger::ILogger TLogger>
TcpServer<TLogger>::TcpServer(
    uint16_t port, handler_t::message_funct messageHandler, std::shared_ptr<TLogger> logger)
    : _endpoint(boost::asio::ip::tcp::v4(), port),
      _acceptor(_ioContext, _endpoint),
      _port(port),
      _messageHandler(messageHandler),
      _logger(logger)
{}

template <logger::ILogger TLogger>
void TcpServer<TLogger>::launch()
{
    _acceptor.listen();
    start_accepting();
    _ioContext.run();
}

template <logger::ILogger TLogger>
void TcpServer<TLogger>::start_accepting()
{
    // TODO: Deny connection gracefully, or consider multiple clients to be able to connect
    if (_connectionHandler != nullptr && _connectionHandler->is_connected()) {
        _logger->log("Cannot connect to more than one client at a time; aboring.");
        return;
    }

    _connectionHandler = handler_t::create(_ioContext, _messageHandler, _logger);

    _acceptor.async_accept(_connectionHandler->socket(), [this](auto error) {
        if (!error) {
            _logger->log("Connection accepted!");
            _connectionHandler->handle();
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