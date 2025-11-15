#ifndef ULIBPP_UPP_FILE_HPP_
#define ULIBPP_UPP_FILE_HPP_

/*

	upp_file.hpp

	File operation

	Kuba, 2007
	Part of ULIB++

*/

#include <stdexcept>
#include <string>
#include <cstddef>
#include <vector>
#include <memory>
#include <cstdint>

#include "upp_locale.hpp"
#include "upp_mthr.hpp"

#if ULIBPP_OS_PLATFORM == ULIBPP_OS_PLATFORM_WIN_NT ||		\
	ULIBPP_OS_PLATFORM == ULIBPP_OS_PLATFORM_WIN_NT64

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <windows.h>

namespace ulibpp {
	
	namespace ulibpp_private_ {

		// some types
		typedef ::HANDLE			ufile_type;
		typedef ::HANDLE			ummfile_type;
		struct umapview_type {
			umapview_type() : addr_(0) { }
			void		*addr_;
			template <typename T>
			T *get_addr() const { return static_cast<T*>(addr_); }
		};

		struct awo {
			awo();
			~awo();
			// move semantics
			awo(const awo &) = delete;
			awo &operator=(const awo &) = delete;
			awo(awo &&wo);
			awo &operator=(awo &&wo);

			OVERLAPPED				o_;
		};

		struct aog {
			aog();
			~aog();
			// move semantics
			aog(const aog &) = delete;
			aog &operator=(const aog &) = delete;
			aog(aog &&og);
			aog &operator=(aog &&og);

			typedef std::vector<::HANDLE>   hdlsv_type;
			hdlsv_type				hdls_;
			::HANDLE				cancel_event_;
			static const std::size_t max_hdls = MAXIMUM_WAIT_OBJECTS-1;
		};

		// some constants
		static const ufile_type ufile_invalid = INVALID_HANDLE_VALUE;
		static const std::size_t ufile_maxpath = MAX_PATH;
		static const ummfile_type ummfile_invalid = INVALID_HANDLE_VALUE;
	}
}

#elif ULIBPP_OS_PLATFORM == ULIBPP_OS_PLATFORM_LINUX ||			\
	  ULIBPP_OS_PLATFORM == ULIBPP_OS_PLATFORM_K1OM

#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE				4
#endif
#include <sys/fcntl.h>
#include <aio.h>
#include <signal.h>

namespace ulibpp {
	
	namespace ulibpp_private_ {

		// some types
		typedef int			ufile_type;
		typedef int			ummfile_type;
		struct umapview_type {
			umapview_type() : addr_(0),size_(0) { }
			void			*addr_;
			std::size_t		size_;
			template <typename T>
			T *get_addr() const { return static_cast<T*>(addr_); }
		};

		struct awo {
			::aiocb				aiocb_;
			bool				signaled_;
		};

		struct aog {
			aog() : sem_(0) { }
			typedef std::vector< ::aiocb*> aiocbv_type;
			aiocbv_type		aiocbs_;
			semaphore		sem_;
			static void aio_completition(::sigval_t sigval);
			static const std::size_t max_hdls = static_cast<std::size_t>(-1);
		};
		
		// some constants
		static const ufile_type ufile_invalid = -1;
		static const std::size_t ufile_maxpath = 255;			// huh, *nices do not define any max size of path?
		static const ummfile_type ummfile_invalid = -1;
	}
}

#else
#error Unsupported OS
#endif

/*

	There are 4 kinds of files:
		- files with synchronous operations
		- files with asynchronous operations
		- memory-mapped files
		- named pipes

	These filetypes are so different in behavior, there is no much room for code reusability or inheritance.
	Moreover, it is not a specialization, so any inheritance is a nonsens.
	So they are designed and implemented as 5 different classes (2 for pipes).

	There is a small preliminary setup for a static inheritance - all classes have some common functions with the same meaning.

	The basic idea behind the design is RAII (Resource Acquisition Is Initialization), because files are resources
	and we must handle them correctly.

*/

namespace ulibpp {

	// Visible types

	// errors
	enum ufile_error {
		UFILEERR_OK,
		// warnings
		UFILEERR_WARNING=100,
		UFILEERR_ALREADY_EXISTS,
		UFILEERR_NOT_EXISTS,
		UFILEERR_FILEOP_SIZE_TRUNC,
		UFILEERR_WAIT_ABANDONED,
		UFILEERR_TIMEOUT,
		// errors
		UFILEERR_ERROR=1000,
		UFILEERR_ILLFORMED_NAME,
		UFILEERR_BADFNCPARAM,
		UFILEERR_TOO_MANY_AWOS,
		UFILEERR_AWO_NOT_BOUND,
		UFILEERR_BUFF_TOO_SMALL,
		UFILEERR_FILE_0_LEN,
		UFILEERR_OS_SPECIFIC,
		UFILEERR_FILE_NOT_FOUND,
		UFILEERR_TOO_MANY_OPEN,
		UFILEERR_ACCESS_DENIED,
		UFILEERR_DISK_FULL,
		UFILEERR_FILE_EXISTS,
		// fatals (it means error in the library implementation)
		UFILEERR_FATAL=5000,
		UFILEERR_INTBUF_SMALL
	};

	namespace ulibpp_private_ {

		// common flags
		typedef std::uint32_t ufile_flag_type;

		// operation
		static const ufile_flag_type uppf_sync = static_cast<ufile_flag_type>(0x00000000);
		static const ufile_flag_type uppf_async = static_cast<ufile_flag_type>(0x80000000);

		static const ufile_flag_type uppf_file = static_cast<ufile_flag_type>(0x00000000);
		static const ufile_flag_type uppf_memmap = static_cast<ufile_flag_type>(0x10000000);
		static const ufile_flag_type uppf_oper_mask = static_cast<ufile_flag_type>(0x30000000);

		// open disposition flags & mask
		/* supported variants:
								file exists				file doesn't exist
		open						open						fail
		open|truncate				open, trunc					fail
		create						open						create
		create|truncate				open, trunc					create
		open|create					fail						create
		*/
		static const ufile_flag_type uppf_open = static_cast<ufile_flag_type>(0x00000100);
		static const ufile_flag_type uppf_create = static_cast<ufile_flag_type>(0x00000200);
		static const ufile_flag_type uppf_truncate = static_cast<ufile_flag_type>(0x00000400);
		static const ufile_flag_type uppf_open_mask = static_cast<ufile_flag_type>(0x00000700);

		// access flags & mask
		static const ufile_flag_type uppf_read = static_cast<ufile_flag_type>(0x00000001);
		static const ufile_flag_type uppf_write = static_cast<ufile_flag_type>(0x00000002);
		static const ufile_flag_type uppf_execute = static_cast<ufile_flag_type>(0x00000004);
		static const ufile_flag_type uppf_access_mask = static_cast<ufile_flag_type>(0x00000007);

		// sharing flags & mask
		static const ufile_flag_type uppf_shr_all = static_cast<ufile_flag_type>(0x00000000);
		static const ufile_flag_type uppf_shr_read = static_cast<ufile_flag_type>(0x00000010);
		static const ufile_flag_type uppf_shr_write = static_cast<ufile_flag_type>(0x00000020);
		static const ufile_flag_type uppf_shr_excl = static_cast<ufile_flag_type>(0x00000030);
		static const ufile_flag_type uppf_shr_mask = static_cast<ufile_flag_type>(0x00000030);

		// hints
		static const ufile_flag_type uppf_temp = static_cast<ufile_flag_type>(0x00001000);
		static const ufile_flag_type uppf_seq = static_cast<ufile_flag_type>(0x00002000);
		static const ufile_flag_type uppf_random = static_cast<ufile_flag_type>(0x00000000);
		static const ufile_flag_type uppf_nocache = static_cast<ufile_flag_type>(0x00004000);

		// type traits
		template <typename T>
		struct ufile_rw_traits {
			typedef T type;
			typedef T orig_type;
			static const std::size_t n = 1;
			static const std::size_t size = sizeof(T);
			typedef T *ptr_type;
			typedef const T *const_ptr_type;
			static const_ptr_type get_const_ptr(const T &t) { return &t; }
			static ptr_type get_ptr(T &t) { return &t; }
		};
		template <typename T>
		struct ufile_rw_traits<T*> {
			typedef T type;
			typedef T *orig_type;
			static const std::size_t n = 1;
			static const std::size_t size = sizeof(T);
			typedef T *ptr_type;
			typedef const T *const_ptr_type;
			static const_ptr_type get_const_ptr(const orig_type &t) { return t; }
			static ptr_type get_ptr(orig_type &t) { return t; }
		};
		template <typename T, std::size_t N>
		struct ufile_rw_traits<T[N]> {
			typedef T type;
			typedef T orig_type[N];
			static const std::size_t n = N;
			static const std::size_t size = sizeof(T);
			typedef T *ptr_type;
			typedef const T *const_ptr_type;
			static const_ptr_type get_const_ptr(const orig_type &t) { return t; }
			static ptr_type get_ptr(orig_type &t) { return t; }
		};

		template <typename F>
		struct ufile_attorney {
			static void set_error(F &f, ufile_error err) { f.set_last_error(err); }
		};

	}

	// ufile exception
	class ufile_exception : public std::exception {
	public:
		explicit ufile_exception(ufile_error err) : err_(err) { }
		virtual ~ufile_exception() throw() { }
		virtual const char *what() const throw();

		ufile_error get_err() const { return err_; }

	private:
		ufile_error err_;
	};

	class sync_file {
	public:
		// public flags for synchronous file
		typedef ulibpp_private_::ufile_flag_type flag_type;
		static const flag_type opn = ulibpp_private_::uppf_open;
		static const flag_type create = ulibpp_private_::uppf_create;
		static const flag_type truncate = ulibpp_private_::uppf_truncate;
		static const flag_type rd = ulibpp_private_::uppf_read;
		static const flag_type wr = ulibpp_private_::uppf_write;
		static const flag_type shr_all = ulibpp_private_::uppf_shr_all;
		static const flag_type shr_read = ulibpp_private_::uppf_shr_read;
		static const flag_type shr_write = ulibpp_private_::uppf_shr_write;
		static const flag_type shr_excl = ulibpp_private_::uppf_shr_excl;
		static const flag_type temp = ulibpp_private_::uppf_temp;
		static const flag_type seq = ulibpp_private_::uppf_seq;
		static const flag_type random = ulibpp_private_::uppf_random;
		static const flag_type nocache = ulibpp_private_::uppf_nocache;
		
		enum seek_method { begin, current, end };

		// constructors
		// throw ufile_exception, if an error occurs
		// for warnings they do nothing, but error is set

		// default constructor doesn't open any file
		sync_file() : f_(ulibpp_private_::ufile_invalid),err_(UFILEERR_OK) { }
		sync_file(const std::string &fname, flag_type flags) : err_(UFILEERR_OK) { open(fname, flags); }
		sync_file(const std::string &fname, flag_type flags, const cp_convertor &cp) : err_(UFILEERR_OK) { open(fname, flags, cp); }
		sync_file(const std::wstring &fname, flag_type flags) : err_(UFILEERR_OK) { open(fname, flags); }

		// open
		// throws ufile_exception, if an error occurs
		// for warnings it returns true and an error is set
		bool open(const std::string &fname, flag_type flags);			// OEM cp
		bool open(const std::string &fname, flag_type flags, const cp_convertor &cp) { return open(cp.cp2ws(fname), flags); }
		bool open(const std::wstring &fname, flag_type flags);

		// close
		// it can throw!!!
		void close();

		// destructor
		// catches any exception
		~sync_file() { try { close(); } catch(...) { } }

		// move semantics
		sync_file(const sync_file &) = delete;
		sync_file &operator=(const sync_file &) = delete;

		sync_file(sync_file &&f)
		{
			f_ = f.f_;
			err_ = f.err_;
			f.f_ = ulibpp_private_::ufile_invalid;
		}

		sync_file &operator=(sync_file &&f)
		{
			sync_file sf(std::move(f));
			std::swap(f_, sf.f_);
			std::swap(err_, sf.err_);
			return *this;
		}

		// file error
		ufile_error get_last_error() const { return err_; }

		// read, write
		// returns number of read/written elements
		template <typename T>
		std::size_t read(T &t) {
			return read_int_(ulibpp_private_::ufile_rw_traits<T>::get_ptr(t), 
							 ulibpp_private_::ufile_rw_traits<T>::size,
							 ulibpp_private_::ufile_rw_traits<T>::n);
		}
		template <typename T>
		std::size_t read(T &t, std::size_t n) {
			return read_int_(ulibpp_private_::ufile_rw_traits<T>::get_ptr(t), 
							 ulibpp_private_::ufile_rw_traits<T>::size*ulibpp_private_::ufile_rw_traits<T>::n,
							 n);
		}
		template <typename T>
		std::size_t write(const T &t) {
			return write_int_(ulibpp_private_::ufile_rw_traits<T>::get_const_ptr(t), 
							  ulibpp_private_::ufile_rw_traits<T>::size,
							  ulibpp_private_::ufile_rw_traits<T>::n);
		}
		template <typename T>
		std::size_t write(const T &t, std::size_t n) {
			return write_int_(ulibpp_private_::ufile_rw_traits<T>::get_const_ptr(t), 
							  ulibpp_private_::ufile_rw_traits<T>::size*ulibpp_private_::ufile_rw_traits<T>::n,
							  n);
		}

		// seek in random access files
		void seek(seek_method sm, std::int64_t pos);
		std::uint64_t tell() const;

		// other
		void flush();
		void set_filesize(std::uint64_t fs);
		std::uint64_t get_filesize() const;

	private:
		ulibpp_private_::ufile_type		f_;
		mutable ufile_error				err_;

		void set_last_error(ufile_error err) const { err_ = err; }		// attorney function

		std::size_t write_int_(const void *p, std::size_t s, std::size_t n);
		std::size_t read_int_(void *p, std::size_t s, std::size_t n);

		template<typename F>
		friend struct ulibpp_private_::ufile_attorney;
	};

	class async_file {
	public:
		// public flags for synchronous file
		typedef ulibpp_private_::ufile_flag_type flag_type;
		static const flag_type opn = ulibpp_private_::uppf_open;
		static const flag_type create = ulibpp_private_::uppf_create;
		static const flag_type truncate = ulibpp_private_::uppf_truncate;
		static const flag_type rd = ulibpp_private_::uppf_read;
		static const flag_type wr = ulibpp_private_::uppf_write;
		static const flag_type shr_all = ulibpp_private_::uppf_shr_all;
		static const flag_type shr_read = ulibpp_private_::uppf_shr_read;
		static const flag_type shr_write = ulibpp_private_::uppf_shr_write;
		static const flag_type shr_excl = ulibpp_private_::uppf_shr_excl;
		static const flag_type temp = ulibpp_private_::uppf_temp;
		static const flag_type seq = ulibpp_private_::uppf_seq;
		static const flag_type random = ulibpp_private_::uppf_random;
		static const flag_type nocache = ulibpp_private_::uppf_nocache;
		
		// constructors
		// throw ufile_exception, if an error occurs
		// for warnings they do nothing, but error is set

		// default constructor doesn't open any file
		async_file() : f_(ulibpp_private_::ufile_invalid),err_(UFILEERR_OK) { }
		async_file(const std::string &fname, flag_type flags) : err_(UFILEERR_OK) { open(fname, flags); }
		async_file(const std::string &fname, flag_type flags, const cp_convertor &cp) : err_(UFILEERR_OK) { open(fname, flags, cp); }
		async_file(const std::wstring &fname, flag_type flags) : err_(UFILEERR_OK) { open(fname, flags); }

		// open
		// throws ufile_exception, if an error occurs
		// for warnings it returns true and an error is set
		bool open(const std::string &fname, flag_type flags);			// OEM cp
		bool open(const std::string &fname, flag_type flags, const cp_convertor &cp) { return open(cp.cp2ws(fname), flags); }
		bool open(const std::wstring &fname, flag_type flags);

		// close
		// it can throw!!!
		void close();

		// destructor
		// catches any exception
		~async_file() { try { close(); } catch(...) { } }

		// move semantics
		async_file(const async_file &) = delete;
		async_file &operator=(const async_file &) = delete;

		async_file(async_file &&f)
		{
			f_ = f.f_;
			err_ = f.err_;
			f.f_ = ulibpp_private_::ufile_invalid;
		}

		async_file &operator=(async_file &&f)
		{
			async_file af(std::move(f));
			std::swap(f_, af.f_);
			std::swap(err_, af.err_);
			return *this;
		}

		// file error
		// file error
		ufile_error get_last_error() const { return err_; }

		// helper objects
		class wait_objects_group;

		class wait_object {
		public:
			typedef std::intptr_t magic;			// integral type expected

			explicit wait_object(magic m=0) : sz_(0),err_(UFILEERR_OK),my_wog_(0),af_(0),m_(m) {}		// unbound wait object
			wait_object(wait_objects_group &ag, magic m=0);		// wait object bound to the group
			~wait_object();

			// move semantics
			wait_object(const wait_object &) = delete;
			wait_object &operator=(const wait_object &) = delete;

			wait_object(wait_object &&wo)
			{
				sz_ = wo.sz_;
				err_ = wo.err_;
				my_wog_ = wo.my_wog_;
				awo_ = std::move(wo.awo_);
				af_ = wo.af_;
				m_ = wo.m_;
				wo.my_wog_ = nullptr;
			}

			wait_object &operator=(wait_object &&wo)
			{
				wait_object nwo(std::move(wo));
				std::swap(sz_, nwo.sz_);
				std::swap(err_, nwo.err_);
				std::swap(my_wog_, nwo.my_wog_);
				std::swap(awo_, nwo.awo_);
				std::swap(af_, nwo.af_);
				std::swap(m_, nwo.m_);
				return *this;
			}

			ufile_error get_error() const { return err_; }
			std::size_t get_size() const { return sz_; }
			magic get_magic() const { return m_; }

		private:
			std::size_t					sz_;
			ufile_error					err_;
			wait_objects_group			*my_wog_;
			ulibpp_private_::awo		awo_;
			const async_file			*af_;
			magic						m_;

			friend class wait_objects_group;
			friend class async_file;
			friend struct ulibpp_private_::awo;
			friend struct ulibpp_private_::aog;
		};

		class wait_objects_group {
		public:
			wait_objects_group() : err_(UFILEERR_OK) { }
			~wait_objects_group();

			// move semantics
			wait_objects_group(const wait_objects_group &) = delete;
			wait_objects_group &operator=(const wait_objects_group &) = delete;

			wait_objects_group(wait_objects_group &&wog)
			{
				spl_ = std::move(wog.spl_);
				aog_ = std::move(wog.aog_);
				awos_ = std::move(wog.awos_);	// this should empty the awos
				err_ = wog.err_;
			}

			wait_objects_group &operator=(wait_objects_group &&wog)
			{
				wait_objects_group nwog(std::move(wog));
				std::swap(spl_, nwog.spl_);
				std::swap(aog_, nwog.aog_);
				std::swap(awos_, nwog.awos_);
				std::swap(err_, nwog.err_);
				return *this;
			}

			wait_object *wait();	// throws, return NULL, if the wait was cancelled
			void cancel_wait();

			ufile_error get_last_error() const { return err_; }

			void add_awo(wait_object &wo);			// must NOT be called, when waiting
			void remove_awo(wait_object &wo);		// must NOT be called, when waiting

			std::size_t size() const { return awos_.size(); }

			static const std::size_t max_awos = ulibpp_private_::aog::max_hdls;

		private:
			typedef std::vector<wait_object*> wov_type;

			spin_lock					spl_;
			ulibpp_private_::aog		aog_;
			wov_type					awos_;
			ufile_error					err_;

			friend class wait_object;
			friend struct ulibpp_private_::awo;
			friend struct ulibpp_private_::aog;
		};

		// starts read, write
		// returns number of bytes, which should be processed
		template <typename T>
		std::size_t read(wait_object &wo, std::uint64_t fpos, T &t) {
			if(!wo.my_wog_)
			{
				err_ = UFILEERR_AWO_NOT_BOUND;
				throw ufile_exception(err_);
			}
			read_int_(wo, fpos,
					  ulibpp_private_::ufile_rw_traits<T>::get_ptr(t), 
					  ulibpp_private_::ufile_rw_traits<T>::size,
					  ulibpp_private_::ufile_rw_traits<T>::n);
			return ulibpp_private_::ufile_rw_traits<T>::size*ulibpp_private_::ufile_rw_traits<T>::n;
		}
		template <typename T>
		std::size_t read(wait_object &wo, std::uint64_t fpos, T &t, std::size_t n) {
			if(!wo.my_wog_)
			{
				err_ = UFILEERR_AWO_NOT_BOUND;
				throw ufile_exception(err_);
			}
			read_int_(wo, fpos,
					  ulibpp_private_::ufile_rw_traits<T>::get_ptr(t), 
					  ulibpp_private_::ufile_rw_traits<T>::size*ulibpp_private_::ufile_rw_traits<T>::n,
					  n);
			return ulibpp_private_::ufile_rw_traits<T>::size*ulibpp_private_::ufile_rw_traits<T>::n*n;
		}
		template <typename T>
		std::size_t write(wait_object &wo, std::uint64_t fpos, const T &t) {
			if(!wo.my_wog_)
			{
				err_ = UFILEERR_AWO_NOT_BOUND;
				throw ufile_exception(err_);
			}
			write_int_(wo, fpos,
					   ulibpp_private_::ufile_rw_traits<T>::get_const_ptr(t), 
					   ulibpp_private_::ufile_rw_traits<T>::size,
					   ulibpp_private_::ufile_rw_traits<T>::n);
			return ulibpp_private_::ufile_rw_traits<T>::size*ulibpp_private_::ufile_rw_traits<T>::n;
		}
		template <typename T>
		std::size_t write(wait_object &wo, std::uint64_t fpos, const T &t, std::size_t n) {
			if(!wo.my_wog_)
			{
				err_ = UFILEERR_AWO_NOT_BOUND;
				throw ufile_exception(err_);
			}
			write_int_(wo, fpos,
					   ulibpp_private_::ufile_rw_traits<T>::get_const_ptr(t), 
					   ulibpp_private_::ufile_rw_traits<T>::size*ulibpp_private_::ufile_rw_traits<T>::n,
					   n);
			return ulibpp_private_::ufile_rw_traits<T>::size*ulibpp_private_::ufile_rw_traits<T>::n*n;
		}
		// other
		void flush();
		void set_filesize(std::uint64_t fs);
		std::uint64_t get_filesize() const;

	private:
		ulibpp_private_::ufile_type		f_;
		mutable ufile_error				err_;

		void set_last_error(ufile_error err) const { err_ = err; }		// attorney function

		void write_int_(wait_object &wo, std::uint64_t fpos, const volatile void *p, std::size_t s, std::size_t n);
		void read_int_(wait_object &wo, std::uint64_t fpos, volatile void *p, std::size_t s, std::size_t n);

		template<typename F>
		friend struct ulibpp_private_::ufile_attorney;
	};

	class mm_file {
	public:
		// public flags for memory-mapped file
		typedef ulibpp_private_::ufile_flag_type flag_type;
		static const flag_type opn = ulibpp_private_::uppf_open;
		static const flag_type create = ulibpp_private_::uppf_create;
		static const flag_type truncate = ulibpp_private_::uppf_truncate;
		static const flag_type rd = ulibpp_private_::uppf_read;
		static const flag_type wr = ulibpp_private_::uppf_write;
		static const flag_type execute = ulibpp_private_::uppf_execute;
		static const flag_type shr_all = ulibpp_private_::uppf_shr_all;
		static const flag_type shr_read = ulibpp_private_::uppf_shr_read;
		static const flag_type shr_write = ulibpp_private_::uppf_shr_write;
		static const flag_type shr_excl = ulibpp_private_::uppf_shr_excl;
		static const flag_type temp = ulibpp_private_::uppf_temp;
		static const flag_type seq = ulibpp_private_::uppf_seq;
		static const flag_type random = ulibpp_private_::uppf_random;

		// public flags for mapping and views
		typedef flag_type access_flag_type;
		static const access_flag_type acc_rd = ulibpp_private_::uppf_read;
		static const access_flag_type acc_wr = ulibpp_private_::uppf_write;
		static const access_flag_type acc_execute = ulibpp_private_::uppf_execute;

		// constructors
		// throw ufile_exception, if an error occurs
		// for warnings they do nothing, but error is set

		// BEWARE!! for WR access a file must be opened with RD|WR flags

		// default constructor doesn't open any file
		mm_file() : f_(ulibpp_private_::ufile_invalid),err_(UFILEERR_OK),mmf_(ulibpp_private_::ummfile_invalid) { }
		mm_file(const std::string &fname, flag_type flags) : err_(UFILEERR_OK),mmf_(ulibpp_private_::ummfile_invalid) { open(fname, flags); }
		mm_file(const std::string &fname, flag_type flags, const cp_convertor &cp) : err_(UFILEERR_OK),mmf_(ulibpp_private_::ummfile_invalid) { open(fname, flags, cp); }
		mm_file(const std::wstring &fname, flag_type flags) : err_(UFILEERR_OK),mmf_(ulibpp_private_::ummfile_invalid) { open(fname, flags); }

		// open
		// throws ufile_exception, if an error occurs
		// for warnings it returns true and an error is set
		bool open(const std::string &fname, flag_type flags);			// OEM cp
		bool open(const std::string &fname, flag_type flags, const cp_convertor &cp) { return open(cp.cp2ws(fname), flags); }
		bool open(const std::wstring &fname, flag_type flags);

		// close
		// it can throw!!!
		void close();

		// destructor
		// catches any exception
		~mm_file() { try { close(); } catch(...) { } }

		// move semantics
		mm_file(const mm_file &) = delete;
		mm_file &operator=(const mm_file &) = delete;

		mm_file(mm_file &&f)
		{
			f_ = f.f_;
			err_ = f.err_;
			mmf_ = f.mmf_;
			f.f_ = ulibpp_private_::ufile_invalid;
			f.mmf_ = ulibpp_private_::ummfile_invalid;
		}

		mm_file &operator=(mm_file &&f)
		{
			mm_file mf(std::move(f));
			std::swap(f_, mf.f_);
			std::swap(err_, mf.err_);
			std::swap(mmf_, mf.mmf_);
			return *this;
		}

		// file error
		ufile_error get_last_error() const { return err_; }

		// other
		void flush();
		void set_filesize(std::uint64_t fs);
		std::uint64_t get_filesize() const;
		static std::size_t get_pagesize();

		// mapping
		void map(access_flag_type flags, std::size_t size);
		void unmap();

		// views
		class mm_view {
		public:
			mm_view() : umv_(),ummf_(0) { }
			~mm_view();
			template <typename T>
			T *get_addr() const { return umv_.get_addr<T>(); }

#if ULIBPP_CC_DELETED_FNCS
			mm_view(const mm_view &) = delete;
			mm_view & operator=(const mm_view &) = delete;
#endif
		private:
			mm_view(mm_file &ummf, const ulibpp_private_::umapview_type &umv) : umv_(umv),ummf_(&ummf) { }

			ulibpp_private_::umapview_type  umv_;
			mm_file							*ummf_;

			friend class mm_file;
		};
		std::unique_ptr<mm_view> create_view(access_flag_type flags, std::uint64_t offset, std::size_t size, void *hint_addr);
	private:
		ulibpp_private_::ufile_type		f_;
		mutable ufile_error				err_;
		ulibpp_private_::ummfile_type	mmf_;

		void set_last_error(ufile_error err) const { err_ = err; }		// attorney function

		template<typename F>
		friend struct ulibpp_private_::ufile_attorney;
	};

}

#endif
