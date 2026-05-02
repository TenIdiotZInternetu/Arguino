#include "ProducerConsumerWrapper.hpp"

#include "MemoryRegion.hpp"

namespace ipc = boost::interprocess;

namespace arguino::shmem {

ProducerConsumerWrapper::ProducerConsumerWrapper(const std::string& shmemName, size_t sizeInPages)
    : _name(shmemName),                                                  //
      _pages(sizeInPages),                                               //
      _shmemObject(ipc::open_or_create, _name.c_str(), ipc::read_write)  //
{
    size_t shmem_size = 2 * _pages * ipc::mapped_region::get_page_size();
    _shmemObject.truncate(shmem_size);
    _producer = CircularBuffer(_shmemObject, 0, shmem_size / 2);
    _consumer = CircularBuffer(_shmemObject, shmem_size / 2, shmem_size / 2);
}

ProducerConsumerWrapper::~ProducerConsumerWrapper()
{
    ipc::shared_memory_object::remove(_name.c_str());
}

size_t ProducerConsumerWrapper::size()
{
    ipc::offset_t size;
    _shmemObject.get_size(size);
    return size;
}

}  // namespace arguino::shmem
