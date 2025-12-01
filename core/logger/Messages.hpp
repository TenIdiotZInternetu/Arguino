#include <string>

#include "ILogger.hpp"

namespace logger::message {

class Read {
   public:
    Read(const std::string& state)
        : _state(state)
    {}

    std::string type() { return "READ"; }
    std::string what() { return _state; }

   private:
    std::string _state;
};

}  // namespace logger