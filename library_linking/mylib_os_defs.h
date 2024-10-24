#pragma once

#if defined(_WIN32) || defined(WIN32)

#ifndef MYLIB_API
#define MYLIB_API __cdecl
#endif // MYLIB_API

#ifndef DLL_EXPORT
#define DLL_EXPORT __declspec(dllexport)
#endif // DLL_EXPORT

#else // UNIX

#ifndef MYLIB_API
#define MYLIB_API __attribute__(( visibility("default") ))
#endif // MYLIB_API

#ifndef DLL_EXPORT
#define DLL_EXPORT
#endif // DLL_EXPORT

#endif // _WIN32

