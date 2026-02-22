#include "ShmemHandler.hpp"

namespace ipc = boost::interprocess;

namespace arguino::shmem {

ShmemHandler::ShmemHandler(const std::string& shmemName, size_t sizeInPages)
    : _name(shmemName),                                               //
      _pages(sizeInPages),                                            //
      _shmemObject(ipc::create_only, _name.c_str(), ipc::read_write)  //
{
    size_t shmem_size = _pages * ipc::mapped_region::get_page_size();
    _shmemObject.truncate(shmem_size);
    _writer = std::make_unique<ShmemWriter>(_shmemObject, 0, size() / 2);
}

ShmemHandler::~ShmemHandler()
{
    ipc::shared_memory_object::remove(_name.c_str());
}

void ShmemHandler::write(size_t offset, uint8_t byte)
{
    *(uint8_t*)_writer->at(offset) = byte;
}

size_t ShmemHandler::size()
{
    ipc::offset_t size;
    _shmemObject.get_size(size);
    return size;
}

}  // namespace arguino::shmem
