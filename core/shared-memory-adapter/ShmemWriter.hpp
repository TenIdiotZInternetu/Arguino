#ifndef ARGUINO_CORE_SHMEMWRITER_HPP
#define ARGUINO_CORE_SHMEMWRITER_HPP

#include <boost/interprocess/mapped_region.hpp>
#include <boost/interprocess/shared_memory_object.hpp>
#include <span>

namespace arguino::shmem {

class ShmemWriter {
   public:
    using shmem_t = boost::interprocess::shared_memory_object;
    using shmem_region_t = boost::interprocess::mapped_region;

    ShmemWriter(const shmem_t& shmemObject, size_t offset, size_t size);

    size_t size() { return _mappedRegion.get_size(); }
    void* begin() { return _mappedRegion.get_address(); }
    void* end() { return (uint8_t*)begin() + size(); }
    void* at(size_t offset) { return (uint8_t*)begin() + offset; }

   private:
    shmem_region_t _mappedRegion;
};

}  // namespace arguino::shmem

#endif