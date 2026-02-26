#ifndef ARGUINO_CORE_SHMEMCONTROLLER_HPP
#define ARGUINO_CORE_SHMEMCONTROLLER_HPP

#include <boost/interprocess/mapped_region.hpp>
#include <boost/interprocess/shared_memory_object.hpp>
#include <memory>
#include <span>
#include <vector>

#include "MemoryRegion.hpp"

namespace arguino::shmem {

class IpcHandler {
   public:
    using shmem_t = boost::interprocess::shared_memory_object;

    IpcHandler(const std::string& shmemName, size_t sizeInPages);
    ~IpcHandler();

    void write(size_t offset, std::span<uint8_t> bytes);
    void write(size_t offset, uint8_t byte);

    uint8_t read(size_t offset);
    std::vector<uint8_t> read(size_t offset, size_t byte_count);

    std::string& name() { return _name; }
    size_t pages() { return _pages; }
    size_t size();

   private:
    std::string _name;
    size_t _pages;
    shmem_t _shmemObject;
    std::unique_ptr<MemoryRegion> _producer;
    std::unique_ptr<MemoryRegion> _consumer;
};  // namespace arguino::shmem

}  // namespace arguino::shmem


#endif