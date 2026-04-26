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
#include <memory>
#include <mutex>
#include <queue>

#include "ArduinoState.hpp"

namespace arguino::tcp {

class ConnectionHandler : public std::enable_shared_from_this<ConnectionHandler> {
   public:
    static constexpr char MESSAGE_DELIMITER = ';';
    static constexpr size_t MAX_QUEUE_SIZE = 100;

    using self_ptr = std::shared_ptr<ConnectionHandler>;
    using logger_ptr = std::shared_ptr<logger::ILogger>;
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

}  // namespace arguino::tcp


#endif  // ARGUINO_CORE_ARGUINOMESSAGEHANDLER_HPP
