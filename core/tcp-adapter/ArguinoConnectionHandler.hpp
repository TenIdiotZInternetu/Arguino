//
// Created by touster on 12. 10. 2025.
//

#ifndef ARGUINO_CORE_ARGUINOMESSAGEHANDLER_HPP
#define ARGUINO_CORE_ARGUINOMESSAGEHANDLER_HPP

#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/read_until.hpp>
#include <boost/asio/write.hpp>

#include "ArduinoState.hpp"

namespace arguino::tcp {

template <typename T>
concept Encoder = requires(T encoder, std::string msg, ArduinoState state) {
    { encoder.encode(state) } -> std::convertible_to<std::string>;
    { encoder.decode(msg) } -> std::same_as<ArduinoState>;
};

template <Encoder TEncoder>
class ArguinoConnectionHandler
    : public std::enable_shared_from_this<ArguinoConnectionHandler<TEncoder>> {
   public:
    static constexpr char MESSAGE_DELIMITER = ';';
    static constexpr char READ_FLAG = 'R';
    static constexpr char WRITE_FLAG = 'W';

    using pointer_t = std::shared_ptr<ArguinoConnectionHandler<TEncoder>>;

    ArguinoConnectionHandler(boost::asio::io_context& ioContext);

    static pointer_t create(boost::asio::io_context& ioContext);
    void handle();
    boost::asio::ip::tcp::socket& socket() { return _socket; }

   private:
    boost::asio::ip::tcp::socket _socket;
    std::string _buffer;
    std::string _outcomingMessage;

    void on_read_message(boost::system::error_code& error, size_t messageSize);
    void handle_read_state(const std::string& message);
    void handle_write_state(const std::string& message);
};

template <Encoder TEncoder>
ArguinoConnectionHandler<TEncoder>::ArguinoConnectionHandler(boost::asio::io_context& ioContext)
    : _socket(ioContext)
{}

template <Encoder TEncoder>
ArguinoConnectionHandler<TEncoder>::pointer_t ArguinoConnectionHandler<TEncoder>::create(
    boost::asio::io_context& ioContext)
{
    return std::make_shared<ArguinoConnectionHandler>(ioContext);
}

template <Encoder TEncoder>
void ArguinoConnectionHandler<TEncoder>::handle()
{
    boost::asio::async_read_until(_socket,
        boost::asio::dynamic_buffer(_buffer),
        MESSAGE_DELIMITER,
        [me = this->shared_from_this()](
            auto error, size_t messageSize) { me->on_read_message(error, messageSize); });
}

template <Encoder TEncoder>
void ArguinoConnectionHandler<TEncoder>::on_read_message(
    boost::system::error_code& error, size_t messageSize)
{
    if (error || _buffer.empty()) {
        // TODO: log
        std::cout << error.message() << std::endl;
        return;
    }

    // TODO: expect timestamp in the message

    const std::string& message = _buffer.substr(0, messageSize);

    if (message[0] == READ_FLAG) {
        handle_read_state(message);
    }
    if (message[0] == WRITE_FLAG) {
        handle_write_state(message);
    }

    _buffer.erase(0, messageSize);
    handle();
}

template <Encoder TEncoder>
void ArguinoConnectionHandler<TEncoder>::handle_read_state(const std::string& message)
{
    TEncoder encoder;

    // TODO get rid of this global reference
    _outcomingMessage = encoder.encode(CanonicalState::state()) + MESSAGE_DELIMITER;

    boost::asio::async_write(_socket,
        boost::asio::buffer(_outcomingMessage),
        [me = this->shared_from_this()](auto error, size_t bytes_written) {
            // TODO another log maybe?
        });
}

template <Encoder TEncoder>
void ArguinoConnectionHandler<TEncoder>::handle_write_state(const std::string& message)
{
    TEncoder encoder;
    ArduinoState newState = encoder.decode(message.substr(1));  // skip write flag
    CanonicalState::update_state(newState);
    // TODO log
}

}  // namespace arguino::tcp


#endif  // ARGUINO_CORE_ARGUINOMESSAGEHANDLER_HPP
