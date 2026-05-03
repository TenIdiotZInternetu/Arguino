#ifndef ARGUINO_CORE_SHMEMCONTROLLER_HPP
#define ARGUINO_CORE_SHMEMCONTROLLER_HPP

#include <boost/interprocess/mapped_region.hpp>
#include <boost/interprocess/shared_memory_object.hpp>
#include <iostream>
#include <memory>
#include <span>
#include <vector>

#include "CircularBuffer.hpp"
#include "MemoryRegion.hpp"

namespace arguino::shmem {

class TwoWayBuffer {
   public:
    using shmem_t = boost::interprocess::shared_memory_object;

    TwoWayBuffer(const std::string& shmemName, size_t sizeInPages);
    ~TwoWayBuffer();

    template <typename T>
    void write(const T& data);

    std::string& name() { return _name; }
    size_t pages() { return _pages; }
    size_t size();

   private:
    std::string _name;
    size_t _pages;
    shmem_t _shmemObject;
    CircularBuffer _producer;
    CircularBuffer _consumer;
};  // namespace arguino::shmem


template <typename T>
inline void TwoWayBuffer::write(const T& data)
{
    if (!_producer.write(data)) {
        std::cout << "Not enough space" << std::endl;
    }
}

}  // namespace arguino::shmem


#endif