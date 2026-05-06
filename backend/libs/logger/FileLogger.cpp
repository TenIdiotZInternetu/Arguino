#include "FileLogger.hpp"

#include <stdexcept>

namespace logger {

FileLogger::FileLogger(const path_t& filePath, timer_fnct timerFunction)
    : _fileName(filePath), timerFunction(timerFunction)
{
    _file.open(_fileName);

    if (!_file.good()) {
        throw std::invalid_argument("File could not be opened: " + filePath.generic_string());
    }
}

void FileLogger::log(std::string&& message)
{
    _file << std::format("{}{}", formatted_time(), message) << std::endl;
}

void FileLogger::log(const IMessage& message)
{
    if (message.log_level() < verbosityLevel) return;

    log(std::format("[{}] {}", message.type(), message.what()));
}

logger::FileLogger::~FileLogger() {}

std::string FileLogger::formatted_time()
{
    if (timerFunction == nullptr) return "";

    // TODO: deltaTime is weird here
    auto micros = timerFunction();
    auto seconds = micros / 1'000'000;
    auto minutes = seconds / 60;

    seconds %= 60;
    micros %= 1'000'000;
    return std::format("[{:02}:{:02}.{:06}]", minutes, seconds, micros);
}

}  // namespace logger
