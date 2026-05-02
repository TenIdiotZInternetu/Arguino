#include "TcpServer.hpp"

namespace arguino::tcp {

TcpServer::TcpServer(uint16_t port, handler_t::message_funct messageHandler, logger_ptr logger)
    : _endpoint(boost::asio::ip::tcp::v4(), port),
      _acceptor(_ioContext, _endpoint),
      _port(port),
      _messageHandler(messageHandler),
      _logger(logger)
{}

void TcpServer::launch()
{
    _acceptor.listen();
    start_accepting();
    _ioContext.run();
}

void TcpServer::post_message(const std::string& message)
{
    {
        std::unique_lock lock(_connectionMutex);
        _connectionCv.wait(lock, [this]() { return is_connected(); });
    }
    _connectionHandler->post_message(message);
}

bool TcpServer::is_connected() const
{
    return _connectionHandler != nullptr && _connectionHandler->is_connected();
}

void TcpServer::start_accepting()
{
    auto newConnection = handler_t::create(_ioContext, _messageHandler, _logger);

    _acceptor.async_accept(newConnection->socket(), [=, this](auto error) {
        // TODO: Consider multiple clients to be able to connect
        // TODO: Race condition in is_connected()
        // if (is_connected()) {
        //     _logger->log_error("Connection denied: Cannot connect to more than one client at a
        //     time.");
        // }
        if (!error) {
            _logger->log_info("Connection accepted!");
            _connectionHandler = newConnection;
            _connectionCv.notify_one();
            _connectionHandler->handle();
        }
        else {
            _logger->log_error("Error occured while accepting new connection: " + error.what());
        }
        start_accepting();
    });
}
}