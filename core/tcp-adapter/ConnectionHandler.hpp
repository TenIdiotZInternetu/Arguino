//
// Created by touster on 12. 10. 2025.
//

#ifndef ARGUINO_CORE_ARGUINOMESSAGEHANDLER_HPP
#define ARGUINO_CORE_ARGUINOMESSAGEHANDLER_HPP

#include <atomic>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/read_until.hpp>
#include <boost/asio/write.hpp>
#include <condition_variable>
#include <mutex>
#include <queue>

#include "ArduinoState.hpp"
#include "LogMessages.hpp"

namespace arguino::tcp {

template <logger::ILogger TLogger>
class ConnectionHandler : public std::enable_shared_from_this<ConnectionHandler<TLogger>> {
   public:
    static constexpr char MESSAGE_DELIMITER = ';';
    static constexpr size_t MAX_QUEUE_SIZE = 100;

    using self_ptr = std::shared_ptr<ConnectionHandler<TLogger>>;
    using logger_ptr = std::shared_ptr<TLogger>;
    using message_funct = std::function<void(const std::string&)>;

    ConnectionHandler(
        boost::asio::io_context& ioContext, message_funct messageHandler, logger_ptr logger);
    static self_ptr create(
        boost::asio::io_context& ioContext, message_funct messageHandler, logger_ptr logger);

    void handle();
    void post_message(const std::string& message);
    boost::asio::ip::tcp::socket& socket() { return _socket; }

    bool is_connected() const { return _isConnected; }

   private:
    boost::asio::io_context::executor_type _executor;
    boost::asio::ip::tcp::socket _socket;

    std::string _incomingBuffer;

    std::queue<std::string> _outboxQueue;
    std::mutex _queueMutex;
    std::condition_variable _queueCv;

    size_t _connectionId;
    static size_t _nextConnectionId;
    bool _isConnected;

    std::atomic<bool> _isWriting;
    message_funct _messageHandler;

    logger_ptr _logger;

    void handle_message(boost::system::error_code error, size_t messageSize);
    void write_from_queue();
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
        boost::asio::dynamic_buffer(_incomingBuffer),                      //
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
void ConnectionHandler<TLogger>::post_message(const std::string& message)
{
    {
        std::unique_lock lock(_queueMutex);
        _queueCv.wait(lock, [this]() {  //
            return _outboxQueue.size() <= MAX_QUEUE_SIZE;
        });

        _outboxQueue.push(message);
    }

    boost::asio::post(_executor, [me = this->shared_from_this()] {  //
        me->write_from_queue();
    });
}

template <logger::ILogger TLogger>
void ConnectionHandler<TLogger>::handle_message(boost::system::error_code error, size_t messageSize)
{
    if (error) {
        _logger->log(message::Error("Error occured while reading from socket: ", error));
        return;
    }

    if (_incomingBuffer.empty()) return;

    const std::string message = _incomingBuffer.substr(0, messageSize - 1);  // -1 strips delimeter
    _logger->log(message::Read(message));
    _messageHandler(message);

    _incomingBuffer.erase(0, messageSize);
}

template <logger::ILogger TLogger>
inline void ConnectionHandler<TLogger>::write_from_queue()
{
    if (_isWriting) return;
    if (_outboxQueue.empty()) return;

    _isWriting = true;

    boost::asio::async_write(_socket,
        boost::asio::buffer(_outboxQueue.front() + ';'),
        [me = this->shared_from_this()](auto error, size_t bytes_written) {
            if (error) {
                me->_logger->log(message::Error("Error occured while writing to socket: ", error));
            }
            else {
                me->_logger->log(message::Write(me->_outboxQueue.front()));
                me->_outboxQueue.pop();
                me->_queueCv.notify_one();
            }

            me->_isWriting = false;
            me->write_from_queue();
        }  //
    );
}

template <logger::ILogger TLogger>
inline void ConnectionHandler<TLogger>::disconnect()
{
    _isConnected = false;
    _logger->log("The client has disconnected");
}

}  // namespace arguino::tcp


#endif  // ARGUINO_CORE_ARGUINOMESSAGEHANDLER_HPP
