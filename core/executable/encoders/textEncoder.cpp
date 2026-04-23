#include <ranges>
#include <sstream>

#include "../encoder.hpp"

using namespace arguino::simulator;

constexpr char PART_DELIMETER = ':';
constexpr char COMMON_PART_FORMAT[] = "{:012}:{:07}:";
constexpr char WRITE_FORMAT[] = "W:{:02}:{}";
constexpr char PINMODE_FORMAT[] = "P:{:02}:{:c}";

constexpr int COMMON_PARTS = 3;
constexpr int MAX_ARGS = 3;
constexpr int MAX_PARTS = 6;

using parts_array_t = std::array<std::string_view, MAX_PARTS>;

static std::string encode_write(const Event& e)
{
    return std::format(WRITE_FORMAT, e.args[0], e.args[1]);
}

static std::string encode_pinmode(const Event& e)
{
    return std::format(PINMODE_FORMAT, e.args[0], e.args[1]);
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

bool decode_int(std::string_view str, int64_t& out)
{
    auto [ptr, ec] = std::from_chars(str.begin(), str.end(), out);
    return ec == std::errc();
}

bool decode_int(std::string_view str, int& out)
{
    auto [ptr, ec] = std::from_chars(str.begin(), str.end(), out);
    return ec == std::errc();
}

parts_array_t split(std::string_view messageView)
{
    parts_array_t parts;

    int partStart = 0;
    for (int i = 0; i < MAX_PARTS; ++i) {
        int delimIdx = messageView.find(PART_DELIMETER, partStart);
        parts[i] = messageView.substr(partStart, delimIdx - partStart);

        if (delimIdx == -1) break;
        partStart = delimIdx + 1;
    }

    return parts;
}

bool decode_event(const std::string& message, Event& event)
{
    parts_array_t parts = split(message);
    std::array<int, MAX_ARGS> args;

    for (int i = 0; i < MAX_ARGS; ++i) {
        if (!decode_int(parts[COMMON_PARTS + i + 1], args[i])) break;
    }

    if (parts[2] == "W") {
        event = Event::write(args[0], args[1]);
    }
    else if (parts[2] == "P") {
        event = Event::set_pinmode(args[0], (PinMode)args[1]);
    }
    else {
        return false;
    }

    if (!decode_int(parts[0], event.timestampMicros) ||  //
        !decode_int(parts[1], event.localVirtualTime)) {
        return false;
    }

    return true;
}