#ifndef ARGUINO__SHMEMCIRCULARBUFFER_HPP
#define ARGUINO__SHMEMCIRCULARBUFFER_HPP

#include <memory>
#include <ranges>
#include <span>
#include <vector>

#include "MemoryRegion.hpp"

namespace arguino::shmem {

class CircularBuffer {
   public:
    struct iterator_t;
    friend struct iterator_t;

    using shmem_t = boost::interprocess::shared_memory_object;

    static constexpr size_t PRODUCER_PTR_LOCATION = 0;
    static constexpr size_t CONSUMER_PTR_LOCATION = 8;
    static constexpr size_t BUFFER_LOCATION = 16;

    CircularBuffer() = default;
    CircularBuffer(const shmem_t& shmemObject, size_t offset, size_t size);

    bool is_empty();
    size_t buffer_size() { return _memoryRegion->size() - BUFFER_LOCATION; }

    size_t bytes_filled();
    size_t bytes_available() { return buffer_size() - bytes_filled(); }

    template <typename T>
        requires std::is_trivially_copyable_v<T>
              && (!std::ranges::contiguous_range<T>)  // Prevents recursion
    bool write(const T& data);

    template <std::ranges::contiguous_range TRange>
    bool write(const TRange& data);

    std::vector<uint8_t> consume_until(uint8_t delimeter);

   private:
    std::unique_ptr<MemoryRegion> _memoryRegion;

    iterator_t begin();
    iterator_t end();
    iterator_t at(uint64_t offset);

    iterator_t producer_it();
    iterator_t consumer_it();

    void write_producer(iterator_t producer_it);
    void write_consumer(iterator_t consumer_it);
};


struct CircularBuffer::iterator_t {
   public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = uint8_t;
    using difference_type = size_t;
    using pointer = uint8_t*;
    using reference = uint8_t&;

    iterator_t() = default;
    iterator_t(CircularBuffer* parent)
        : _parent(parent)
    {}
    iterator_t(CircularBuffer* parent, size_t offset)
        : _parent(parent), offset(offset)
    {}

    uint64_t offset;

    reference operator*();
    pointer operator->();
    bool operator==(iterator_t other) const { return offset == other.offset; };
    bool operator!=(iterator_t other) { return !(*this == other); }
    iterator_t operator++();
    iterator_t operator++(int);
    iterator_t operator+(iterator_t other);
    iterator_t operator+(difference_type shift);
    difference_type operator-(iterator_t other);

   private:
    CircularBuffer* _parent;
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

    size_t bytesUntilBufferEnd = end() - producer_it();

    if (bytes.size() < bytesUntilBufferEnd) {
        std::memcpy(&*producer_it(), bytes.data(), bytes.size());
    }
    else {
        auto firstBytes = bytes.first(bytesUntilBufferEnd);
        auto lastBytes = bytes.last(bytes.size() - bytesUntilBufferEnd);

        std::memcpy(&*producer_it(), firstBytes.data(), firstBytes.size());
        std::memcpy(&*begin(), lastBytes.data(), lastBytes.size());
    }

    write_producer(producer_it() + bytes.size());
    return true;
}

}  // namespace arguino::shmem


#endif