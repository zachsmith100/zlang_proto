#pragma once

// STL
//////
#include <iostream>
#include <string>
#include <vector>
#include <sstream>

#define PLATFORM_NAME_WINDOWS "windows"
#define PLATFORM_NAME_LINUX "windows"
#define PLATFORM_NAME_WINDOWS "windows"

// Platform Constants
/////////////////////
#if defined(unix) || defined(__unix__) || defined(__unix) || defined(__posix) || defined(__APPLE__)
  #define PLATFORM_UNIX
  #ifndef __unix__
    #define __unix__
  #endif
  #define PLATFORM_NAME "linux"
#elif defined(WIN32)
  #define PLATFORM_WINDOWS
  #define PLATFORM_WIN32
  #define PLATFORM_NAME "windows"
#else
  #error Unsupported OS
#endif

#include "Path.h"

// Platform dependencies

#ifdef PLATFORM_WINDOWS
	#include <shlobj.h>
	#include <windows.h>
#else
	#include <dirent.h>
#endif

#ifdef __APPLE__
	#include "fmemopen.h"
	#undef PLATFORM_NAME
	#define PLATFORM_NAME "macosx"
#endif

#define BASE_ID		"base"
#define MY_ID		"MY"

