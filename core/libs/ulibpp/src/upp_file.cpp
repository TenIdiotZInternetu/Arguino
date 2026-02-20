/*

	upp_file.cpp

	Kuba, 2007, 2008
	Part of ULIB++

*/

#include <algorithm>
#include <functional>
#include <type_traits>
#include <array>

#include "upp_file.hpp"

using namespace std;
using namespace ulibpp;

// independent stuff
namespace {

	struct uferrm {
		ufile_error erc_;
		const char  *msg_;
	};

	// this must be synchronised with ufile_error
	std::array<uferrm, 18> uferrs = { {
		{ UFILEERR_OK, "OK" },
		{ UFILEERR_ALREADY_EXISTS, "The file already exists" },
		{ UFILEERR_NOT_EXISTS, "The file does not exist" },
		{ UFILEERR_FILEOP_SIZE_TRUNC, "File operation size was truncated" },
		{ UFILEERR_WAIT_ABANDONED, "The wait operation was abandoned" },
		{ UFILEERR_TIMEOUT, "The operation has timeouted" },
		{ UFILEERR_ILLFORMED_NAME, "The name is ill-formed" },
		{ UFILEERR_BADFNCPARAM, "Bad function parameters" },
		{ UFILEERR_TOO_MANY_AWOS, "Too many asynchronous-wait operations" },
		{ UFILEERR_BUFF_TOO_SMALL, "The buffer is too small" },
		{ UFILEERR_FILE_0_LEN, "The file has length zero" },
		{ UFILEERR_OS_SPECIFIC, "OS specific error" },
		{ UFILEERR_FILE_NOT_FOUND, "The file was not found" },
		{ UFILEERR_TOO_MANY_OPEN, "Too many open files" },
		{ UFILEERR_ACCESS_DENIED, "Access denied" },
		{ UFILEERR_DISK_FULL, "Disk is full" },
		{ UFILEERR_FILE_EXISTS, "The file already exists" },
		{ UFILEERR_INTBUF_SMALL, "Internal buffer is too small" },
	} };

	class ftor_find_err {
	public:
		ftor_find_err(ufile_error err) : err_(err) { }
		bool operator()(const uferrm &uem) {
			return uem.erc_==err_;
		}
	private:
		ufile_error		err_;
	};

	static const ulibpp_private_::ufile_flag_type sync_file_mask = 
		ulibpp_private_::uppf_open_mask|
		ulibpp_private_::uppf_shr_mask|
		ulibpp_private_::uppf_read|
		ulibpp_private_::uppf_write|
		ulibpp_private_::uppf_temp|
		ulibpp_private_::uppf_seq|
		ulibpp_private_::uppf_random|
		ulibpp_private_::uppf_nocache;

	static const ulibpp_private_::ufile_flag_type async_file_mask = 
		ulibpp_private_::uppf_open_mask|
		ulibpp_private_::uppf_shr_mask|
		ulibpp_private_::uppf_read|
		ulibpp_private_::uppf_write|
		ulibpp_private_::uppf_temp|
		ulibpp_private_::uppf_seq|
		ulibpp_private_::uppf_random|
		ulibpp_private_::uppf_nocache;

	static const ulibpp_private_::ufile_flag_type mm_file_mask = 
		ulibpp_private_::uppf_open_mask|
		ulibpp_private_::uppf_shr_mask|
		ulibpp_private_::uppf_access_mask|
		ulibpp_private_::uppf_temp|
		ulibpp_private_::uppf_seq|
		ulibpp_private_::uppf_random;

	template <typename T>
	struct const_remover;
	template <typename T>
	struct const_remover<const T &>
	{
		typedef T&		type;
	};

}

namespace ulibpp {

	const char *ufile_exception::what() const throw()
	{
		auto ci = find_if(uferrs.begin(), uferrs.end(), ftor_find_err(err_));
		assert(ci!=uferrs.end());
		return ci->msg_;
	}

	async_file::wait_objects_group::~wait_objects_group()
	{
		while(!awos_.empty())
			remove_awo(*awos_.front());
	}

	async_file::wait_object::wait_object(wait_objects_group &ag, magic m) : sz_(0),err_(UFILEERR_OK),my_wog_(0),af_(0),m_(m)
	{
		ag.add_awo(*this);
	}

	async_file::wait_object::~wait_object()
	{
		if(my_wog_)
			my_wog_->remove_awo(*this);			// well, this is not atomic (test and then using my_wog_ as pointer)
	}
}

// OS dependent stuff

#if ULIBPP_OS_PLATFORM == ULIBPP_OS_PLATFORM_WIN_NT ||		\
	ULIBPP_OS_PLATFORM == ULIBPP_OS_PLATFORM_WIN_NT64

namespace {

	ufile_error cvrt_error(DWORD we)
	{
		ufile_error ufe = UFILEERR_OS_SPECIFIC;
		switch(we)
		{
		case NO_ERROR:
			ufe = UFILEERR_OK;
			break;
		case ERROR_FILE_NOT_FOUND:
		case ERROR_PATH_NOT_FOUND:
			ufe = UFILEERR_FILE_NOT_FOUND;
			break;
		case ERROR_TOO_MANY_OPEN_FILES:
			ufe = UFILEERR_TOO_MANY_OPEN;
			break;
		case ERROR_ACCESS_DENIED:
		case ERROR_NETWORK_ACCESS_DENIED:
			ufe = UFILEERR_ACCESS_DENIED;
			break;
		case ERROR_HANDLE_DISK_FULL:
			ufe = UFILEERR_DISK_FULL;
			break;
		case ERROR_FILE_EXISTS:
		case ERROR_ALREADY_EXISTS:
			ufe = UFILEERR_FILE_EXISTS;
			break;
		case ERROR_NEGATIVE_SEEK:
			ufe = UFILEERR_BADFNCPARAM;
			break;
		default:
			// do nothing, already assigned
			break;
		}

		return ufe;
	}

	// checks the last error, converts it to ulibpp error, and throws an exception, if it is an error
	template <typename F>
	void err(F &f)
	{
		DWORD e = ::GetLastError();
		ufile_error ufe = cvrt_error(e);

		ulibpp_private_::ufile_attorney<F>::set_error(f, ufe);

		if(ufe>=UFILEERR_ERROR)
			throw ufile_exception(ufe);
	}

	template <typename F>
	void const_err(F &f)
	{

		DWORD e = ::GetLastError();
		ufile_error ufe = cvrt_error(e);

		ulibpp_private_::ufile_attorney<typename std::remove_const<F>::type>::set_error(const_cast<typename const_remover<F &>::type>(f), ufe);

		if(ufe>=UFILEERR_ERROR)
			throw ufile_exception(ufe);
	}

	struct wt_a {
		typedef string		stype;
		static const char slash='/';
		static const char bkslash='\\';
		static ::HANDLE wcreatefile(const string &fname, DWORD access, DWORD share, DWORD crdisp, DWORD flags) {
			return ::CreateFileA(fname.c_str(), access, share, 0, crdisp, flags, 0);
		}
	};

	struct wt_w {
		typedef wstring		stype;
		static const wchar_t slash=L'/';
		static const wchar_t bkslash=L'\\';
		static ::HANDLE wcreatefile(const wstring &fname, DWORD access, DWORD share, DWORD crdisp, DWORD flags) {
			return ::CreateFileW(fname.c_str(), access, share, 0, crdisp, flags, 0);
		}
	};

	template <typename WT>
	typename WT::stype translate_dirsep(typename WT::stype const &fn) {
		typename WT::stype s(fn);
#if ULIBPP_CC_LAMBDAS
		replace_if(s.begin(), s.end(), [=](auto ch) { return ch==WT::slash; }, WT::bkslash);
#else
		replace_if(s.begin(), s.end(), bind2nd(equal_to<typename WT::stype::value_type>(), WT::slash), WT::bkslash);
#endif
		return s;
	}

	template <typename WT, ulibpp_private_::ufile_flag_type FM, ulibpp_private_::ufile_flag_type FF, typename F>
	bool open_wt(typename WT::stype const &fname, ulibpp_private_::ufile_flag_type flags, ::HANDLE &fh, ufile_error &ufe, F &f)
	{
		assert((flags & ~FM)==0);
		flags = (flags & FM) | FF;
		typename WT::stype fn = translate_dirsep<WT>(fname);
		DWORD access, share, create, cflags;

		// setting "DesiredAccess" parameter
		switch(flags & ulibpp_private_::uppf_access_mask)
		{
		case ulibpp_private_::uppf_read:
			access = FILE_READ_DATA;
			break;
		case ulibpp_private_::uppf_write:
			assert((flags & ulibpp_private_::uppf_oper_mask)!=ulibpp_private_::uppf_memmap);
			access = FILE_WRITE_DATA|FILE_APPEND_DATA;
			break;
		case ulibpp_private_::uppf_read|ulibpp_private_::uppf_write:
			access = FILE_READ_DATA|FILE_WRITE_DATA|FILE_APPEND_DATA;
			break;
		case ulibpp_private_::uppf_read|ulibpp_private_::uppf_execute:
			access = FILE_READ_DATA|FILE_EXECUTE;
			break;
		case ulibpp_private_::uppf_read|ulibpp_private_::uppf_write|ulibpp_private_::uppf_execute:
			access = FILE_READ_DATA|FILE_WRITE_DATA|FILE_APPEND_DATA|FILE_EXECUTE;
			break;
		default:			// no access? or unknown value
			assert(0);
			access = 0;
			break;
		}

		// setting sharing mode
		switch(flags & ulibpp_private_::uppf_shr_mask)
		{
		case ulibpp_private_::uppf_shr_all:
			share = FILE_SHARE_READ|FILE_SHARE_WRITE;
			break;
		case ulibpp_private_::uppf_shr_read:
			share = FILE_SHARE_READ;
			break;
		case ulibpp_private_::uppf_shr_write:
			share = FILE_SHARE_WRITE;
			break;
		case ulibpp_private_::uppf_shr_excl:
			share = 0;
			break;
		}

		// setting "CreationDisposition"
		switch(flags & ulibpp_private_::uppf_open_mask)
		{
		case ulibpp_private_::uppf_open:
			create = OPEN_EXISTING;
			break;
		case ulibpp_private_::uppf_open|ulibpp_private_::uppf_truncate:
			create = TRUNCATE_EXISTING;
			break;
		case ulibpp_private_::uppf_create:
			create = OPEN_ALWAYS;
			break;
		case ulibpp_private_::uppf_create|ulibpp_private_::uppf_truncate:
			create = CREATE_ALWAYS;
			break;
		case ulibpp_private_::uppf_open|ulibpp_private_::uppf_create:
			create = CREATE_NEW;
			break;
		default:
			assert(0);
			create = 0;
			break;
		}

		// setting "flags"
		cflags = (flags & ulibpp_private_::uppf_seq)?FILE_FLAG_SEQUENTIAL_SCAN:FILE_FLAG_RANDOM_ACCESS;
		if(flags & ulibpp_private_::uppf_temp)
		{
			cflags |= FILE_ATTRIBUTE_TEMPORARY;
			share |= FILE_SHARE_DELETE;
		}
		if((flags & ulibpp_private_::uppf_async)==ulibpp_private_::uppf_async)
			cflags |= FILE_FLAG_OVERLAPPED;
		if((flags & ulibpp_private_::uppf_nocache)==ulibpp_private_::uppf_nocache)
			cflags |= FILE_FLAG_NO_BUFFERING|FILE_FLAG_WRITE_THROUGH;

		// do it
		if((fh=WT::wcreatefile(fn, access, share, create, cflags))==INVALID_HANDLE_VALUE)
			err(f);
		else if((create==CREATE_ALWAYS || create==OPEN_ALWAYS) && ::GetLastError()==ERROR_ALREADY_EXISTS)
			ufe = UFILEERR_ALREADY_EXISTS;

		return ufe!=UFILEERR_OK;
	}

	template <ulibpp_private_::ufile_flag_type FM, ulibpp_private_::ufile_flag_type FF, typename F>
	bool open_oem(const string &fname, ulibpp_private_::ufile_flag_type flags, ::HANDLE &fh, ufile_error &ufe, F &f)
	{
		return open_wt<wt_a, FM, FF>(fname, flags, fh, ufe, f);
	}

	template <ulibpp_private_::ufile_flag_type FM, ulibpp_private_::ufile_flag_type FF, typename F>
	bool open_ucs2(const wstring &fname, ulibpp_private_::ufile_flag_type flags, ::HANDLE &fh, ufile_error &ufe, F &f)
	{
		return open_wt<wt_w, FM, FF>(fname, flags, fh, ufe, f);
	}

	template <typename F>
	void flush_int_(F &f, ::HANDLE h)
	{
		if(!::FlushFileBuffers(h))
			err(f);
	}

	template <typename F>
	void set_filesize_int_(std::uint64_t fs, F &f, ::HANDLE h)
	{
		LARGE_INTEGER li, tli;
		li.QuadPart = 0;
		if(!::SetFilePointerEx(h, li, &tli, FILE_CURRENT))
			err(f);

		li.QuadPart = fs;
		if(!::SetFilePointerEx(h, li, 0, FILE_BEGIN))
			err(f);

		if(!::SetEndOfFile(h))
			err(f);

		if(!::SetFilePointerEx(h, tli, 0, FILE_BEGIN))
			err(f);
	}

	template <typename F>
	std::uint64_t get_filesize_int_(const F &f, ::HANDLE h)
	{
		LARGE_INTEGER li;

		if(!::GetFileSizeEx(h, &li))
			const_err(f);
		return li.QuadPart;
	}

}

namespace ulibpp {

	// sync_file
	bool sync_file::open(const std::string &fname, flag_type flags)
	{
		return open_oem<sync_file_mask, ulibpp_private_::uppf_file|ulibpp_private_::uppf_sync>(fname, flags, f_, err_, *this);
	}
	bool sync_file::open(const std::wstring &fname, flag_type flags)
	{
		return open_ucs2<sync_file_mask, ulibpp_private_::uppf_file|ulibpp_private_::uppf_sync>(fname, flags, f_, err_, *this);
	}

	void sync_file::close()
	{
		if(f_!=ulibpp_private_::ufile_invalid)
		{
			if(!::CloseHandle(f_))
				err(*this);
			f_ = ulibpp_private_::ufile_invalid;
		}
	}

	size_t sync_file::write_int_(const void *p, size_t s, size_t n) 
	{
		DWORD w;
		if(!::WriteFile(f_, p, static_cast<DWORD>(s*n), &w, 0))
			err(*this);
		return w/s;
	}

	size_t sync_file::read_int_(void *p, size_t s, size_t n) 
	{
		DWORD w;
		if(!::ReadFile(f_, p, static_cast<DWORD>(s*n), &w, 0))
			err(*this);
		return w/s;
	}

	void sync_file::seek(seek_method sm, std::int64_t pos)
	{
		DWORD skm = 0;;
		switch(sm)
		{
		case begin:
			skm = FILE_BEGIN;
			break;
		case current:
			skm = FILE_CURRENT;
			break;
		case end:
			skm = FILE_END;
			break;
		default:
			assert(0);
			break;
		}
		LARGE_INTEGER li;
		li.QuadPart = pos;
		if(!::SetFilePointerEx(f_, li, 0, skm))
			err(*this);
	}

	std::uint64_t sync_file::tell() const
	{
		LARGE_INTEGER li0, tli;
		li0.QuadPart = 0;
		if(!::SetFilePointerEx(f_, li0, &tli, FILE_CURRENT))
			err(*this);
		return tli.QuadPart;
	}

	void sync_file::flush()
	{
		flush_int_(*this, f_);
	}

	void sync_file::set_filesize(std::uint64_t fs)
	{
		set_filesize_int_(fs, *this, f_);
	}

	std::uint64_t sync_file::get_filesize() const
	{
		return get_filesize_int_(*this, f_);
	}

	// async file
	bool async_file::open(const std::string &fname, flag_type flags)
	{
		return open_oem<async_file_mask, ulibpp_private_::uppf_file|ulibpp_private_::uppf_async>(fname, flags, f_, err_, *this);
	}
	bool async_file::open(const std::wstring &fname, flag_type flags)
	{
		return open_ucs2<async_file_mask, ulibpp_private_::uppf_file|ulibpp_private_::uppf_async>(fname, flags, f_, err_, *this);
	}

	void async_file::close()
	{
		if(f_!=ulibpp_private_::ufile_invalid)
		{
			if(!::CloseHandle(f_))
				err(*this);
			f_ = ulibpp_private_::ufile_invalid;
		}
	}

	void async_file::flush()
	{
		flush_int_(*this, f_);
	}

	void async_file::set_filesize(std::uint64_t fs)
	{
		set_filesize_int_(fs, *this, f_);
	}

	std::uint64_t async_file::get_filesize() const
	{
		return get_filesize_int_(*this, f_);
	}

	ulibpp_private_::awo::awo()
	{
		if((o_.hEvent=::CreateEvent(0, TRUE, FALSE, 0))==0)
			throw ufile_exception(cvrt_error(::GetLastError()));
	}

	ulibpp_private_::awo::~awo()
	{
		if(o_.hEvent)
			::CloseHandle(o_.hEvent);
	}

	ulibpp_private_::awo::awo(ulibpp_private_::awo &&wo)
	{
		o_ = std::move(wo.o_);
		wo.o_.hEvent = 0;
	}

	ulibpp_private_::awo & ulibpp_private_::awo::operator=(ulibpp_private_::awo &&wo)
	{
		awo t( std::move( wo));
		std::swap(o_, t.o_);
		return *this;
	}

	ulibpp_private_::aog::aog()
	{
		if((cancel_event_=::CreateEvent(0, FALSE, FALSE, 0))==0)
			throw ufile_exception(cvrt_error(::GetLastError()));
		hdls_.push_back(cancel_event_);
	}

	ulibpp_private_::aog::~aog()
	{
		if(cancel_event_)
			::CloseHandle(cancel_event_);
	}

	ulibpp_private_::aog::aog(ulibpp_private_::aog &&og)
	{
		hdls_ = std::move(og.hdls_);
		cancel_event_ = og.cancel_event_;
		og.cancel_event_ = 0;
	}

	ulibpp_private_::aog & ulibpp_private_::aog::operator=(ulibpp_private_::aog &&og)
	{
		aog t( std::move( og));
		std::swap(hdls_, t.hdls_);
		std::swap(cancel_event_, t.cancel_event_);
		return *this;
	}

	void async_file::wait_objects_group::add_awo(wait_object &wo)
	{
		assert(wo.my_wog_==0);
		spl_.acquire();
		if(aog_.hdls_.size()>=max_awos)
			throw ufile_exception(UFILEERR_TOO_MANY_AWOS);
		aog_.hdls_.push_back(wo.awo_.o_.hEvent);
		awos_.push_back(&wo);
		wo.my_wog_ = this;
		spl_.release();
	}

	void async_file::wait_objects_group::remove_awo(wait_object &wo)
	{
		assert(wo.my_wog_==this);
		spl_.acquire();
		wov_type::iterator it = find(awos_.begin(), awos_.end(), &wo);
		if(it!=awos_.end())
		{
			size_t n = it-awos_.begin();
			awos_.erase(it);
			aog_.hdls_.erase(aog_.hdls_.begin()+n+1);		// add cancel_event
			wo.my_wog_ = 0;
		}
		spl_.release();
	}

	async_file::wait_object *async_file::wait_objects_group::wait()
	{
		assert(awos_.size()==aog_.hdls_.size()-1);  // added cancel_event
		DWORD wmorv;
		if((wmorv=::WaitForMultipleObjects(static_cast<DWORD>(aog_.hdls_.size()), &aog_.hdls_[0], FALSE, INFINITE))==WAIT_FAILED)
		{
			err_ = cvrt_error(::GetLastError());
			throw ufile_exception(err_);
		}
		if(wmorv==WAIT_OBJECT_0)			// wait cancelled
			return 0;
		if(wmorv>WAIT_OBJECT_0 && wmorv<WAIT_OBJECT_0+aog_.hdls_.size())
		{
			wait_object *wo = awos_[wmorv-(WAIT_OBJECT_0+1)];
			DWORD sz=0;
			::GetOverlappedResult(wo->af_->f_, &wo->awo_.o_, &sz, FALSE);
			const DWORD err = ::GetLastError();
			wo->err_ = cvrt_error(err==ERROR_IO_PENDING?NO_ERROR:err);  // hack for Vista, is it correct, at this point it should be finished ???
			wo->sz_ = sz;
			wo->af_ = 0;

			return wo;
		}
		else if(wmorv==WAIT_ABANDONED_0)		// wtf, abandoned cancel_event?
		{
			assert(0);
		}
		else if(wmorv>WAIT_ABANDONED_0 && wmorv<WAIT_ABANDONED_0+aog_.hdls_.size())
		{
			wait_object *wo = awos_[wmorv-(WAIT_OBJECT_0+1)];

			wo->err_ = UFILEERR_WAIT_ABANDONED;
			wo->sz_ = 0;
			wo->af_ = 0;

			return wo;
		}
		else
		{
			err_ = UFILEERR_TIMEOUT;
			throw ufile_exception(err_);
		}
		assert(0);
		return 0;		// this should be never reached
	}

	void async_file::wait_objects_group::cancel_wait()
	{
		::SetEvent(aog_.cancel_event_);
	}

	void async_file::write_int_(wait_object &wo, std::uint64_t fpos, const volatile void *p, size_t s, size_t n)
	{
		size_t totsz = s*n;
#if ULIBPP_HW_PLATFORM_BITS==64		// WinAPI can read only 32-bit sizes
		if(totsz>0xFFFFFFFF)
		{
			err_ = UFILEERR_FILEOP_SIZE_TRUNC;
			throw ufile_exception(err_);
		}
#endif
		::HANDLE h = wo.awo_.o_.hEvent;
		memset(&wo.awo_.o_, 0, sizeof(::OVERLAPPED));
		wo.awo_.o_.hEvent = h;
		wo.awo_.o_.Offset = static_cast<DWORD>(fpos);
		wo.awo_.o_.OffsetHigh = static_cast<DWORD>(fpos>>32);
		wo.af_ = this;
		if(::WriteFile(f_, const_cast<const void *>(p), static_cast<DWORD>(totsz), 0, &wo.awo_.o_)==0 && ::GetLastError()!=ERROR_IO_PENDING)
			err(*this);
	}

	void async_file::read_int_(wait_object &wo, std::uint64_t fpos, volatile void *p, size_t s, size_t n)
	{
		size_t totsz = s*n;
#if ULIBPP_HW_PLATFORM_BITS==64		// WinAPI can read only 32-bit sizes
		if(totsz>0xFFFFFFFF)
		{
			err_ = UFILEERR_FILEOP_SIZE_TRUNC;
			throw ufile_exception(err_);
		}
#endif
		::HANDLE h = wo.awo_.o_.hEvent;
		memset(&wo.awo_.o_, 0, sizeof(::OVERLAPPED));
		wo.awo_.o_.hEvent = h;
		wo.awo_.o_.Offset = static_cast<DWORD>(fpos);
		wo.awo_.o_.OffsetHigh = static_cast<DWORD>(fpos>>32);
		wo.af_ = this;
		if(::ReadFile(f_, const_cast<void*>(p), static_cast<DWORD>(totsz), 0, &wo.awo_.o_)==0 && ::GetLastError()!=ERROR_IO_PENDING)
			err(*this);
	}

	// memory-mapped file
	bool mm_file::open(const std::string &fname, flag_type flags)
	{
		return open_oem<mm_file_mask, ulibpp_private_::uppf_memmap|ulibpp_private_::uppf_sync>(fname, flags, f_, err_, *this);
	}
	bool mm_file::open(const std::wstring &fname, flag_type flags)
	{
		return open_ucs2<mm_file_mask, ulibpp_private_::uppf_memmap|ulibpp_private_::uppf_sync>(fname, flags, f_, err_, *this);
	}

	void mm_file::close()
	{
		if(mmf_!=ulibpp_private_::ummfile_invalid)
			unmap();

		if(f_!=ulibpp_private_::ufile_invalid)
		{
			if(!::CloseHandle(f_))
				err(*this);
			f_ = ulibpp_private_::ufile_invalid;
		}
	}

	void mm_file::flush()
	{
		flush_int_(*this, f_);
	}

	void mm_file::set_filesize(std::uint64_t fs)
	{
		set_filesize_int_(fs, *this, f_);
	}

	std::uint64_t mm_file::get_filesize() const
	{
		return get_filesize_int_(*this, f_);
	}

	std::size_t mm_file::get_pagesize()
	{
		SYSTEM_INFO sysinfo;
		::GetSystemInfo(&sysinfo);
		return sysinfo.dwPageSize;
	}

	void mm_file::map(access_flag_type flags, std::size_t size)
	{
		if(mmf_!=ulibpp_private_::ummfile_invalid)
			unmap();

		DWORD protect = 0;

		switch(flags & ulibpp_private_::uppf_access_mask)
		{
		case acc_rd:
			protect = PAGE_READONLY;
			break;
		case acc_wr:
		case acc_rd|acc_wr:
			protect = PAGE_READWRITE;
			break;
		case acc_rd|acc_execute:
			protect = PAGE_EXECUTE_READ;
			break;
		case acc_rd|acc_wr|acc_execute:
			protect = PAGE_EXECUTE_READWRITE;
			break;
		default:			/* no access? or unknown value */
			assert(0);
			return;
		}

		::HANDLE h = ::CreateFileMapping(f_, 0, protect,
#if ULIBPP_HW_PLATFORM_BITS==64
										  (DWORD)(size>>32),	/* the whole file */
#elif ULIBPP_HW_PLATFORM_BITS==32
										  0,
#else
#error Unsupported platform bits
#endif
										  (DWORD)(size & 0xffffffff),
										  0);
		if(h==0)
			err(*this);

		mmf_ = h;
	}

	void mm_file::unmap()
	{
		if(!::CloseHandle(mmf_))
			err(*this);
		mmf_ = ulibpp_private_::ummfile_invalid;
	}

	mm_file::mm_view::~mm_view()
	{
		if(!::UnmapViewOfFile(umv_.addr_))
		{
			try {
				err(*ummf_);
			} catch(...)
			{
				// destructor must not throw
			}
		}
	}

	std::unique_ptr<mm_file::mm_view> mm_file::create_view(access_flag_type flags, std::uint64_t offset, std::size_t size, void *hint_addr)
	{
		DWORD access = 0;;

		switch(flags & ulibpp_private_::uppf_access_mask)
		{
		case acc_rd:
			access = FILE_MAP_READ;
			break;
		case acc_wr:
		case acc_rd|acc_wr:
			access = FILE_MAP_WRITE;
			break;
		case acc_rd|acc_execute:
			access = FILE_MAP_READ|FILE_MAP_EXECUTE;
			break;
		case acc_rd|acc_wr|acc_execute:
			access = FILE_MAP_WRITE|FILE_MAP_EXECUTE;
			break;
		default:			/* no access? or unknown value */
			assert(0);
			break;
		}

		ulibpp_private_::umapview_type umv;
		umv.addr_ = ::MapViewOfFileEx(mmf_, 
										access,
										(DWORD)(offset>>32),
										(DWORD)(offset&0xffffffff),
										size, hint_addr);
		if(!umv.addr_)
			err(*this);

		return std::unique_ptr<mm_view>(new mm_view(*this, umv));
	}

}

#elif ULIBPP_OS_PLATFORM == ULIBPP_OS_PLATFORM_LINUX ||		\
	  ULIBPP_OS_PLATFORM == ULIBPP_OS_PLATFORM_K1OM

#include <sys/mman.h>
#include <cstdio>
#include <cstring>
#include <sys/stat.h>
#include <signal.h>

namespace {

	ufile_error cvrt_error(int ue)
	{
		ufile_error ufe;
		switch(ue)
		{
		case 0:
			ufe = UFILEERR_OK;
			break;
		case ENOENT:
			ufe = UFILEERR_FILE_NOT_FOUND;
			break;
		case ENFILE:
			ufe = UFILEERR_TOO_MANY_OPEN;
			break;
		case EACCES:
			ufe = UFILEERR_ACCESS_DENIED;
			break;
		case ENOSPC:
			ufe = UFILEERR_DISK_FULL;
			break;
		case EEXIST:
			ufe = UFILEERR_FILE_EXISTS;
			break;
		case EINVAL:
			ufe = UFILEERR_BADFNCPARAM;
			break;
		default:
			ufe = UFILEERR_OS_SPECIFIC;
			break;
		}

		return ufe;
	}

	// checks the last error, converts it to ulibpp error, and throws an exception, if it is an error
	template <typename F>
	void err(F &f)
	{
		ufile_error ufe = cvrt_error(errno);

		ulibpp_private_::ufile_attorney<F>::set_error(f, ufe);

		if(ufe>=UFILEERR_ERROR)
			throw ufile_exception(ufe);
	}

	template <typename F>
	void const_err(F &f)
	{
		ufile_error ufe = cvrt_error(errno);

		ulibpp_private_::ufile_attorney<typename std::remove_const<F>::type>::set_error(const_cast<typename const_remover<F &>::type>(f), ufe);

		if(ufe>=UFILEERR_ERROR)
			throw ufile_exception(ufe);
	}

	struct wt_a {
		typedef std::string		stype;
		static string c_str(const stype &s) { return s; }
		static stype tostype(const char *s) { return std::string(s); }
	};

	struct wt_w {
		typedef std::wstring		stype;
		static string c_str(const stype &s)
		{
			return ulibpp::cvrt_wstring2utf8(s);
		}
		static stype tostype(const char *s)
		{
			return ulibpp::cvrt_utf82wstring(std::string_view(s));
		}
	};

	template <typename WT, ulibpp_private_::ufile_flag_type FM, ulibpp_private_::ufile_flag_type FF, typename F>
	bool open_wt(typename WT::stype const &fname, ulibpp_private_::ufile_flag_type flags, int &fh, ufile_error &ufe, F &f)
	{
		assert((flags & ~FM)==0);
		flags = (flags & FM) | FF;
		int ofl;

		switch(flags & ulibpp_private_::uppf_access_mask)
		{
		case ulibpp_private_::uppf_read:
		case ulibpp_private_::uppf_read|ulibpp_private_::uppf_execute:
			ofl = O_RDONLY;
			break;
		case ulibpp_private_::uppf_write:
			assert((flags & ulibpp_private_::uppf_oper_mask)!=ulibpp_private_::uppf_memmap);
			ofl = O_WRONLY;
			break;
		case ulibpp_private_::uppf_read|ulibpp_private_::uppf_write:
		case ulibpp_private_::uppf_read|ulibpp_private_::uppf_write|ulibpp_private_::uppf_execute:
			ofl = O_RDWR;
			break;
		default:			// no access? or unknown value
			assert(0);
			ofl = 0;
			break;
		}

		// setting sharing mode
		switch(flags & ulibpp_private_::uppf_shr_mask)
		{
		case ulibpp_private_::uppf_shr_all:
		case ulibpp_private_::uppf_shr_read:
		case ulibpp_private_::uppf_shr_write:
			/* unable to do anything on *nices - say an error?? */
			break;
		case ulibpp_private_::uppf_shr_excl:
			/* *nices open files always exclusively */
			break;
		}

		// setting open mode
		switch(flags & ulibpp_private_::uppf_open_mask)
		{
		case ulibpp_private_::uppf_open:
			/* do nothing */
			break;
		case ulibpp_private_::uppf_open|ulibpp_private_::uppf_truncate:
			ofl |= O_TRUNC;
			break;
		case ulibpp_private_::uppf_create:
			ofl |= O_CREAT;
			break;
		case ulibpp_private_::uppf_create|ulibpp_private_::uppf_truncate:
			ofl |= O_CREAT|O_TRUNC;
			break;
		case ulibpp_private_::uppf_open|ulibpp_private_::uppf_create:
			ofl |= O_CREAT|O_EXCL;
			break;
		default:
			assert(0);
			break;
		}

		// setting "flags"
#if defined(_LARGEFILE64_SOURCE) || defined(O_LARGEFILE)
		ofl |= O_LARGEFILE;
#endif
		if((flags & ulibpp_private_::uppf_nocache)==ulibpp_private_::uppf_nocache)
			ofl |= O_DIRECT|O_SYNC;

		// do it
		if((fh=open64(WT::c_str(fname).c_str(), ofl, S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH))<0)
			err(f);
		else if((ofl & O_CREAT)!=0 && (ofl & O_EXCL)==0 && errno==EEXIST)
			ufe = UFILEERR_ALREADY_EXISTS;

		return ufe!=UFILEERR_OK;
	}

	template <ulibpp_private_::ufile_flag_type FM, ulibpp_private_::ufile_flag_type FF, typename F>
	bool open_oem(const string &fname, ulibpp_private_::ufile_flag_type flags, int &fh, ufile_error &ufe, F &f)
	{
		return open_wt<wt_a, FM, FF>(fname, flags, fh, ufe, f);
	}

	template <ulibpp_private_::ufile_flag_type FM, ulibpp_private_::ufile_flag_type FF, typename F>
	bool open_utf8(const wstring &fname, ulibpp_private_::ufile_flag_type flags, int &fh, ufile_error &ufe, F &f)
	{
		return open_wt<wt_w, FM, FF>(fname, flags, fh, ufe, f);
	}

	template <typename F>
	void flush_int_(F &f, int fh)
	{
		if(::fsync(fh)==-1)
			err(f);
	}

	template <typename F>
	void set_filesize_int_(std::uint64_t fs, F &f, int fh)
	{
		if(::ftruncate(fh, (off_t)fs)==(off_t)-1)
			err(f);
	}

	template <typename F>
	std::uint64_t get_filesize_int_(const F &f, int fh)
	{
		off_t opos, npos=0;

		if((opos=::lseek64(fh, 0, SEEK_CUR))==static_cast< ::off_t>(-1) ||
			(npos=::lseek64(fh, 0, SEEK_END))==static_cast< ::off_t>(-1) ||
			::lseek64(fh, opos, SEEK_SET)==static_cast< ::off_t>(-1))
			const_err(f);
		return npos;
	}
}

namespace ulibpp {

	// sync_file
	bool sync_file::open(const std::string &fname, flag_type flags)
	{
		return open_oem<sync_file_mask, ulibpp_private_::uppf_file|ulibpp_private_::uppf_sync>(fname, flags, f_, err_, *this);
	}
	bool sync_file::open(const std::wstring &fname, flag_type flags)
	{
		return open_utf8<sync_file_mask, ulibpp_private_::uppf_file|ulibpp_private_::uppf_sync>(fname, flags, f_, err_, *this);
	}

	void sync_file::close()
	{
		if(f_!=ulibpp_private_::ufile_invalid)
		{
			if(::close(f_))
				err(*this);
			f_ = ulibpp_private_::ufile_invalid;
		}
	}

	size_t sync_file::write_int_(const void *p, size_t s, size_t n) 
	{
		ssize_t rsz;
		if((rsz=::write(f_, p, s*n))==-1)
			err(*this);
		return rsz/s;
	}

	size_t sync_file::read_int_(void *p, size_t s, size_t n) 
	{
		ssize_t rsz;
		if((rsz=::read(f_, p, s*n))==-1)
			err(*this);
		return rsz/s;
	}

	void sync_file::seek(seek_method sm, std::int64_t pos)
	{
		int skm;
		switch(sm)
		{
		case begin:
			skm = SEEK_SET;
			break;
		case current:
			skm = SEEK_CUR;
			break;
		case end:
			skm = SEEK_END;
			break;
		default:
			assert(0);
			skm = 0;
			break;
		}
		if(::lseek64(f_, (off_t)pos, skm)==(off_t)-1)
			err(*this);
	}

	std::uint64_t sync_file::tell() const
	{
		off_t cpos;
		if((cpos=::lseek64(f_, 0, SEEK_CUR))==(off_t)-1)
			err(*this);
		return cpos;
	}

	void sync_file::flush()
	{
		flush_int_(*this, f_);
	}

	void sync_file::set_filesize(std::uint64_t fs)
	{
		set_filesize_int_(fs, *this, f_);
	}

	std::uint64_t sync_file::get_filesize() const
	{
		return get_filesize_int_(*this, f_);
	}

	// async file
	bool async_file::open(const std::string &fname, flag_type flags)
	{
		return open_oem<sync_file_mask, ulibpp_private_::uppf_file|ulibpp_private_::uppf_async>(fname, flags, f_, err_, *this);
	}
	bool async_file::open(const std::wstring &fname, flag_type flags)
	{
		return open_utf8<sync_file_mask, ulibpp_private_::uppf_file|ulibpp_private_::uppf_async>(fname, flags, f_, err_, *this);
	}

	void async_file::close()
	{
		if(f_!=ulibpp_private_::ufile_invalid)
		{
			if(::close(f_))
				err(*this);
			f_ = ulibpp_private_::ufile_invalid;
		}
	}

	void async_file::flush()
	{
		flush_int_(*this, f_);
	}

	void async_file::set_filesize(std::uint64_t fs)
	{
		set_filesize_int_(fs, *this, f_);
	}

	std::uint64_t async_file::get_filesize() const
	{
		return get_filesize_int_(*this, f_);
	}

	void async_file::wait_objects_group::add_awo(wait_object &wo)
	{
		assert(wo.my_wog_==0);
		spl_.acquire();
		aog_.aiocbs_.push_back(&wo.awo_.aiocb_);
		awos_.push_back(&wo);
		wo.my_wog_ = this;
		spl_.release();
	}

	void async_file::wait_objects_group::remove_awo(wait_object &wo)
	{
		assert(wo.my_wog_==this);
		spl_.acquire();
		wov_type::iterator it = find(awos_.begin(), awos_.end(), &wo);
		if(it!=awos_.end())
		{
			size_t n = it-awos_.begin();
			awos_.erase(it);
			aog_.aiocbs_.erase(aog_.aiocbs_.begin()+n);
			wo.my_wog_ = 0;
		}
		spl_.release();
	}

	void ulibpp_private_::aog::aio_completition(::sigval_t sigval)
	{
		async_file::wait_object *wo = static_cast<async_file::wait_object *>(sigval.sival_ptr);
		int err;

		if((err=::aio_error(&wo->awo_.aiocb_))==0)		// no error
		{
			::ssize_t ssz = ::aio_return(&wo->awo_.aiocb_);
			assert(ssz!=static_cast< ::ssize_t>(-1));
			wo->sz_ = static_cast<size_t>(ssz);
			wo->err_ = UFILEERR_OK;
		}
		else if(err!=EINPROGRESS)						// error
		{
			wo->sz_ = 0;
			wo->err_ = cvrt_error(err);
		}
		else
		{
			assert(0);				// wtf, why this callback has been called?
		}

		wo->awo_.signaled_ = true;
		wo->my_wog_->aog_.sem_.release();
	}

	async_file::wait_object *async_file::wait_objects_group::wait()
	{
		async_file::wait_object *wo=0;

		aog_.sem_.acquire();			// wait on the semaphore

		spl_.acquire();
		for(wov_type::const_iterator cit=awos_.begin();cit!=awos_.end();++cit)
		{
			if((*cit)->awo_.signaled_)
			{
				wo = *cit;
				break;
			}
		}
		spl_.release();

		return wo;
	}

	void async_file::wait_objects_group::cancel_wait()
	{
		aog_.sem_.release();
	}

	void async_file::write_int_(wait_object &wo, std::uint64_t fpos, const volatile void *p, size_t s, size_t n)
	{
		// not sure, if it must be zeroed before any action, so it is zeroed
		::memset(&wo.awo_.aiocb_, 0, sizeof(::aiocb));
		wo.awo_.aiocb_.aio_fildes = f_;
		wo.awo_.aiocb_.aio_sigevent.sigev_notify = SIGEV_THREAD;
		wo.awo_.aiocb_.aio_sigevent.sigev_notify_function = ulibpp_private_::aog::aio_completition;
		wo.awo_.aiocb_.aio_sigevent.sigev_notify_attributes = NULL;
		wo.awo_.aiocb_.aio_sigevent.sigev_value.sival_ptr = static_cast<void*>(&wo);
		wo.awo_.aiocb_.aio_nbytes = s*n;
		wo.awo_.aiocb_.aio_offset = fpos;
		wo.awo_.aiocb_.aio_buf = const_cast<volatile void *>(p);
		wo.awo_.signaled_ = false;
		if(::aio_write(&wo.awo_.aiocb_)==-1)
			err(*this);
	}

	void async_file::read_int_(wait_object &wo, std::uint64_t fpos, volatile void *p, size_t s, size_t n)
	{
		// not sure, if it must be zeroed before any action, so it is zeroed
		::memset(&wo.awo_.aiocb_, 0, sizeof(::aiocb));
		wo.awo_.aiocb_.aio_fildes = f_;
		wo.awo_.aiocb_.aio_sigevent.sigev_notify = SIGEV_THREAD;
		wo.awo_.aiocb_.aio_sigevent.sigev_notify_function = ulibpp_private_::aog::aio_completition;
		wo.awo_.aiocb_.aio_sigevent.sigev_notify_attributes = NULL;
		wo.awo_.aiocb_.aio_sigevent.sigev_value.sival_ptr = static_cast<void*>(&wo);
		wo.awo_.aiocb_.aio_nbytes = s*n;
		wo.awo_.aiocb_.aio_offset = fpos;
		wo.awo_.aiocb_.aio_buf = p;
		wo.awo_.signaled_ = false;
		if(::aio_read(&wo.awo_.aiocb_)==-1)
			err(*this);
	}

	// memory-mapped file
	bool mm_file::open(const std::string &fname, flag_type flags)
	{
		return open_oem<mm_file_mask, ulibpp_private_::uppf_memmap|ulibpp_private_::uppf_sync>(fname, flags, f_, err_, *this);
	}
	bool mm_file::open(const std::wstring &fname, flag_type flags)
	{
		return open_utf8<mm_file_mask, ulibpp_private_::uppf_memmap|ulibpp_private_::uppf_sync>(fname, flags, f_, err_, *this);
	}

	void mm_file::close()
	{
		if(mmf_!=ulibpp_private_::ummfile_invalid)
			unmap();

		if(f_!=ulibpp_private_::ufile_invalid)
		{
			if(::close(f_))
				err(*this);
			f_ = ulibpp_private_::ufile_invalid;
		}
	}

	void mm_file::flush()
	{
		flush_int_(*this, f_);
	}

	void mm_file::set_filesize(std::uint64_t fs)
	{
		set_filesize_int_(fs, *this, f_);
	}

	std::uint64_t mm_file::get_filesize() const
	{
		return get_filesize_int_(*this, f_);
	}

	std::size_t mm_file::get_pagesize()
	{
		return ::sysconf(_SC_PAGESIZE);
	}

	void mm_file::map(access_flag_type , std::size_t )
	{
		mmf_ = f_;
	}

	void mm_file::unmap()
	{
		mmf_ = ulibpp_private_::ummfile_invalid;
	}

	mm_file::mm_view::~mm_view()
	{
		if(::munmap(umv_.addr_, umv_.size_)==-1)
		{
			try {
				err(*ummf_);
			} catch(...)
			{
				// destructor must not throw
			}
		}
	}

	std::unique_ptr<mm_file::mm_view> mm_file::create_view(access_flag_type flags, std::uint64_t offset, std::size_t size, void *hint_addr)
	{
		int memprot = PROT_READ;
		if(flags & acc_wr)
			memprot |= PROT_WRITE;
		if(flags & acc_execute)
			memprot |= PROT_EXEC;

		ulibpp_private_::umapview_type umv;
		size_t pgsz = get_pagesize();
		umv.size_ = (size+pgsz-1)&~(pgsz-1);
		if((umv.addr_=::mmap(hint_addr, umv.size_, memprot, MAP_SHARED, mmf_, (off_t)offset))==MAP_FAILED)
			err(*this);

		return std::unique_ptr<mm_file::mm_view>(new mm_view(*this, umv));
	}
}

#else
#error Unsupported OS
#endif
