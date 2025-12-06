#include <format>
#include <string>

#include "ILogger.hpp"

namespace arguino::simulator::message {

class Read {
   public:
    Read(const std::string& writtenData)
        : _data(writtenData)
    {}

    std::string type() { return "ARDUINO:WRITE"; }
    std::string what() { return std::format("Write data '{}' to simulator", _data); }

   private:
    std::string _data;
};
}