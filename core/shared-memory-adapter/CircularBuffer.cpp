#include "CircularBuffer.hpp"

namespace arguino::shmem {

CircularBuffer::CircularBuffer(const shmem_t& shmemObject, size_t offset, size_t size)
    : _memoryRegion(std::make_unique<MemoryRegion>(shmemObject, offset, size)),
      _nextProducerOffset(0)
{
    _memoryRegion->at<uint64_t>(PRODUCER_PTR_LOCATION) = 0;
    _memoryRegion->at<uint64_t>(CONSUMER_PTR_LOCATION) = 0;
};

CircularBuffer::offset_t& CircularBuffer::producer_offset()
{
    return _memoryRegion->at<offset_t>(PRODUCER_PTR_LOCATION);
}

CircularBuffer::offset_t& CircularBuffer::consumer_offset()
{
    return _memoryRegion->at<offset_t>(CONSUMER_PTR_LOCATION);
}

size_t CircularBuffer::bytes_filled()
{
    return consumer_offset() > producer_offset()  //
             ? consumer_offset() - producer_offset()
             : producer_offset() - consumer_offset();
}

CircularBuffer::offset_t CircularBuffer::next_producer_offset(size_t shift)
{
    offset_t newOffset = producer_offset() + shift;
    return newOffset % buffer_size();
}

}  // namespace arguino::shmem