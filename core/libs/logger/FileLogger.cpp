#include "FileLogger.hpp"

#include <stdexcept>

namespace logger {

FileLogger::FileLogger(const path_t& filePath)
    : _fileName(filePath)
{
    _file.open(_fileName);

    if (!_file.good()) {
        throw std::invalid_argument("File could not be opened: " + filePath.generic_string());
    }
}

void FileLogger::log(std::string&& message)
{
    _file << message << std::endl;
}

void FileLogger::log(const IMessage& message)
{
    if (message.log_level() < verbosityLevel) return;

    // TODO: deltaTime is weird here
    log(std::format("[][{}] {}", timer->deltaTime(), message.type(), message.what()));
}

logger::FileLogger::~FileLogger() {}

}  // namespace logger
