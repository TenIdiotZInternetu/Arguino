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

    static constexpr size_t PRODUCER_PTR_LOCATION = 0;
    static constexpr size_t CONSUMER_PTR_LOCATION = 8;
    static constexpr size_t OVERHEAD_SIZE =  //
        sizeof(PRODUCER_PTR_LOCATION) +      //
        sizeof(CONSUMER_PTR_LOCATION);       //

    CircularBuffer(const shmem_t& shmemObject, size_t offset, size_t size);

    uint64_t producer_ptr();
    uint64_t consumer_ptr();

    size_t buffer_size() { return _memoryRegion->size() - OVERHEAD_SIZE; }

    template <typename T>
    bool write(T data);

    template <typename T>
    bool write(std::span<T> data);

    template <typename T>
    bool consume(T& out);

    template <typename T>
    bool consume(std::span<T>& out);

   private:
    std::unique_ptr<MemoryRegion> _memoryRegion;
    bool _ptrsAreFlipped;
};

template <typename T>
inline bool CircularBuffer::write(T data)
{
    return false;
}

template <typename T>
inline bool CircularBuffer::write(std::span<T> data)
{
    return false;
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