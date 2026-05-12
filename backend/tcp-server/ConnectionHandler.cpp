#include "ConnectionHandler.hpp"

namespace arguino::tcp {

size_t ConnectionHandler::_nextConnectionId = 1;

ConnectionHandler::ConnectionHandler(
    boost::asio::io_context& ioContext, message_funct messageHandler, logger_ptr logger)
    : _executor(ioContext.get_executor()),
      _socket(ioContext),
      _messageHandler(messageHandler),
      _logger(logger)
{}

ConnectionHandler::self_ptr ConnectionHandler::create(
    boost::asio::io_context& ioContext, message_funct messageHandler, logger_ptr logger)
{
    return std::make_shared<ConnectionHandler>(ioContext, messageHandler, logger);
}

void ConnectionHandler::handle()
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

void ConnectionHandler::post_message(const std::string& message)
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

void ConnectionHandler::handle_message(boost::system::error_code error, size_t messageSize)
{
    if (error) {
        _logger->log_error("Error occured while reading from socket: " + error.what());
        return;
    }

    if (_incomingBuffer.empty()) return;

    const std::string message = _incomingBuffer.substr(0, messageSize - 1);  // -1 strips delimeter
    _logger->log_debug("Recieved message: " + message);
    _messageHandler(message);

    _incomingBuffer.erase(0, messageSize);
}

void ConnectionHandler::write_from_queue()
{
    if (_isWriting) return;
    if (_outboxQueue.empty()) return;

    _isWriting = true;

    boost::asio::async_write(_socket,
        boost::asio::buffer(_outboxQueue.front() + ';'),
        [me = this->shared_from_this()](auto error, size_t bytes_written) {
            if (error) {
                me->_logger->log_error("Error occured while writing to socket: " + error.what());
            }
            else {
                me->_logger->log_debug("Sent message: " + me->_outboxQueue.front());
                me->_outboxQueue.pop();
                me->_queueCv.notify_one();
            }

            me->_isWriting = false;
            me->write_from_queue();
        }  //
    );
}

void ConnectionHandler::disconnect()
{
    _isConnected = false;
    _logger->log_warning("The client has disconnected");
}
}