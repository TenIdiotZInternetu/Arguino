#ifndef ARGUINO__SHMEMCIRCULARBUFFER_HPP
#define ARGUINO__SHMEMCIRCULARBUFFER_HPP

#include <memory>
#include <ranges>
#include <span>

#include "MemoryRegion.hpp"

namespace arguino::shmem {

class CircularBuffer {
   public:
    using shmem_t = boost::interprocess::shared_memory_object;
    using offset_t = uint64_t;

    static constexpr size_t PRODUCER_PTR_LOCATION = 0;
    static constexpr size_t CONSUMER_PTR_LOCATION = 8;
    static constexpr size_t BUFFER_LOCATION = sizeof(PRODUCER_PTR_LOCATION)
                                            + sizeof(CONSUMER_PTR_LOCATION);

    CircularBuffer() = default;
    CircularBuffer(const shmem_t& shmemObject, size_t offset, size_t size);

    size_t buffer_size() { return _memoryRegion->size() - BUFFER_LOCATION; }

    size_t bytes_filled();
    size_t bytes_available() { return buffer_size() - bytes_filled(); }

    template <typename T>
        requires std::is_trivially_copyable_v<T>
              && (!std::ranges::contiguous_range<T>)  // Prevents recursion
    bool write(const T& data);

    template <std::ranges::contiguous_range TRange>
    bool write(const TRange& data);

    template <typename T>
    bool consume(T& out);

    template <typename T>
    bool consume(std::span<T>& out);

   private:
    std::unique_ptr<MemoryRegion> _memoryRegion;
    offset_t _nextProducerOffset;

    uint8_t* buffer_begin() { return _memoryRegion->begin() + BUFFER_LOCATION; }
    uint8_t* at(offset_t offset) { return buffer_begin() + offset; }

    offset_t& producer_offset();
    offset_t& consumer_offset();

    offset_t next_producer_offset(offset_t shift);
};

template <typename T>
    requires std::is_trivially_copyable_v<T> && (!std::ranges::contiguous_range<T>)
inline bool CircularBuffer::write(const T& data)
{
    return write(std::span<const T, 1>(&data, 1));
}

template <std::ranges::contiguous_range TRange>
inline bool CircularBuffer::write(const TRange& data)
{
    auto bytes = std::as_bytes(std::span(data));

    if (bytes_available() <= bytes.size()) {  // We don't want the buffer to be fully filled
        return false;
    }

    offset_t bytesUntilBufferEnd = buffer_size() - _nextProducerOffset;

    if (bytes.size() < bytesUntilBufferEnd) {
        std::memcpy(at(_nextProducerOffset), bytes.data(), bytes.size());
    }
    else {
        auto firstBytes = bytes.first(bytesUntilBufferEnd);
        auto lastBytes = bytes.last(bytes.size() - bytesUntilBufferEnd);

        std::memcpy(at(_nextProducerOffset), firstBytes.data(), firstBytes.size());
        std::memcpy(buffer_begin(), lastBytes.data(), lastBytes.size());
    }

    producer_offset() = _nextProducerOffset;
    _nextProducerOffset = next_producer_offset(bytes.size());
    return true;
}

template <typename T>
inline bool CircularBuffer::consume(T& out)
{
    return false;
}

template <typename T>
inline bool CircularBuffer::consume(std::span<T>& out)
{
    return false;
}

}  // namespace arguino::shmem


#endif