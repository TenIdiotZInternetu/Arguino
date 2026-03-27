#include "../encoder.hpp"

using namespace arguino::simulator;

static std::string encode_write(const Event& event)
{
    return std::format("{:07}:W:{:02}:{}", event.localVirtualTime, event.args[0], event.args[1]);
}

static std::string encode_pinmode(const Event& event)
{
    return std::format("{:07}:W:{:02}:{}", event.localVirtualTime, event.args[0], event.args[1]);
}

std::string encode_event(const Event& event)
{
    switch (event.type) {
        case Event::Type::Write:
            return encode_write(event);
        case Event::Type::PinMode:
            return encode_pinmode(event);
    }
}

Event decode_event(const std::string& message) {}