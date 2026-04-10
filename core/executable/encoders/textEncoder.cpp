#include "../encoder.hpp"

using namespace arguino::simulator;

constexpr char COMMON_PART_FORMAT[] = "{:012}:{:07}:";
constexpr char WRITE_FORMAT[] = "W:{:02}:{}";
constexpr char PINMODE_FORMAT[] = "P:{:02}:{:c}";

static std::string encode_write(const Event& event)
{
    return std::format(WRITE_FORMAT, event.args[0], event.args[1]);
}

static std::string encode_pinmode(const Event& event)
{
    return std::format(PINMODE_FORMAT, event.args[0], event.args[1]);
}

std::string encode_event(const Event& event)
{
    std::string commonPart = std::format(
        COMMON_PART_FORMAT, event.timestampMicros, event.localVirtualTime);

    switch (event.type) {
        case Event::Type::Write:
            return commonPart + encode_write(event);
        case Event::Type::PinMode:
            return commonPart + encode_pinmode(event);
    }
}

Event decode_event(const std::string& message) {}