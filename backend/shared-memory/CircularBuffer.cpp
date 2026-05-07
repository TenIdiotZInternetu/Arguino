#include "CircularBuffer.hpp"

#include <algorithm>
#include <vector>

namespace arguino::shmem {

CircularBuffer::CircularBuffer(const shmem_t& shmemObject, size_t offset, size_t size)
    : _memoryRegion(std::make_unique<MemoryRegion>(shmemObject, offset, size)),
      _nextProducerOffset(0)
{
    _memoryRegion->at<uint64_t>(PRODUCER_PTR_LOCATION) = 0;
    _memoryRegion->at<uint64_t>(CONSUMER_PTR_LOCATION) = 0;
};

std::vector<std::byte> CircularBuffer::consume_until(std::byte delimeter)
{
    auto consumer_it = _memoryRegion->at(consumer_offset());
    auto delimeter_it = std::find(             //
        _memoryRegion->at(consumer_offset()),  //
        _memoryRegion->at(producer_offset()),  //
        delimeter);
}

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

CircularBuffer::iterator_t::reference CircularBuffer::iterator_t::operator*()
{
    return _parent->_memoryRegion->begin() + BUFFER_LOCATION + _offset;
}

CircularBuffer::iterator_t::pointer CircularBuffer::iterator_t::operator&()
{
    return &*(*this);
}

CircularBuffer::iterator_t CircularBuffer::iterator_t::operator++()
{
    _offset++;
    if (_offset == _parent->buffer_size()) {
        _offset = 0;
    }
    return *this;
}

CircularBuffer::iterator_t CircularBuffer::iterator_t::operator++(int)
{
    iterator_t tmp = *this;
    ++(*this);
    return tmp;
}

}  // namespace arguino::shmem