#include "FileLogger.hpp"

#include <stdexcept>

logger::FileLogger::FileLogger(const path_t& filePath)
    : _fileName(filePath)
{
    _file.open(_fileName);

    if (!_file.good()) {
        throw std::invalid_argument("File could not be opened: " + filePath.generic_string());
    }
}

void logger::FileLogger::log(const std::string& message)
{
    _file << message << std::endl;
}

logger::FileLogger::~FileLogger() {}
