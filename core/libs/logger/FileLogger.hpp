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
    using path_t = std::filesystem::path;
    using timer_ptr = std::shared_ptr<Timer>;

    LogLevel verbosityLevel = LogLevel::Info;
    timer_ptr timer;

    FileLogger(const path_t& filePath, timer_ptr timer = nullptr);

    void log(const IMessage& message) override;
    void log(std::string&& message) override;

    ~FileLogger();

   private:
    const std::filesystem::path _fileName;
    std::ofstream _file;

    std::string formatted_time();
};

}  // namespace logger

#endif