#ifndef LOGGER_FILELOGGER_HPP
#define LOGGER_FILELOGGER_HPP

#include <filesystem>
#include <fstream>
#include <string>

#include "ILogger.hpp"
#include "Messages.hpp"
#include "Timer.hpp"

namespace logger {

class FileLogger {
   public:
    LogLevel verbosityLevel = LogLevel::Info;
    std::shared_ptr<Timer> timer;

    using path_t = std::filesystem::path;
    FileLogger(const path_t& filePath);

    template <IMessage TMessage>
    void log(TMessage message);
    void log(std::string&& message);

    void log_debug(std::string&& message) { log(DebugMessage(std::move(message))); }
    void log_info(std::string&& message) { log(InfoMessage(std::move(message))); }
    void log_warning(std::string&& message) { log(WarningMessage(std::move(message))); }
    void log_error(std::string&& message) { log(ErrorMessage(std::move(message))); }

    ~FileLogger();

   private:
    const std::filesystem::path _fileName;
    std::ofstream _file;
};


template <IMessage TMessage>
inline void FileLogger::log(TMessage message)
{
    if (message.log_level() < verbosityLevel) return;

    // TODO: deltaTime is weird here
    log(std::format("[][{}] {}", timer->deltaTime(), message.type(), message.what()));
}

}  // namespace logger

#endif