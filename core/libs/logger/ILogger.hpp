#ifndef LOGGER_ILOGGER_HPP
#define LOGGER_ILOGGER_HPP

#include <string>

#include "Messages.hpp"

namespace logger {

class ILogger {
   public:
    virtual void log(std::string&& text) const = 0;
    virtual void log(const IMessage& message) const = 0;

    void log_debug(std::string&& text) { log(DebugMessage(std::move(text))); }
    void log_info(std::string&& text) { log(InfoMessage(std::move(text))); }
    void log_warning(std::string&& text) { log(WarningMessage(std::move(text))); }
    void log_error(std::string&& text) { log(ErrorMessage(std::move(text))); }
};

struct DummyLogger : ILogger {
    void log(std::string&& message) {}
    void log(const IMessage& message) {}
};

}  // namespace logger

#endif