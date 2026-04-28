#ifndef ARGUINO_ENCODER_HPP
#define ARGUINO_ENCODER_HPP

#include <format>
#include <string>

#include "Events.hpp"


std::string encode_event(const arguino::simulator::Event& event);
bool decode_event(const std::string& message, arguino::simulator::Event& event);

#endif