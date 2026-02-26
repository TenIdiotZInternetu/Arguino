#include "CircularBuffer.hpp"

namespace arguino::shmem {

CircularBuffer::CircularBuffer(const shmem_t& shmemObject, size_t offset, size_t size)
    : _memoryRegion(std::make_unique<MemoryRegion>(shmemObject, offset, size)) {};

uint64_t CircularBuffer::producer_ptr()
{
    return *(uint64_t*)(_memoryRegion->at(PRODUCER_PTR_LOCATION));
}

uint64_t CircularBuffer::consumer_ptr()
{
    return *(uint64_t*)(_memoryRegion->at(CONSUMER_PTR_LOCATION));
}

}  // namespace arguino::shmem