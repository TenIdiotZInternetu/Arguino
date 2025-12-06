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
    Error()
};
}  // namespace arguino::tcp::message