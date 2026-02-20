/*

	upp_shrmem.cpp

	Kuba, 2007
	Part of ULIB++

*/

#include <stdexcept>
#include <cassert>

#include "upp_shrmem.hpp"
#include "upp_mthr.hpp"

using namespace std;

#if ULIBPP_OS_PLATFORM==ULIBPP_OS_PLATFORM_WIN_NT ||		\
	ULIBPP_OS_PLATFORM==ULIBPP_OS_PLATFORM_WIN_NT64

#if ULIBPP_CC_PLATFORM==ULIBPP_CC_PLATFORM_MSVC && ULIBPP_CC_VERSION >= 1800
#include <VersionHelpers.h>
#endif

namespace {

	struct shmem_char_tr {
		typedef string string_type;
		static string_type patch_name(bool vistaplus, const string_type &name) { return (vistaplus ? "" : "Global\\") + name; }
		static ::HANDLE create_file_mapping(DWORD prot, DWORD size_h, DWORD size_l, const string_type &name)
		{ return ::CreateFileMappingA(INVALID_HANDLE_VALUE, 0, prot, size_h, size_l, name.c_str()); }
		static ::HANDLE open_file_mapping(DWORD acc, const string_type &name) { return ::OpenFileMappingA(acc, FALSE, name.c_str()); }
	};

	struct shmem_wchar_tr {
		typedef wstring string_type;
		static string_type patch_name(bool vistaplus, const string_type &name) { return (vistaplus ? L"" : L"Global\\") + name; }
		static ::HANDLE create_file_mapping(DWORD prot, DWORD size_h, DWORD size_l, const string_type &name)
		{ return ::CreateFileMappingW(INVALID_HANDLE_VALUE, 0, prot, size_h, size_l, name.c_str()); }
		static ::HANDLE open_file_mapping(DWORD acc, const string_type &name) { return ::OpenFileMappingW(acc, FALSE, name.c_str()); }
	};

}

namespace ulibpp {

	const shared_memory::shrm_handle_type shared_memory::invalid_handle = NULL;
	void * shared_memory_mapping::invalid_addr = NULL;

	template <typename TR>
	void shared_memory::construct_shmem(const typename TR::string_type &name, size_t size, flag_type flags, bool create)
	{
		size_ = size;
		created_ = create;

		::SYSTEM_INFO si;
		::GetSystemInfo(&si);
		mag_ = si.dwAllocationGranularity;
		mps_ = si.dwPageSize;
#if ULIBPP_CC_PLATFORM==ULIBPP_CC_PLATFORM_MSVC && ULIBPP_CC_VERSION >= 1800
		has_exec_ = IsWindowsVistaOrGreater()						// Vista and above
			|| IsWindowsXPSP2OrGreater() && !IsWindowsServer()		// WinXPSP2+
			|| IsWindowsVersionOrGreater(5, 2, 1);					// W2k3SP1/WinXP64+
#else
		::OSVERSIONINFOEX ver;
		ver.dwOSVersionInfoSize = sizeof(::OSVERSIONINFOEX);
		::GetVersionEx((LPOSVERSIONINFO)(&ver));	// ugh, rough work
		has_exec_ = ver.dwMajorVersion >= 6							// Vista and above
			|| ver.dwMajorVersion == 5 && ver.dwMinorVersion == 1 && ver.wServicePackMajor >= 2   // WinXPSP2+
			|| ver.dwMajorVersion == 5 && ver.dwMinorVersion >= 2 && ver.wServicePackMajor >= 1;   // W2k3SP1/WinXP64+
#endif

		assert(flags & read);

#if ULIBPP_CC_PLATFORM==ULIBPP_CC_PLATFORM_MSVC && ULIBPP_CC_VERSION >= 1800
		typename TR::string_type rn = TR::patch_name(IsWindowsVistaOrGreater(), name);
#else
		typename TR::string_type rn = TR::patch_name(ver.dwMajorVersion>=6, name);
#endif

		if(create)
		{
			DWORD prot=0;
			if(has_exec_)
				switch(flags & (write|execute))
				{
				case 0:
					prot = PAGE_READONLY;
					break;
				case write:
					prot = PAGE_READWRITE;
					break;
				case execute:
					prot = PAGE_EXECUTE_READ;
					break;
				case write|execute:
					prot = PAGE_EXECUTE_READWRITE;
					break;
				}
			else
				prot = flags & write ? PAGE_READWRITE : PAGE_READONLY;
			memobj_ = TR::create_file_mapping(prot, 
#if ULIBPP_HW_PLATFORM_BITS == 32
									  0,
#elif ULIBPP_HW_PLATFORM_BITS == 64
									  static_cast<std::uint32_t>(size>>32),
#else
#error SHAREMEM does not support other sizes than 32- and 64-bits.
#endif
									  static_cast<std::uint32_t>(size & 0xffffffff), rn);
			DWORD err=::GetLastError();
			if(memobj_==0)
				throw std::runtime_error("Unable to create shared memory");
			if(err==ERROR_ALREADY_EXISTS)
			{
				::CloseHandle(memobj_);
				throw std::runtime_error("Shared memory already exists");
			}
		}
		else			// open
		{
			DWORD acc = flags & write ? FILE_MAP_WRITE|FILE_MAP_READ : FILE_MAP_READ;
#if !(ULIBPP_CC_PLATFORM==ULIBPP_CC_PLATFORM_MSVC && ULIBPP_CC_VERSION<1400)		// old PSDK for MSVC<VS8
			if(has_exec_ && (flags & execute))
				acc |= FILE_MAP_EXECUTE;
#endif
			memobj_ = TR::open_file_mapping(acc, rn);
			if(memobj_==0)
				throw std::runtime_error("Unable to open shared memory");
		}
	}

	void shared_memory::create(const string &name, size_t size, flag_type flags)
	{
		construct_shmem<shmem_char_tr>(name, size, flags, true);
	}

	void shared_memory::open(const string &name, size_t size, flag_type flags)
	{
		construct_shmem<shmem_char_tr>(name, size, flags, false);
	}

	void shared_memory::create(const wstring &name, size_t size, flag_type flags)
	{
		construct_shmem<shmem_wchar_tr>(name, size, flags, true);
	}

	void shared_memory::open(const wstring &name, size_t size, flag_type flags)
	{
		construct_shmem<shmem_wchar_tr>(name, size, flags, false);
	}

	shared_memory::~shared_memory()
	{
		if(memobj_!=invalid_handle)
			::CloseHandle(memobj_);
	}

	shared_memory::shared_memory(shared_memory &&sm)
	{
		memobj_ = sm.memobj_;
		sm.memobj_ = invalid_handle;
		size_ = sm.size_;
		mag_ = sm.mag_;
		mps_ = sm.mps_;
	}

	shared_memory &shared_memory::operator=(shared_memory &&sm)
	{
		if(memobj_!=sm.memobj_)
		{
			shared_memory  t(std::move(sm));
			std::swap(memobj_, t.memobj_);
			size_ = sm.size_;
			mag_ = sm.mag_;
			mps_ = sm.mps_;
		}
		return *this;
	}

	void *shared_memory_mapping::map(shared_memory &shm, size_t offset, size_t size, shared_memory::flag_type flags, void *addr_hint)
	{
		assert((offset & shm.get_mem_alloc_granularity())==0);

#pragma warning(push)
#if ULIBPP_CC_PLATFORM==ULIBPP_CC_PLATFORM_ICC
#pragma warning(disable:1684)		// this IS intentional and correct cast to integer type, fuck off with Intel Compiler
#endif

		assert((reinterpret_cast<std::uintptr_t>(addr_hint) & shm.get_mem_alloc_granularity())==0);

#pragma warning(pop)

		assert(flags & shared_memory::read);
		flags &= shared_memory::read|shared_memory::write|shared_memory::execute;
		assert((((flags & (shared_memory::read|shared_memory::write|shared_memory::execute))^flags)&flags)==0);
		if(size==0)
			size = shm.get_size();
		DWORD acc = flags & shared_memory::write ? FILE_MAP_WRITE : FILE_MAP_READ;
#if !(ULIBPP_CC_PLATFORM==ULIBPP_CC_PLATFORM_MSVC && ULIBPP_CC_VERSION<1400)		// old PSDK for MSVC<VS8
		if(ulibpp_private_::shrm_attorney::has_exec(shm) && (flags & shared_memory::execute))
			acc |= FILE_MAP_EXECUTE;
#endif
		addr_ = ::MapViewOfFileEx(ulibpp_private_::shrm_attorney::get_handle(shm), 
							acc,
#if ULIBPP_HW_PLATFORM_BITS == 32
							0,
#elif ULIBPP_HW_PLATFORM_BITS == 64
							(DWORD)(offset>>32),
#else
#error SHAREMEM does not support other sizes than 32- and 64-bits.
#endif
							(DWORD)(offset&0xffffffff),
							size_=size, addr_hint);
		if(addr_==0)
		{
			//DWORD err = ::GetLastError();
			throw std::runtime_error("Unable to map shared memory");
		}

		return addr_;
	}

	shared_memory_mapping::~shared_memory_mapping()
	{
		::UnmapViewOfFile(addr_);
	}

}

#elif ULIBPP_OS_PLATFORM==ULIBPP_OS_PLATFORM_LINUX ||		\
	  ULIBPP_OS_PLATFORM==ULIBPP_OS_PLATFORM_K1OM

#include "upp_locale.hpp"

namespace {

	struct wt_a {
		typedef std::string		stype;
		static string c_str(const stype &s) { return s; }
	};

	struct wt_w {
		typedef std::wstring		stype;
		static string c_str(const stype &s)
		{
			return ulibpp::cvrt_wstring2utf8(s);
		}
	};
}

namespace ulibpp {

	const shared_memory::shrm_handle_type shared_memory::invalid_handle = -1;
	void * shared_memory_mapping::invalid_addr = MAP_FAILED;

	template <typename TR>
	void shared_memory::construct_shmem(const typename TR::stype &name, size_t size, flag_type flags, bool create)
	{
		size_ = size;
		created_ = create;
		mag_ = mps_ = ::sysconf(_SC_PAGESIZE);
		assert(flags & read);
		shm_name_ = '/'+TR::c_str(name);		// adding initial slash-global visibility according to the POSIX manual
		int oflag = flags & write ? O_RDWR : O_RDONLY;
		if(create)
			oflag |= O_CREAT|O_EXCL;
		memobj_ = ::shm_open(shm_name_.c_str(), oflag, 0660);
		if(memobj_==-1)
		{
			if(errno==EEXIST)
				throw std::runtime_error("Shared memory '"+TR::c_str(name)+"' already exists");
			else
				throw std::runtime_error("Unable to create shared memory '"+TR::c_str(name)+"'");
		}
		if(create && ::ftruncate(memobj_, size))
		{
			::close(memobj_);
			::shm_unlink(shm_name_.c_str());
			throw std::runtime_error("Unable to create shared memory '"+TR::c_str(name)+"'");
		}
	}

	void shared_memory::create(const string &name, size_t size, flag_type flags)
	{
		construct_shmem<wt_a>(name, size, flags, true);
	}

	void shared_memory::open(const string &name, size_t size, flag_type flags)
	{
		construct_shmem<wt_a>(name, size, flags, false);
	}

	void shared_memory::create(const wstring &name, size_t size, flag_type flags)
	{
		construct_shmem<wt_w>(name, size, flags, true);
	}

	void shared_memory::open(const wstring &name, size_t size, flag_type flags)
	{
		construct_shmem<wt_w>(name, size, flags, false);
	}

	shared_memory::~shared_memory()
	{
		if(memobj_!=invalid_handle)
		{
			::close(memobj_);
			if(created_)
				::shm_unlink(shm_name_.c_str());
		}
	}

	shared_memory::shared_memory(shared_memory &&sm)
	{
		memobj_ = sm.memobj_;
		shm_name_ = std::move(sm.shm_name_);
		sm.memobj_ = invalid_handle;
		size_ = sm.size_;
		mag_ = sm.mag_;
		mps_ = sm.mps_;
	}

	shared_memory &shared_memory::operator=(shared_memory &&sm)
	{
		if(memobj_!=sm.memobj_)
		{
			shared_memory  t( std::move( sm));
			std::swap(memobj_, t.memobj_);
			std::swap(shm_name_, t.shm_name_);
			size_ = sm.size_;
			mag_ = sm.mag_;
			mps_ = sm.mps_;
		}
		return *this;
	}

	void *shared_memory_mapping::map(shared_memory &shm, size_t offset, size_t size, shared_memory::flag_type flags, void *addr_hint)
	{
		assert((offset & shm.get_mem_alloc_granularity())==0);
		assert((reinterpret_cast<uintptr_t>(addr_hint) & shm.get_mem_alloc_granularity())==0);
		assert(flags & shared_memory::read);
		flags &= shared_memory::read|shared_memory::write|shared_memory::execute;
		assert((((flags & (shared_memory::read|shared_memory::write|shared_memory::execute))^flags)&flags)==0);
		if(size==0)
			size = shm.get_size();
		int memprot = PROT_READ;
		if(flags & shared_memory::write)
			memprot |= PROT_WRITE;
		if(flags & shared_memory::execute)
			memprot |= PROT_EXEC;
		addr_ = ::mmap(	addr_hint, 
						size_ = size,
						memprot,
						MAP_SHARED,
						ulibpp_private_::shrm_attorney::get_handle(shm),
						offset);
		if(addr_==0)
			throw std::runtime_error("Unable to map shared memory");

		return addr_;
	}

	shared_memory_mapping::~shared_memory_mapping()
	{
		::munmap(addr_, size_);
	}

}

#else
#error Unsupported OS platform
#endif

