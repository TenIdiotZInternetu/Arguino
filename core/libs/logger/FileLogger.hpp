#ifndef LOGGER_FILELOGGER_HPP
#define LOGGER_FILELOGGER_HPP

#include <filesystem>
#include <fstream>
#include <string>

#include "ILogger.hpp"
#include "Messages.hpp"
#include "Timer.hpp"

namespace logger {

class FileLogger : public ILogger {
   public:
    LogLevel verbosityLevel = LogLevel::Info;
    std::shared_ptr<Timer> timer;

    using path_t = std::filesystem::path;
    FileLogger(const path_t& filePath);

    void log(const IMessage& message) override;
    void log(std::string&& message) override;

    ~FileLogger();

   private:
    const std::filesystem::path _fileName;
    std::ofstream _file;
};

}  // namespace logger

#endif