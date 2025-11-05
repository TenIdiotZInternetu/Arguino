#ifndef ULIBPP_UPP_MTHR_HPP_
#define ULIBPP_UPP_MTHR_HPP_

/*

	upp_mthr.hpp

	Multi-threaded support & policies

	Kuba, 2006-2007
	Part of ULIB++

*/

#include <stdexcept>
#include <deque>
#include <string>
#include <vector>
#include <cassert>
#include <algorithm>
#include <set>
#include <atomic>
#include <condition_variable>
#include <mutex>

#include "upp_def.hpp"

// OS platform dependent code
#if ULIBPP_OS_PLATFORM==ULIBPP_OS_PLATFORM_WIN_NT ||		\
	ULIBPP_OS_PLATFORM==ULIBPP_OS_PLATFORM_WIN_NT64

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <windows.h>

#elif ULIBPP_OS_PLATFORM==ULIBPP_OS_PLATFORM_LINUX ||		\
	  ULIBPP_OS_PLATFORM==ULIBPP_OS_PLATFORM_K1OM

#include <pthread.h>
#include <semaphore.h>
#include <errno.h>
#include <sys/fcntl.h>
#include <sched.h>
  
#else
#error Unsupported OS platform
#endif

namespace ulibpp {

#if ULIBPP_OS_PLATFORM==ULIBPP_OS_PLATFORM_WIN_NT ||		\
	ULIBPP_OS_PLATFORM==ULIBPP_OS_PLATFORM_WIN_NT64

	class spin_lock {
	public:
		spin_lock():sl_(0) { }
		~spin_lock() {}

		// move semantics
		spin_lock(const spin_lock &) = delete;
		spin_lock &operator=(const spin_lock &) = delete;

		spin_lock(spin_lock &&sl)
		{
			sl_ = sl.sl_;
		}

		spin_lock &operator=(spin_lock &&sl)
		{
			spin_lock nsl(std::move(sl));
			std::swap(sl_, nsl.sl_);
			return *this;
		}

		void acquire()
		{ 
			while(_InterlockedCompareExchange(&sl_, 1, 0))
			{
			}
		}
		bool try_acquire()
		{
			return !_InterlockedCompareExchange(&sl_, 1, 0);
		}
		void release() { _InterlockedExchange(&sl_, 0); }
	private:
		long volatile				sl_;
	};

	class semaphore {
	public:
		semaphore() : sem_(nullptr) {}
		explicit semaphore(int initval)
		{
			if((sem_=::CreateSemaphore(0, initval, 0x7FFFFFFF, 0))==NULL) 
				throw std::runtime_error("Unable to create a semaphore"); 
		}
		~semaphore() { if(sem_) ::CloseHandle(sem_); }
		// move semantics
		semaphore(const semaphore &) = delete;
		semaphore &operator=(const semaphore &) = delete;

		semaphore(semaphore &&s)
		{
			sem_ = s.sem_;
			s.sem_ = nullptr;
		}

		semaphore &operator=(semaphore &&s)
		{
			semaphore ns(std::move(s));
			std::swap(sem_, ns.sem_);
			return *this;
		}

		void acquire()
		{
			const DWORD rv=::WaitForSingleObject(sem_, INFINITE);
			if(rv==WAIT_FAILED)
				throw std::runtime_error("Not waiting");
		}
		void release() { ::ReleaseSemaphore(sem_, 1, 0); }
	private:
		::HANDLE				sem_;
	};


	class named_semaphore {
	public:
		named_semaphore() : sem_(nullptr) {}
		~named_semaphore() { if(sem_) ::CloseHandle(sem_); }
		// move semantics
		named_semaphore(const named_semaphore &) = delete;
		named_semaphore &operator=(const named_semaphore &) = delete;

		named_semaphore(named_semaphore &&s)
		{
			sem_ = s.sem_;
			s.sem_ = nullptr;
		}

		named_semaphore &operator=(named_semaphore &&s)
		{
			named_semaphore ns(std::move(s));
			std::swap(sem_, ns.sem_);
			return *this;
		}

		void create(const std::string &semname, int initval)
		{
			std::string rn = "Global\\"+semname;
			if((sem_=::CreateSemaphoreA(0, initval, 0x7FFFFFFF, rn.c_str()))==NULL)
				throw std::runtime_error("Unable to create a named semaphore");
			if(::GetLastError()==ERROR_ALREADY_EXISTS)
				throw std::runtime_error("Named semaphore already exists");
		}
		void open(const std::string &semname)
		{
			std::string rn = "Global\\"+semname;
			if((sem_=::OpenSemaphoreA(SEMAPHORE_ALL_ACCESS, 0, rn.c_str()))==NULL)
				throw std::runtime_error("Unable to open a named semaphore");
		}

#if ULIBPP_CC_NOEXCEPT
		// OK=true, ERR=false
		bool create(const std::string &semname, int initval, const std::nothrow_t &) noexcept
		{
			std::string rn = "Global\\"+semname;
			if((sem_=::CreateSemaphoreA(0, initval, 0x7FFFFFFF, rn.c_str()))==NULL)
				return false;
			if(::GetLastError()==ERROR_ALREADY_EXISTS)
				return false;
			return true;
		}
		bool open(const std::string &semname, const std::nothrow_t &) noexcept
		{
			std::string rn = "Global\\"+semname;
			if((sem_=::OpenSemaphoreA(SEMAPHORE_ALL_ACCESS, 0, rn.c_str()))==NULL)
				return false;
			return true;
		}
#endif

		void acquire() { ::WaitForSingleObject(sem_, INFINITE); }
		void release() { ::ReleaseSemaphore(sem_, 1, 0); }
	private:
		::HANDLE				sem_;
	};

	class critical_section {
	public:
		critical_section() : valid_(true) { ::InitializeCriticalSection(&cs_); }
		~critical_section() { if(valid_) ::DeleteCriticalSection(&cs_); }
		// move semantics
		critical_section(const critical_section &) = delete;
		critical_section &operator=(const critical_section &) = delete;

		critical_section(critical_section &&cs)
		{
			cs_ = std::move(cs.cs_);
			valid_ = cs.valid_;
			cs.valid_ = false;
		}

		critical_section &operator=(critical_section &&cs)
		{
			critical_section ncs(std::move(cs));
			std::swap(cs_, ncs.cs_);
			return *this;
		}

		void acquire() { ::EnterCriticalSection(&cs_); }
		void release() { ::LeaveCriticalSection(&cs_); }
	private:
		::CRITICAL_SECTION		cs_;
		bool					valid_;
	};

	class ULIBPP_DEPRECATED waiter {
	public:
		waiter() { }
		explicit waiter(std::uint32_t ms) { wait(ms); }
		void wait(std::uint32_t ms) { ::Sleep(static_cast<DWORD>(ms)); }
	};

#elif ULIBPP_OS_PLATFORM==ULIBPP_OS_PLATFORM_LINUX ||		\
	  ULIBPP_OS_PLATFORM==ULIBPP_OS_PLATFORM_K1OM

	class spin_lock {
	public:
		spin_lock() : valid_(true) { ::pthread_spin_init(&sl_, 0); }
		~spin_lock() { if(valid_) ::pthread_spin_destroy(&sl_); }

		// move semantics
		spin_lock(const spin_lock &) = delete;
		spin_lock &operator=(const spin_lock &) = delete;

		spin_lock(spin_lock &&sl)
		{
			sl_ = std::move(sl.sl_);
			valid_ = sl.valid_;
			sl.valid_ = false;
		}

		spin_lock &operator=(spin_lock &&sl)
		{
			spin_lock nsl(std::move(sl));
			std::swap(sl_, nsl.sl_);
			std::swap(valid_, nsl.valid_);
			return *this;
		}

		bool try_acquire() { return !::pthread_spin_trylock(&sl_); }
		void acquire() { ::pthread_spin_lock(&sl_); }
		void release() { ::pthread_spin_unlock(&sl_); }
	private:
		::pthread_spinlock_t		sl_;
		bool						valid_;
	};

	class semaphore {
	public:
		semaphore() : valid_(false) {}
		explicit semaphore(int initval=1) : valid_(true)
		{
			if(::sem_init(&sem_, 1, initval))
				throw std::runtime_error("Unable to open a semaphore");
		}
		~semaphore() { if(valid_) ::sem_destroy(&sem_); }
		// move semantics
		semaphore(const semaphore &) = delete;
		semaphore &operator=(const semaphore &) = delete;

		semaphore(semaphore &&s)
		{
			sem_ = std::move(s.sem_);
			valid_ = s.valid_;
			s.valid_ = false;
		}

		semaphore &operator=(semaphore &&s)
		{
			semaphore ns(std::move(s));
			std::swap(sem_, ns.sem_);
			std::swap(valid_, ns.valid_);
			return *this;
		}

		void acquire() {
			bool cont = true;
			while (cont) {
				if (::sem_wait(&sem_) != 0) {
					if (errno != EINTR)
					{
						//perror("sem_wait");
						throw std::runtime_error("Semaphore wait failed");
					}
				} else {
					cont = false;
				}
			}
		}
		void release() { if(::sem_post(&sem_)!=0) throw std::runtime_error("Semaphore post failed"); }
//		int getval() { int sval; ::sem_getvalue(&sem_, &sval); return sval; }
	private:
		::sem_t						sem_;
		bool						valid_;
	};

	class named_semaphore {
	public:
		named_semaphore() : valid_(false) {}
		~named_semaphore()
		{
			if(valid_)
			{
				::sem_close(sem_);
				if(!semname_.empty())
					::sem_unlink(semname_.c_str());
			}
		}
		// move semantics
		named_semaphore(const named_semaphore &) = delete;
		named_semaphore &operator=(const named_semaphore &) = delete;

		named_semaphore(named_semaphore &&s)
		{
			sem_ = std::move(s.sem_);
			valid_ = s.valid_;
			semname_ = s.semname_;
			s.valid_ = false;
		}

		named_semaphore &operator=(named_semaphore &&s)
		{
			named_semaphore ns(std::move(s));
			std::swap(sem_, ns.sem_);
			std::swap(valid_, ns.valid_);
			std::swap(semname_, ns.semname_);
			return *this;
		}

		void create(const std::string &semname, int initval)
		{
			semname_ = '/'+semname;
			if((sem_=::sem_open(semname_.c_str(), O_CREAT|O_EXCL, S_IRWXU|S_IRWXG|S_IRWXO, static_cast<unsigned>(initval)))==reinterpret_cast< ::sem_t*>(SEM_FAILED))
				throw std::runtime_error(errno==EEXIST?"Named semaphore already exists":"Unable to create a named semaphore");
			valid_ = true;
		}
		void open(const std::string &semname)
		{
			std::string rn='/'+semname;
			if((sem_=::sem_open(rn.c_str(), 0, S_IRWXU|S_IRWXG|S_IRWXO, 0))==reinterpret_cast< ::sem_t*>(SEM_FAILED))
				throw std::runtime_error("Unable to open a named semaphore");
		}

#if ULIBPP_CC_NOEXCEPT
		// OK=true, ERR=false
		bool create(const std::string &semname, int initval, const std::nothrow_t &) noexcept
		{
			semname_ = '/'+semname;
			if((sem_=::sem_open(semname_.c_str(), O_CREAT|O_EXCL, S_IRWXU|S_IRWXG|S_IRWXO, static_cast<unsigned>(initval)))==reinterpret_cast< ::sem_t*>(SEM_FAILED))
				return false;
			valid_ = true;
			return true;
		}
		bool open(const std::string &semname, const std::nothrow_t &) noexcept
		{
			std::string rn='/'+semname;
			if((sem_=::sem_open(rn.c_str(), 0, S_IRWXU|S_IRWXG|S_IRWXO, 0))==reinterpret_cast< ::sem_t*>(SEM_FAILED))
				return false;
			return true;
		}
#endif

		void acquire() { ::sem_wait(sem_); }
		void release() { ::sem_post(sem_); }
	private:
		::sem_t					*sem_;
		std::string				semname_;
		bool					valid_;
	};

	class critical_section {
	public:
		critical_section():cs_(1) { }
		~critical_section() { }
		void acquire() { cs_.acquire(); }
		void release() { cs_.release(); }
	private:
		semaphore				cs_;
	};

	class ULIBPP_DEPRECATED waiter {
	public:
		waiter() { }
		waiter(std::uint32_t ms) { wait(ms); }
		void wait(std::uint32_t ms) 
		{
			::timeval stv;
			stv.tv_sec = ms/1000;
			stv.tv_usec = (ms%1000)*1000;
			::select(0,0,0,0,&stv);
		}
	};

#else
#error Unsupported OS platform
#endif

	class event {
	public:
		explicit event(bool automatic = false) : signaled_(false),automatic_(automatic) {}
		// move semantics
		event(const event &) = delete;
		event &operator=(const event &) = delete;

		void wait();		// wait for signaled event
		void set();			// signals the event, automatic event resets immediately after one wait, manual reset stays signaled until manual reset is called
		void reset();		// designals manual event

	private:
		std::condition_variable	cv_;
		std::mutex				mtx_;
		bool					signaled_;
		bool					automatic_;
	};

	template <typename T> class message_queue {
	public:
		virtual void send(T message) = 0;
		virtual T receive() = 0;
		virtual ~message_queue() {}
	};

	template <typename T> class message_queue_limited : public message_queue<T> {
	public:
		message_queue_limited(size_t size)
			: full_(0), empty_(size)
		{}

		virtual void send(T message)
		{
			empty_.acquire();
			section_.acquire();
			queue_.push_back(message);
			section_.release();
			full_.release();
		}

		virtual T receive()
		{
			full_.acquire();
			section_.acquire();
			T message = queue_.front();
			queue_.pop_front();
			section_.release();
			empty_.release();

			return message;
		}

	private:
		semaphore full_;
		semaphore empty_;
		critical_section section_;
		std::deque<T> queue_;
	};

	template <typename T> class message_queue_unlimited : public message_queue<T> {
	public:
		message_queue_unlimited()
			: full_(0)
		{}

		virtual void send(T message)
		{
			section_.acquire();
			queue_.push_back(message);
			section_.release();
			full_.release();
		}

		virtual T receive()
		{
			full_.acquire();
			section_.acquire();
			T message = queue_.front();
			queue_.pop_front();
			section_.release();

			return message;
		}

	private:
		semaphore full_;
		critical_section section_;
		std::deque<T> queue_;
	};


	class fair_spin_lock {
	public:
		fair_spin_lock()
			: ticket_(0), client_(1)
		{ }
		~fair_spin_lock() {}
		void acquire()
		{ 
			volatile std::uint16_t ticket = ++ticket_;

			while(ticket != client_)
			{
			}
		}
		void release() { ++client_; }
	private:
		std::atomic<std::uint16_t> ticket_;
		std::atomic<std::uint16_t> client_;

		fair_spin_lock(const spin_lock &);
		fair_spin_lock &operator=(const spin_lock &);
	};


	/**
	 * \brief Scope lock encapsulates usage of other type of locks (spin_lock, critical_section, ...) within code scope.
	 *
	 * Scope lock locks given synchronization primitive in constructor and release it in destructor.
	 * Therefore the lock is always released when current scope of the code is left (even if exception has been thrown).
	 */
	template<typename T> class scope_lock {
	public:
		scope_lock(T &lock) : lock_(&lock)
		{
			lock_->acquire();
		}

		~scope_lock()
		{
			lock_->release();
		}

	private:
		T *lock_;
	};

	class mutex {
	public:
		mutex()
			: sem_(1)
		{}

		void acquire()
		{
			sem_.acquire();
		}

		void release()
		{
			sem_.release();
		}
	private:
		semaphore sem_;
	};

	template <class TRead_lock = spin_lock, class TWrite_lock = mutex>
	class rw_lock {
	public:

		typedef TRead_lock read_lock_type;
		typedef TWrite_lock write_lock_type;

		rw_lock()
			: readers_(0)
		{}

		void acquire_read()
		{
			rlock_.acquire();
			if (++readers_ == 1)
				wlock_.acquire();
			rlock_.release();
		}

		void release_read()
		{
			rlock_.acquire();
			if (--readers_ == 0)
				wlock_.release();
			rlock_.release();
		}

		void acquire_write()
		{
			wlock_.acquire();
		}

		void release_write()
		{
			wlock_.release();
		}

		volatile size_t readers_;
		TRead_lock rlock_;
		TWrite_lock wlock_;
	};

	template <class TRwlock>
	class scope_read_lock {
	public:

		scope_read_lock(TRwlock &lock)
			: lock_(&lock)
		{
			lock_->acquire_read();
		}

		~scope_read_lock()
		{
			lock_->release_read();
		}

	private:
		TRwlock *lock_;
	};

	template <class TRwlock>
	class scope_write_lock {
	public:

		scope_write_lock(TRwlock &lock)
			: lock_(&lock)
		{
			lock_->acquire_write();
		}

		~scope_write_lock()
		{
			lock_->release_write();
		}

	private:
		TRwlock *lock_;
	};

} // end of namespace ulibpp

#endif
