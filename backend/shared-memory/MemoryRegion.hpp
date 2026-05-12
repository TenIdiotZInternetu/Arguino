#ifndef ARGUINO_CORE_SHMEMWRITER_HPP
#define ARGUINO_CORE_SHMEMWRITER_HPP

#include <boost/interprocess/mapped_region.hpp>
#include <boost/interprocess/shared_memory_object.hpp>
#include <span>

namespace arguino::shmem {

class MemoryRegion {
   public:
    using shmem_t = boost::interprocess::shared_memory_object;
    using shmem_region_t = boost::interprocess::mapped_region;

    MemoryRegion(const shmem_t& shmemObject, size_t offset, size_t size);

    size_t size() { return _mappedRegion.get_size(); }
    uint8_t* begin() { return (uint8_t*)_mappedRegion.get_address(); }
    uint8_t* end() { return begin() + size(); }

    template <typename T>
    T& at(size_t offset);
    uint8_t* at(size_t offset) { return begin() + offset; }

    size_t offset(void* ptr) { return (uint8_t*)ptr - begin(); }

   private:
    shmem_region_t _mappedRegion;
};

template <typename T>
inline T& MemoryRegion::at(size_t offset)
{
    return *reinterpret_cast<T*>(at(offset));
}

}  // namespace arguino::shmem

#endif