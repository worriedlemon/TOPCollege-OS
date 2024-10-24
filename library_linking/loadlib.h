#pragma once

#ifdef _runtime

#if defined(_WIN32) || defined(WIN32)
#include <windows.h>

typedef HINSTANCE libhandle_t;
typedef double (__cdecl* sqrtfunc_t)(double);

libhandle_t LoadMyLibrary(const char* name)
{
	return LoadLibraryA(name);
}


void * GetSymbolAddress(libhandle_t handle, const char* symbol)
{
    return GetProcAddress(handle, symbol);
}

void UnloadMyLibrary(libhandle_t handle)
{
	FreeLibrary(handle);
}

#else // UNIX
#include <dlfcn.h>

typedef void * libhandle_t;
typedef double (* sqrtfunc_t)(double);

libhandle_t LoadMyLibrary(const char* name)
{
	return dlopen(name, RTLD_LAZY);
}


void * GetSymbolAddress(libhandle_t handle, const char* symbol)
{
    return dlsym(handle, symbol);
}

void UnloadMyLibrary(libhandle_t handle)
{
	dlclose(handle);
}

#endif // _WIN32

sqrtfunc_t GetSqrtFunction(libhandle_t handle)
{
	if (handle == nullptr)
    {
        return nullptr;
    }

	return reinterpret_cast<sqrtfunc_t>(GetSymbolAddress(handle, "my_sqrt"));
}

#else

#if defined(_WIN32) || defined(WIN32)
extern "C" double __cdecl my_sqrt(double);
#else // UNIX
extern "C" double my_sqrt(double);
#endif // _WIN32

#endif // _runtime
