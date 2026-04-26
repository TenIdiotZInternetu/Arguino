#ifndef ARGUINO__MESSAGES_HPP
#define ARGUINO__MESSAGES_HPP

#include <string>

#include "ILogger.hpp"

namespace logger {

struct DebugMessage {
    std::string text;
    DebugMessage(std::string&& text)
        : text(text)
    {}

    LogLevel log_level() const { return LogLevel::Debug; }
    std::string what() const { return text; }
    std::string type() const { return "DEBUG"; }
};

struct InfoMessage {
    std::string text;
    InfoMessage(std::string&& text)
        : text(text)
    {}

    LogLevel log_level() const { return LogLevel::Info; }
    std::string what() const { return text; }
    std::string type() const { return "INFO"; }
};

struct WarningMessage {
    std::string text;
    WarningMessage(std::string&& text)
        : text(text)
    {}

    LogLevel log_level() const { return LogLevel::Warning; }
    std::string what() const { return text; }
    std::string type() const { return "WARNING"; }
};

struct ErrorMessage {
    std::string text;
    ErrorMessage(std::string&& text)
        : text(text)
    {}

    LogLevel log_level() const { return LogLevel::Error; }
    std::string what() const { return text; }
    std::string type() const { return "ERROR"; }
};

static_assert(IMessage<DebugMessage>);
static_assert(IMessage<InfoMessage>);
static_assert(IMessage<WarningMessage>);
static_assert(IMessage<ErrorMessage>);

}  // namespace logger

#endif