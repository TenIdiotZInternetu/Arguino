#ifndef ARGUINO__MESSAGES_HPP
#define ARGUINO__MESSAGES_HPP

#include <string>

namespace logger {

enum class LogLevel { Debug, Info, Warning, Error };

struct IMessage {
    std::string text;

    virtual LogLevel log_level() const = 0;
    virtual const std::string& what() const = 0;
    virtual std::string type() const = 0;
};

struct DebugMessage : public IMessage {
    std::string text;
    DebugMessage(std::string&& text)
        : text(text)
    {}

    LogLevel log_level() const { return LogLevel::Debug; }
    const std::string& what() const { return text; }
    std::string type() const { return "DEBUG"; }
};

struct InfoMessage : public IMessage {
    std::string text;
    InfoMessage(std::string&& text)
        : text(text)
    {}

    LogLevel log_level() const { return LogLevel::Info; }
    const std::string& what() const { return text; }
    std::string type() const { return "INFO"; }
};

struct WarningMessage : public IMessage {
    std::string text;
    WarningMessage(std::string&& text)
        : text(text)
    {}

    LogLevel log_level() const { return LogLevel::Warning; }
    const std::string& what() const { return text; }
    std::string type() const { return "WARNING"; }
};

struct ErrorMessage : public IMessage {
    std::string text;
    ErrorMessage(std::string&& text)
        : text(text)
    {}

    LogLevel log_level() const { return LogLevel::Error; }
    const std::string& what() const { return text; }
    std::string type() const { return "ERROR"; }
};

}  // namespace logger

#endif