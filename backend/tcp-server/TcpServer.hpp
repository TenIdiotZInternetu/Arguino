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

#include "ConnectionHandler.hpp"

namespace arguino::tcp {

class TcpServer {
   public:
    using handler_t = ConnectionHandler;
    using logger_ptr = std::shared_ptr<logger::ILogger>;

    TcpServer(uint16_t port, handler_t::message_funct messageHandler, logger_ptr logger);

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

    logger_ptr _logger;

    void start_accepting();
};

}  // namespace arguino::tcp
// arguino

#endif  // ARGUINO_CORE_TCPSERVER_HPP