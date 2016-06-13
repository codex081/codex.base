#ifndef __codex_predefs_h__
#define __codex_predefs_h__

#if defined( _WIN32 )

#include <WinSock2.h>
#include <MSWSock.h>
#include <ws2tcpip.h>
#include <Windows.h>

#else

#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#endif

#include <errno.h>

#include <iostream>
#include <mutex>
#include <atomic>
#include <system_error>
#include <memory>
#include <deque>
#include <vector>

#if defined( _WIN32 )

/// @brief win32 predef
/// @detail
/// _WIN32 always
/// _WIN64 : win64 application
/// _M_AMD64 or _M_X64 : x64
/// _M_IX86 : x86
/// _M_ARM : ARM 
/// _M_ARMT : ARM Thumb Mode
#define __codex_win32__

#elif defined( __linux__ )

/// @brief linux predef
/// @detail
/// __x86_64__ : x64
/// __arm__ : ARM
/// __thumb__ : ARM Thumb Mode
/// __i386__ : x86
/// __ANDROID__ : android
#define __codex_linux__

#elif defined( __APPLE__ )

/// @brief apple predef
/// @detail
/// __x86_64__ : x64
/// __arm__ : ARM
/// __thumb__ : ARM Thumb Mode
/// __i386__ : x86
/// TARGET_IPHONE_SIMULATOR
/// TARGET_OS_IPHONE
/// TARGET_OS_MAC
#define __codex_apple__

#endif

#ifndef DISALLOW_COPY
#define DISALLOW_COPY( clazz )\
	clazz( const clazz& rhs );\
	clazz& operator=( const clazz rhs );
#endif

namespace codex { namespace threading {

  using mutex = std::mutex;
  template < class T > using lock_guard = std::lock_guard< T >;
  template < class T > using atomic = std::atomic< T >;

}}



#endif
