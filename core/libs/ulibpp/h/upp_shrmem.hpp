#ifndef ULIBPP_UPP_SHRMEM_HPP_
#define ULIBPP_UPP_SHRMEM_HPP_

/*

	upp_shrmem.hpp

	Shared memory

	Kuba, 2007
	Part of ULIB++

*/

#include <stdexcept>
#include <string>
#include <cassert>

#include "upp_def.hpp"

#if ULIBPP_OS_PLATFORM == ULIBPP_OS_PLATFORM_WIN_NT ||		\
	ULIBPP_OS_PLATFORM == ULIBPP_OS_PLATFORM_WIN_NT64

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <windows.h>

#elif ULIBPP_OS_PLATFORM == ULIBPP_OS_PLATFORM_LINUX ||		\
	  ULIBPP_OS_PLATFORM == ULIBPP_OS_PLATFORM_K1OM

#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE				4
#endif
#include <sys/mman.h>
#include <sys/fcntl.h>

#else
#error Unsupported OS
#endif

namespace ulibpp {

	namespace ulibpp_private_ {

		class shrm_attorney;

	}

	class shared_memory_mapping;

	class shared_memory {
	public:
		// flags
		typedef std::uint32_t flag_type;
		static const flag_type read = static_cast<flag_type>(0x00000001);
		static const flag_type write = static_cast<flag_type>(0x00000002);
		static const flag_type execute = static_cast<flag_type>(0x00000004);
		// constructors, destructors
		shared_memory() : memobj_(invalid_handle) { }
		~shared_memory();
		// move semantics
		shared_memory(const shared_memory &) = delete;
		shared_memory &operator=(const shared_memory &) = delete;
		shared_memory(shared_memory &&sm);
		shared_memory &operator=(shared_memory &&sm);

		// create & open
		void create(const std::string &name, std::size_t size, flag_type flags=read|write);
		void open(const std::string &name, std::size_t size, flag_type flags=read|write);
		void create(const std::wstring &name, std::size_t size, flag_type flags=read|write);
		void open(const std::wstring &name, std::size_t size, flag_type flags=read|write);

		// infos
		std::size_t get_mem_alloc_granularity() const { return mag_; }
		std::size_t get_mem_page_size() const { return mps_; }
		std::size_t get_size() const { return size_; }
	private:
		std::size_t mag_, mps_;
		std::size_t size_;
		bool		has_exec_;
		bool		created_;

#if ULIBPP_OS_PLATFORM == ULIBPP_OS_PLATFORM_WIN_NT ||		\
	ULIBPP_OS_PLATFORM == ULIBPP_OS_PLATFORM_WIN_NT64

		template <typename TR>
		void construct_shmem(const typename TR::string_type &name, std::size_t size, flag_type flags, bool create);

		typedef ::HANDLE shrm_handle_type;

#elif ULIBPP_OS_PLATFORM == ULIBPP_OS_PLATFORM_LINUX ||		\
	  ULIBPP_OS_PLATFORM == ULIBPP_OS_PLATFORM_K1OM

		template <typename TR>
		void construct_shmem(const typename TR::stype &name, std::size_t size, flag_type flags, bool create);

		typedef int shrm_handle_type;
		
		std::string		shm_name_;

#else
#error Unsupported OS
#endif
		static const shrm_handle_type invalid_handle;

		shrm_handle_type memobj_;

		friend class ulibpp_private_::shrm_attorney;
	};

	namespace ulibpp_private_ {

		class shrm_attorney {
		public:
			static shared_memory::shrm_handle_type get_handle(const shared_memory &shm) { return shm.memobj_; }
			static bool has_exec(const shared_memory &shm) { return shm.has_exec_; }
		private:
			friend class shared_memory_mapping;
		};

	}

/*
	offset must be a multiple of the system's memory allocaiton granularity
	access_flags may be subset from open (e.g. opened with R/W, map with R/O)
	len==0 -> map all

	On unices, sometimes some system tweaking needs to be done for shared memory
	to work; eg. on linux, 2.4 and 2.6 kernel require for shared memory:
	1) /dev/shm must exist
	2) it should be tmpfs mountpoint (ie. "mount -t tmpfs none /dev/shm")
*/
	class shared_memory_mapping {
	public:
		// constructors & destructor
		shared_memory_mapping() : addr_(invalid_addr) {}
		~shared_memory_mapping();

		// move semantics
		shared_memory_mapping(const shared_memory_mapping &) = delete;
		shared_memory_mapping &operator=(const shared_memory_mapping &) = delete;
		shared_memory_mapping(shared_memory_mapping &&sm) { addr_ = sm.addr_; sm.addr_ = invalid_addr; size_ = sm.size_; }
		shared_memory_mapping &operator=(shared_memory_mapping &&sm) { if(sm.addr_!=addr_) {shared_memory_mapping t(std::move(sm)); std::swap(addr_, t.addr_); std::swap(size_, t.size_); } return *this; }

		// mapping
		void *map(shared_memory &shm, std::size_t offset, std::size_t size, shared_memory::flag_type flags=shared_memory::read|shared_memory::write, void *addr_hint=0);

		void *get_addr() const { assert(addr_!=invalid_addr); return addr_; }
	private:
		static void *invalid_addr;

		void			*addr_;
		std::size_t		size_;
	};

}

#endif
