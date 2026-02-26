#include <MemoryRegion.hpp>

namespace ipc = boost::interprocess;

namespace arguino::shmem {

MemoryRegion::MemoryRegion(const shmem_t& shmemObject, size_t offset, size_t size)
    : _mappedRegion(shmemObject, ipc::read_write, offset, size)
{}


}  // namespace arguino::shmem
