//
// Created by touster on 10. 10. 2025.
//

#ifndef ARGUINO_CORE_TCPSERVER_HPP
#define ARGUINO_CORE_TCPSERVER_HPP

#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <condition_variable>
#include <cstdint>
#include <memory>
#include <mutex>

#include "LogMessages.hpp"

namespace arguino::tcp {

template <logger::ILogger TLogger>
class TcpServer {
   public:
    using handler_t = ConnectionHandler<TLogger>;

    TcpServer(
        uint16_t port, handler_t::message_funct messageHandler, std::shared_ptr<TLogger> logger);

    void launch();
    void post_message(const std::string& message);  // TODO: Decouple from connection handler

    bool is_connected() const;

   private:
    boost::asio::io_context _ioContext;
    boost::asio::ip::tcp::endpoint _endpoint;
    boost::asio::ip::tcp::acceptor _acceptor;
    uint16_t _port;

    // TODO: Decouple from server, and return each handler in a callback
    std::shared_ptr<handler_t> _connectionHandler;
    std::mutex _connectionMutex;
    std::condition_variable _connectionCv;

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
inline void TcpServer<TLogger>::post_message(const std::string& message)
{
    {
        std::unique_lock lock(_connectionMutex);
        _connectionCv.wait(lock, [this]() { return is_connected(); });
    }
    _connectionHandler->post_message(message);
}

template <logger::ILogger TLogger>
inline bool TcpServer<TLogger>::is_connected() const
{
    return _connectionHandler != nullptr && _connectionHandler->is_connected();
}

template <logger::ILogger TLogger>
void TcpServer<TLogger>::start_accepting()
{
    // TODO: Handle reconnects to the same client
    auto newConnection = handler_t::create(_ioContext, _messageHandler, _logger);

    _acceptor.async_accept(newConnection->socket(), [=, this](auto error) {
        // TODO: Consider multiple clients to be able to connect
        if (is_connected()) {
            _logger->log("Connection denied: Cannot connect to more than one client at a time.");
        }
        else if (!error) {
            _logger->log("Connection accepted!");
            _connectionHandler = newConnection;
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