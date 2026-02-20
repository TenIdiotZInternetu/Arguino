/*

	upp_mthr.cpp

	Kuba, 2006-2007, 2016
	Part of ULIB++

*/

#include <cassert>
#include <string>
#include <bitset>
#include <set>
#include <map>
#include <cstdio>

#include "upp_mthr.hpp"
#include "upp_file.hpp"

#if ULIBPP_OS_PLATFORM==ULIBPP_OS_PLATFORM_WIN_NT ||		\
	ULIBPP_OS_PLATFORM==ULIBPP_OS_PLATFORM_WIN_NT64

#include <Windows.h>
#include <powerbase.h>

#if ULIBPP_CC_PLATFORM==ULIBPP_CC_PLATFORM_MSVC && ULIBPP_CC_VERSION >= 1800

#include <VersionHelpers.h>

#endif

#elif ULIBPP_OS_PLATFORM==ULIBPP_OS_PLATFORM_LINUX ||		\
	  ULIBPP_OS_PLATFORM==ULIBPP_OS_PLATFORM_K1OM

#include <iostream>
#include <sys/time.h>
#include <sys/mman.h>
#include <cctype>

#if ULIBPP_OS_PLATFORM != ULIBPP_OS_PLATFORM_K1OM

#include <numa.h>

#endif

#endif

using namespace ulibpp;
using namespace std;

namespace ulibpp {

	void event::wait()
	{
		std::unique_lock<std::mutex> lk(mtx_);
		cv_.wait(lk, [this]{ return signaled_; });
		if(automatic_)
			signaled_ = false;
	}

	void event::set()
	{
		{
			std::lock_guard<std::mutex>	lg(mtx_);
			signaled_ = true;
		}
		if(automatic_)
			cv_.notify_one();
		else
			cv_.notify_all();
	}

	void event::reset()
	{
		std::lock_guard<std::mutex>	lg(mtx_);
		signaled_ = false;
	}

}
