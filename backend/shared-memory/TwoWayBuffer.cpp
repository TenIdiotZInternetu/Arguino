#include "TwoWayBuffer.hpp"

#include "MemoryRegion.hpp"

namespace ipc = boost::interprocess;

namespace arguino::shmem {

constexpr int WINDOWS_PAGE_SIZE = 4096;

TwoWayBuffer::TwoWayBuffer(const std::string& shmemName, size_t sizeInPages)
    : _name(shmemName),                                                  //
      _pages(sizeInPages),                                               //
      _shmemObject(ipc::open_or_create, _name.c_str(), ipc::read_write)  //
{
#ifdef WIN32
    size_t shmem_size = 2 * _pages * WINDOWS_PAGE_SIZE;
#else
    size_t shmem_size = 2 * _pages * ipc::mapped_region::get_page_size();
#endif
    _shmemObject.truncate(shmem_size);
    _producer = CircularBuffer(_shmemObject, 0, shmem_size / 2);
    _consumer = CircularBuffer(_shmemObject, shmem_size / 2, shmem_size / 2);
}

TwoWayBuffer::~TwoWayBuffer()
{
    ipc::shared_memory_object::remove(_name.c_str());
}

std::vector<uint8_t> TwoWayBuffer::consume_until(uint8_t delimeter)
{
    return _consumer.consume_until(delimeter);
}

size_t TwoWayBuffer::size()
{
    ipc::offset_t size;
    _shmemObject.get_size(size);
    return size;
}

}  // namespace arguino::shmem
