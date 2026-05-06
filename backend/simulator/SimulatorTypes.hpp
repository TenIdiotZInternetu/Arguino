#ifndef ARGUINO_SIMULATOR_TYPES_HPP
#define ARGUINO_SIMULATOR_TYPES_HPP

#include <cstdint>
#include <string>

namespace arguino::simulator {

enum class PinMode { Out, In };

inline std::string pinmode_to_str(PinMode mode)
{
    switch (mode) {
        case PinMode::In:
            return "In";
        case PinMode::Out:
            return "Out";
        default:
            return std::format("Unknown ({})", (int)mode);
    };
}

using pin_t = uint8_t;
using digital_t = bool;
using analog_t = float;

}  // namespace arguino::simulator

#endif