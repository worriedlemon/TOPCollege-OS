#pragma once

#ifndef ALLOCATE_ADDRESS_H
#define ALLOCATE_ADDRESS_H

#include <string>
#include <cstdint>

// Function to convert string to address
uint64_t GetAddressFromStr(std::string& str)
{
	uint64_t address = 0;

	// If address starts with `0x` it is also valid
	size_t index;
	if (str.substr(0, 2) == "0x")
	{
		index = 2;
	}
	else index = 0;

	for (; index < str.size(); index++)
	{
		// Taking a symbol
		char val = str[index], offset = 0;

		// Depending on a symbol, converting it from a hexadecimal number to decimal (case insensitive)
        if (val >= '0' && val <= '9')
        {
            offset = (val - '0');
        }
        else if ((val >= 'a' && val <= 'z') || (val >= 'A' && val <= 'Z'))
        {
            val |= 0b00100000; // OR with 32 means force lowercase
            offset = (val - 'a' + 10);
        }
        else
        {
            // If values are not in range [0-9,A-F], this string is not valid
            return 0;
        }

		// Getting address
		address = (address << 4) + offset;
	}

	return address;
}

#if defined(_WIN32) || defined(WIN32)

#include <windows.h>

template <typename T>
T* AllocateAddress(T* possibleAddress, size_t sizeInBytes)
{
    return reinterpret_cast<T*>(VirtualAlloc(possibleAddress, sizeInBytes, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE));
}

void DeallocateAddress(void* address, size_t /*size*/)
{
    VirtualFree(address, 0, MEM_RELEASE);
}

#elif defined(__unix__)

#include <sys/mman.h>

template < typename T >
T* AllocateAddress(T* possibleAddress, size_t sizeInBytes)
{
    return reinterpret_cast<T*>(mmap(possibleAddress, sizeInBytes, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0));
}

void DeallocateAddress(void* address, size_t size)
{
    munmap(address, size);
}

#else
#error Unknown operating system
#endif // _WIN32

#endif // ALLOCATE_ADDRESS_H
