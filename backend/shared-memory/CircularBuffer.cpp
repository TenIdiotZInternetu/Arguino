#include "CircularBuffer.hpp"

#include <algorithm>
#include <vector>

namespace arguino::shmem {

CircularBuffer::CircularBuffer(const shmem_t& shmemObject, size_t offset, size_t size)
    : _memoryRegion(std::make_unique<MemoryRegion>(shmemObject, offset, size))
{
    _memoryRegion->at<uint64_t>(PRODUCER_PTR_LOCATION) = 0;
    _memoryRegion->at<uint64_t>(CONSUMER_PTR_LOCATION) = 0;
}

bool CircularBuffer::is_empty()
{
    return producer_it() == consumer_it();
}

std::vector<uint8_t> CircularBuffer::consume_until(uint8_t delimeter)
{
    auto delimeter_it = std::find(consumer_it(), producer_it(), delimeter);
}

CircularBuffer::iterator_t CircularBuffer::begin()
{
    return iterator_t(this, 0);
}

CircularBuffer::iterator_t CircularBuffer::end()
{
    return iterator_t(this, buffer_size());
}

CircularBuffer::iterator_t CircularBuffer::at(offset_t offset)
{
    return iterator_t(this, offset);
}

CircularBuffer::iterator_t CircularBuffer::producer_it()
{
    return iterator_t(this, _memoryRegion->at<offset_t>(PRODUCER_PTR_LOCATION));
}

CircularBuffer::iterator_t CircularBuffer::consumer_it()
{
    return iterator_t(this, _memoryRegion->at<offset_t>(CONSUMER_PTR_LOCATION));
}

size_t CircularBuffer::bytes_filled()
{
    return producer_it() - consumer_it();
}

CircularBuffer::iterator_t::reference CircularBuffer::iterator_t::operator*()
{
    return *(_parent->_memoryRegion->begin() + BUFFER_LOCATION + _offset);
}

CircularBuffer::iterator_t::pointer CircularBuffer::iterator_t::operator->()
{
    return &(operator*());
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

CircularBuffer::iterator_t CircularBuffer::iterator_t::operator+(iterator_t other)
{
    return *this + other._offset;
}

CircularBuffer::iterator_t CircularBuffer::iterator_t::operator+(difference_type shift)
{
    return iterator_t(_parent, (_offset + shift) % _parent->buffer_size());
}

CircularBuffer::iterator_t::difference_type CircularBuffer::iterator_t::operator-(iterator_t other)
{
    return _offset > other._offset  //
             ? _offset - other._offset
             : _parent->end()._offset - other._offset + _offset;
}

}  // namespace arguino::shmem