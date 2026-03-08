#include <cstdint>
#ifndef ARGUINO_SIMULATOR_TYPES_HPP
#define ARGUINO_SIMULATOR_TYPES_HPP

namespace arguino::simulator {

enum class PinMode { In, Out };

using pin_t = uint8_t;
using digital_t = bool;
using analog_t = float;

}  // namespace arguino::simulator

#endif