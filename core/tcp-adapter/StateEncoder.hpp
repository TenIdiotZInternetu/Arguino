//
// Created by touster on 12. 10. 2025.
//

#ifndef ARGUINO_CORE_STATEENCODER_HPP
#define ARGUINO_CORE_STATEENCODER_HPP

namespace arguino::tcp {

class StateEncoder {
   public:
    using state_t = ArduinoState;
    std::string encode(state_t state);
    state_t decode(std::string message);
};

inline std::string StateEncoder::encode(state_t state)
{
    std::string message;
    for (auto&& pinOn : state.get_digital()) {
        message += std::format(":{}", pinOn ? 1 : 0);
    }

    // TODO: implement other state variables
    return message;
}

// Example of a message is :0:0:0:1:1:... with 14 pin values
inline StateEncoder::state_t StateEncoder::decode(std::string message)
{
    state_t newState{};
    uint8_t messageIndex = 1;

    // TODO: Validate Message

    for (auto&& pin : newState.get_digital()) {
        pin = message[messageIndex] == '1' ? true : false;
        messageIndex += 2;
    }
    return newState;
}

}  // namespace arguino::tcp

#endif  // ARGUINO_CORE_STATEENCODER_HPP
