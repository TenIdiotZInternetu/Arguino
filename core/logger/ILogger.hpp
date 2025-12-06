#ifndef LOGGER_ILOGGER_HPP
#define LOGGER_ILOGGER_HPP

namespace logger {

template <typename T>
concept IMessage = requires(T message) {
    { message.what() } -> std::convertible_to<std::string>;
    { message.type() } -> std::convertible_to<std::string>;
};

template <typename T>
concept ILogger = requires(T logger, const std::string& str) {
    { logger.log(str) } -> std::same_as<void>;
    // { logger.log(msg) } -> std::same_as<void>;
};

}  // namespace logger

#endif