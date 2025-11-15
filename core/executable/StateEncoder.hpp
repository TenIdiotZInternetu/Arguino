//
// Created by touster on 12. 10. 2025.
//

#ifndef ARGUINO_CORE_STATEENCODER_HPP
#define ARGUINO_CORE_STATEENCODER_HPP


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

inline StateEncoder::state_t StateEncoder::decode(std::string message)
{
    // TODO implement decode
    return state_t{};
}


#endif  // ARGUINO_CORE_STATEENCODER_HPP
