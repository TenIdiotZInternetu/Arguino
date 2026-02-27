#include "CircularBuffer.hpp"

namespace arguino::shmem {

CircularBuffer::CircularBuffer(const shmem_t& shmemObject, size_t offset, size_t size)
    : _memoryRegion(std::make_unique<MemoryRegion>(shmemObject, offset, size))
{
    *(uint64_t*)_memoryRegion->at(PRODUCER_PTR_LOCATION) = BUFFER_BEGIN;
    *(uint64_t*)_memoryRegion->at(CONSUMER_PTR_LOCATION) = BUFFER_BEGIN;
};

uint8_t* CircularBuffer::producer_ptr()
{
    return (uint8_t*)_memoryRegion->at(PRODUCER_PTR_LOCATION);
}

uint8_t* CircularBuffer::consumer_ptr()
{
    return (uint8_t*)_memoryRegion->at(CONSUMER_PTR_LOCATION);
}

size_t CircularBuffer::bytes_filled()
{
    return consumer_ptr() > producer_ptr()  //
             ? consumer_ptr() - producer_ptr()
             : producer_ptr() - consumer_ptr();
}

void CircularBuffer::update_producer_ptr(uint64_t shift)
{
    uint8_t* newPtr = producer_ptr() + shift;
    uint64_t offset = _memoryRegion->offset(newPtr);

    if (offset >= buffer_size()) {
        offset %= buffer_size();
        offset += BUFFER_BEGIN;
    }

    *(uint64_t*)_memoryRegion->at(PRODUCER_PTR_LOCATION) = offset;
}

}  // namespace arguino::shmem