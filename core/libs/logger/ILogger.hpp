#ifndef LOGGER_ILOGGER_HPP
#define LOGGER_ILOGGER_HPP

#include <string>

namespace logger {

enum class LogLevel { Debug, Info, Warning, Error };

template <typename T>
concept IMessage = requires(T message) {
    { message.log_level() } -> std::same_as<LogLevel>;
    { message.what() } -> std::convertible_to<std::string>;
    { message.type() } -> std::convertible_to<std::string>;
};

template <typename T>
concept ILogger = requires(T logger, std::string&& str) {
    { logger.log(std::move(str)) } -> std::same_as<void>;
    // { logger.log(msg) } -> std::same_as<void>;

    { logger.log_debug(std::move(str)) } -> std::same_as<void>;
    { logger.log_info(std::move(str)) } -> std::same_as<void>;
    { logger.log_warning(std::move(str)) } -> std::same_as<void>;
    { logger.log_error(std::move(str)) } -> std::same_as<void>;
};

}  // namespace logger

#endif