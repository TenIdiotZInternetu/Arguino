#include "IpcHandler.hpp"

#include "MemoryRegion.hpp"

namespace ipc = boost::interprocess;

namespace arguino::shmem {

IpcHandler::IpcHandler(const std::string& shmemName, size_t sizeInPages)
    : _name(shmemName),                                               //
      _pages(sizeInPages),                                            //
      _shmemObject(ipc::create_only, _name.c_str(), ipc::read_write)  //
{
    size_t shmem_size = _pages * ipc::mapped_region::get_page_size();
    _shmemObject.truncate(shmem_size);
    _producer = CircularBuffer(_shmemObject, 0, size());
    _consumer = CircularBuffer(_shmemObject, 0, size());
}

IpcHandler::~IpcHandler()
{
    ipc::shared_memory_object::remove(_name.c_str());
}

size_t IpcHandler::size()
{
    ipc::offset_t size;
    _shmemObject.get_size(size);
    return size;
}

}  // namespace arguino::shmem
