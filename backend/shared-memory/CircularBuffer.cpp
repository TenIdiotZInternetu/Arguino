#include "CircularBuffer.hpp"

#include <math.h>

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
    iterator_t producerIt = producer_it();
    iterator_t consumerIt = consumer_it();

    auto delimeterIt = std::find(consumerIt, producerIt, delimeter);
    bool delimeterNotFound = delimeterIt == producerIt;
    if (delimeterNotFound) return {};

    size_t dataLength = delimeterIt - consumerIt + 1;
    size_t wrapsAround = dataLength > end() - consumerIt;

    std::vector<uint8_t> result(dataLength);

    if (!wrapsAround) {
        std::memcpy(&result[0], &*consumerIt, dataLength);
    }
    else {
        size_t firstBytesLength = end() - consumerIt;
        size_t lastBytesLength = delimeterIt - begin();

        std::memcpy(&result[0], &*consumerIt, firstBytesLength);
        std::memcpy(&result[firstBytesLength], &*begin(), lastBytesLength);
    }

    write_consumer(delimeterIt + 1);
    return result;
}

CircularBuffer::iterator_t CircularBuffer::begin()
{
    return iterator_t(this, 0);
}

CircularBuffer::iterator_t CircularBuffer::end()
{
    return iterator_t(this, buffer_size());
}

CircularBuffer::iterator_t CircularBuffer::at(size_t offset)
{
    return iterator_t(this, offset);
}

CircularBuffer::iterator_t CircularBuffer::producer_it()
{
    return iterator_t(this, _memoryRegion->at<size_t>(PRODUCER_PTR_LOCATION));
}

CircularBuffer::iterator_t CircularBuffer::consumer_it()
{
    return iterator_t(this, _memoryRegion->at<size_t>(CONSUMER_PTR_LOCATION));
}

void CircularBuffer::write_producer(iterator_t producer_it)
{
    _memoryRegion->at<size_t>(PRODUCER_PTR_LOCATION) = producer_it.offset;
}

void CircularBuffer::write_consumer(iterator_t consumer_it)
{
    _memoryRegion->at<size_t>(CONSUMER_PTR_LOCATION) = consumer_it.offset;
}

size_t CircularBuffer::bytes_filled()
{
    return producer_it() - consumer_it();
}

CircularBuffer::iterator_t::reference CircularBuffer::iterator_t::operator*()
{
    return *(_parent->_memoryRegion->begin() + BUFFER_LOCATION + offset);
}

CircularBuffer::iterator_t::pointer CircularBuffer::iterator_t::operator->()
{
    return &(operator*());
}

CircularBuffer::iterator_t CircularBuffer::iterator_t::operator++()
{
    offset++;
    if (offset == _parent->buffer_size()) {
        offset = 0;
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
    return *this + other.offset;
}

CircularBuffer::iterator_t CircularBuffer::iterator_t::operator+(difference_type shift)
{
    return iterator_t(_parent, (offset + shift) % _parent->buffer_size());
}

CircularBuffer::iterator_t::difference_type CircularBuffer::iterator_t::operator-(iterator_t other)
{
    return offset >= other.offset  //
             ? offset - other.offset
             : _parent->end().offset - other.offset + offset;
}

}  // namespace arguino::shmem