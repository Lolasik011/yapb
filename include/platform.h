//
// Yet Another POD-Bot, based on PODBot by Markus Klinge ("CountFloyd").
// Copyright (c) YaPB Development Team.
//
// This software is licensed under the BSD-style license.
// Additional exceptions apply. For full license details, see LICENSE.txt or visit:
//     https://yapb.ru/license
//

#pragma once

// detects the build platform
#if defined(__linux__)
   #define PLATFORM_LINUX
#elif defined(__APPLE__)
   #define PLATFORM_OSX
#elif defined(_WIN32)
   #define PLATFORM_WIN32
#endif

// by default sse has everyone
#define PLATFORM_HAS_SSE2

// detects the compiler
#if defined(_MSC_VER)
   #define CXX_MSVC
#elif defined(__clang__)
   #define CXX_CLANG
#endif

// configure export macros
#if defined(PLATFORM_WIN32)
   #define SHARED_LIBRARAY_EXPORT extern "C" __declspec (dllexport)
#elif defined(PLATFORM_LINUX) || defined(PLATFORM_OSX)
   #define SHARED_LIBRARAY_EXPORT extern "C" __attribute__ ((visibility ("default")))
#else
   #error "Can't configure export macros. Compiler unrecognized."
#endif

// operating system specific macros, functions and typedefs
#ifdef PLATFORM_WIN32

   #include <direct.h>
   #include <string.h>

   #define STD_CALL __stdcall

   #define DLL_ENTRYPOINT int STD_CALL DllMain (HINSTANCE, DWORD dwReason, LPVOID)
   #define DLL_DETACHING (dwReason == DLL_PROCESS_DETACH)
   #define DLL_RETENTRY return TRUE

   #if defined(CXX_MSVC) && !defined (_M_X64)
      #define DLL_GIVEFNPTRSTODLL extern "C" void STD_CALL
   #elif defined(CXX_CLANG) || defined (_M_X64)
      #define DLL_GIVEFNPTRSTODLL SHARED_LIBRARAY_EXPORT void STD_CALL
   #endif

   // specify export parameter
   #if defined(CXX_MSVC) || defined (CXX_CLANG)
      #if !defined (_M_X64)
         #pragma comment(linker, "/EXPORT:GiveFnptrsToDll=_GiveFnptrsToDll@8,@1")
      #endif
      #pragma comment(linker, "/SECTION:.data,RW")
   #endif

#elif defined(PLATFORM_LINUX) || defined(PLATFORM_OSX)
   #include <dlfcn.h>
   #include <errno.h>
   #include <fcntl.h>
   #include <sys/stat.h>
   #include <unistd.h>

   #include <arpa/inet.h>
   #include <netdb.h>
   #include <netinet/in.h>
   #include <sys/socket.h>
   #include <sys/types.h>

   #define DLL_ENTRYPOINT __attribute__ ((destructor)) void _fini (void)
   #define DLL_DETACHING TRUE
   #define DLL_RETENTRY return
   #define DLL_GIVEFNPTRSTODLL extern "C" void __attribute__ ((visibility ("default")))

   #define STD_CALL /* */

   // android is a linux with a special cases
   // @todo: sse should be working ok on x86 android?
   #if defined(__ANDROID__)
      #define PLATFORM_ANDROID
      #undef PLATFORM_HAS_SSE2
   #endif
#else
   #error "Platform unrecognized."
#endif
