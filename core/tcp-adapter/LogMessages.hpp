#ifndef ARGUINO_CORE_TCP_LOGMESSAGES_HPP
#define ARGUINO_CORE_TCP_LOGMESSAGES_HPP

#include <boost/system/error_code.hpp>
#include <format>
#include <string>

#include "ILogger.hpp"

namespace arguino::tcp::message {

class Read {
   public:
    Read(const std::string& readData)
        : _data(readData)
    {}

    std::string type() { return "TCP:READ"; }
    std::string what() { return std::format("Read data '{}' from simulator", _data); }

   private:
    std::string _data;
};

class Write {
   public:
    Write(const std::string& writtenData)
        : _data(writtenData)
    {}

    std::string type() { return "TCP:WRITE"; }
    std::string what() { return std::format("Written data '{}' to simulator", _data); }

   private:
    std::string _data;
};

class Error {
   public:
    Error(const std::string& infoMessage, const boost::system::error_code& error)
        : _infoMessage(infoMessage), _error(error)
    {}

    std::string type() { return "TCP:ERROR"; }
    std::string what() { return _infoMessage + _error.message(); }

   private:
    std::string _infoMessage;
    boost::system::error_code _error;
};
}  // namespace arguino::tcp::message

#endif