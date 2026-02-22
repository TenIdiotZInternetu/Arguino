#ifndef ARGUINO_CORE_SHMEMCONTROLLER_HPP
#define ARGUINO_CORE_SHMEMCONTROLLER_HPP

#include <boost/interprocess/mapped_region.hpp>
#include <boost/interprocess/shared_memory_object.hpp>
#include <memory>
#include <span>

#include "ShmemWriter.hpp"

namespace arguino::shmem {

class ShmemHandler {
   public:
    using shmem_t = boost::interprocess::shared_memory_object;

    ShmemHandler(const std::string& shmemName, size_t sizeInPages);
    ~ShmemHandler();

    template <typename T>
    void write(size_t offset, std::span<T> data);
    void write(size_t offset, uint8_t byte);

    std::string& name() { return _name; }
    size_t pages() { return _pages; }
    size_t size();

   private:
    std::string _name;
    size_t _pages;
    shmem_t _shmemObject;
    std::unique_ptr<ShmemWriter> _writer;
};  // namespace arguino::shmem

}  // namespace arguino::shmem


#endif