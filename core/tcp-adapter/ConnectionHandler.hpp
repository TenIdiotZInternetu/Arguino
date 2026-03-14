//
// Created by touster on 12. 10. 2025.
//

#ifndef ARGUINO_CORE_ARGUINOMESSAGEHANDLER_HPP
#define ARGUINO_CORE_ARGUINOMESSAGEHANDLER_HPP

#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/read_until.hpp>
#include <boost/asio/write.hpp>

#include "ArduinoState.hpp"
#include "LogMessages.hpp"

namespace arguino::tcp {

template <logger::ILogger TLogger>
class ConnectionHandler : public std::enable_shared_from_this<ConnectionHandler<TLogger>> {
   public:
    static constexpr char MESSAGE_DELIMITER = ';';

    using self_ptr = std::shared_ptr<ConnectionHandler<TLogger>>;
    using logger_ptr = std::shared_ptr<TLogger>;
    using message_funct = std::function<void(const std::string&)>;

    ConnectionHandler(
        boost::asio::io_context& ioContext, message_funct messageHandler, logger_ptr logger);
    static self_ptr create(
        boost::asio::io_context& ioContext, message_funct messageHandler, logger_ptr logger);

    void handle();
    void post(const std::string& message);
    boost::asio::ip::tcp::socket& socket() { return _socket; }

    bool is_connected() { return _isConnected; }

   private:
    boost::asio::io_context::executor_type _executor;
    boost::asio::ip::tcp::socket _socket;
    std::string _buffer;
    std::string _outcomingMessage;

    size_t _connectionId;
    static size_t _nextConnectionId;
    bool _isConnected;

    message_funct _messageHandler;

    logger_ptr _logger;

    void handle_message(boost::system::error_code error, size_t messageSize);
    void disconnect();
};

template <logger::ILogger TLogger>
size_t ConnectionHandler<TLogger>::_nextConnectionId = 1;

template <logger::ILogger TLogger>
ConnectionHandler<TLogger>::ConnectionHandler(
    boost::asio::io_context& ioContext, message_funct messageHandler, logger_ptr logger)
    : _executor(ioContext.get_executor()),
      _socket(ioContext),
      _messageHandler(messageHandler),
      _logger(logger)
{}

template <logger::ILogger TLogger>
ConnectionHandler<TLogger>::self_ptr ConnectionHandler<TLogger>::create(
    boost::asio::io_context& ioContext, message_funct messageHandler, logger_ptr logger)
{
    return std::make_shared<ConnectionHandler>(ioContext, messageHandler, logger);
}

template <logger::ILogger TLogger>
void ConnectionHandler<TLogger>::handle()
{
    _isConnected = true;
    _connectionId = _nextConnectionId;
    ++_nextConnectionId;

    boost::asio::async_read_until(                                         //
        _socket,                                                           //
        boost::asio::dynamic_buffer(_buffer),                              //
        MESSAGE_DELIMITER,                                                 //
        [me = this->shared_from_this()](auto error, size_t messageSize) {  //
            if (error == boost::asio::error::eof) {
                me->disconnect();
            }
            else {
                me->handle_message(error, messageSize);
                me->handle();
            }
        }  //
    );
}

template <logger::ILogger TLogger>
void ConnectionHandler<TLogger>::handle_message(boost::system::error_code error, size_t messageSize)
{
    if (error) {
        _logger->log(message::Error("Error occured while reading from socket: ", error));
        return;
    }

    if (_buffer.empty()) return;

    const std::string message = _buffer.substr(0, messageSize);
    me->_logger->log(message::Write(me->_outcomingMessage));
    _messageHandler(message);

    _buffer.erase(0, messageSize);
}

template <logger::ILogger TLogger>
inline void ConnectionHandler<TLogger>::disconnect()
{
    _isConnected = false;
    _logger->log("The client has disconnected");
}

template <logger::ILogger TLogger>
void ConnectionHandler<TLogger>::post(const std::string& message)
{
    boost::asio::async_write(_socket,
        boost::asio::buffer(_outcomingMessage),
        [me = this->shared_from_this()](auto error, size_t bytes_written) {
            if (error) {
                me->_logger->log(message::Error("Error occured while writing to socket: ", error));
            }
            else {
                me->_logger->log(message::Write(me->_outcomingMessage));
            }
        }  //
    );
}

}  // namespace arguino::tcp


#endif  // ARGUINO_CORE_ARGUINOMESSAGEHANDLER_HPP
