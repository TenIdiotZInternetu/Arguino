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

    ConnectionHandler(boost::asio::io_context& ioContext, logger_ptr logger);
    static self_ptr create(boost::asio::io_context& ioContext, logger_ptr logger);

    void handle();
    void post(const std::string& message);
    boost::asio::ip::tcp::socket& socket() { return _socket; }

   private:
    boost::asio::io_context::executor_type _executor;
    boost::asio::ip::tcp::socket _socket;
    std::string _buffer;
    std::string _outcomingMessage;

    message_funct _onReadMessage;

    logger_ptr _logger;

    void on_read_message(boost::system::error_code error, size_t messageSize);
};

template <logger::ILogger TLogger>
ConnectionHandler<TLogger>::ConnectionHandler(boost::asio::io_context& ioContext, logger_ptr logger)
    : _executor(ioContext.get_executor()), _socket(ioContext), _logger(logger)
{}

template <logger::ILogger TLogger>
ConnectionHandler<TLogger>::self_ptr ConnectionHandler<TLogger>::create(
    boost::asio::io_context& ioContext, logger_ptr logger)
{
    return std::make_shared<ConnectionHandler>(ioContext, logger);
}

template <logger::ILogger TLogger>
void ConnectionHandler<TLogger>::handle()
{
    boost::asio::async_read_until(_socket,
        boost::asio::dynamic_buffer(_buffer),
        MESSAGE_DELIMITER,
        [me = this->shared_from_this()](auto error, size_t messageSize) {  //
            me->on_read_message(error, messageSize);
        }  //
    );
}

template <logger::ILogger TLogger>
void ConnectionHandler<TLogger>::on_read_message(
    boost::system::error_code error, size_t messageSize)
{
    if (error) {
        _logger->log(message::Error("Error occured while reading from socket: ", error));
        return;
    }

    if (_buffer.empty()) return;

    // TODO: expect timestamp in the message

    const std::string message = _buffer.substr(0, messageSize);

    if (message[0] == READ_FLAG) {
        handle_read_state(message);
    }
    if (message[0] == WRITE_FLAG) {
        handle_write_state(message);
    }

    _buffer.erase(0, messageSize);
    handle();
}

template <logger::ILogger TLogger>
void ConnectionHandler<TLogger>::handle_read_state(const std::string& message)
{
    boost::asio::async_write(_socket,
        boost::asio::buffer(_outcomingMessage),
        [me = this->shared_from_this()](auto error, size_t bytes_written) {
            if (error) {
                me->_logger->log(message::Error("Error occured while writing to socket: ", error));
            }
            else {
                me->_logger->log(message::Read(me->_outcomingMessage));
            }
        }  //
    );
}

template <logger::ILogger TLogger>
void ConnectionHandler<TLogger>::handle_write_state(const std::string& message)
{
    simulator::ArduinoState newState = encoder.decode(message.substr(1));  // skip write flag
    simulator::CanonicalState::update_state(newState);
    _logger->log(message::Write(message));
}

}  // namespace arguino::tcp


#endif  // ARGUINO_CORE_ARGUINOMESSAGEHANDLER_HPP
