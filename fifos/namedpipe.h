#pragma once

#ifndef NAMEDPIPE_H
#define NAMEDPIPE_H

#include <string>
#include <cstring>
#include <cstdlib>
#include <vector>

#if defined(_WIN32) || defined(WIN32)
#include <windows.h>
#define O_WRONLY GENERIC_WRITE
#define O_RDONLY GENERIC_READ
#define PIPE_WRONLY PIPE_ACCESS_OUTBOUND
#define PIPE_RDONLY PIPE_ACCESS_INBOUND
constexpr const char* PIPE_NAME = R"(\\.\pipe\my_named_pipe)";

typedef HANDLE fifo_handle_t;

fifo_handle_t CreateFifo(const char* name, DWORD access)
{
    fifo_handle_t h = CreateNamedPipeA(
        name,
        access,
        PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT,
        1,
        1024,
        1024,
        0,
        nullptr
    );
    if (!ConnectNamedPipe(h, nullptr))
    {
        CloseHandle(h);
        return INVALID_HANDLE_VALUE;
    }
    return h;
}

fifo_handle_t OpenFifo(const char* name, DWORD access)
{
     if (!WaitNamedPipeA(name, 0))
     {
         return INVALID_HANDLE_VALUE;
     }

    return CreateFileA(
         name,
         access,
         0,
         nullptr,
         OPEN_EXISTING,
         0,
         nullptr
     );
}

bool ReadFromFifo(fifo_handle_t handle, std::string& result)
{
    DWORD bytesRead;
    std::vector<char> buffer(result.capacity());
    bool ok = ReadFile(handle, buffer.data(), static_cast<DWORD>(buffer.size()), &bytesRead, nullptr);
    if (ok)
    {
        result.append(buffer.data(), buffer.data() + static_cast<size_t>(bytesRead));
    }
    return ok;
}

bool WriteToFifo(fifo_handle_t handle, const std::string& result)
{
    DWORD totalWritten = 0;
    DWORD bytesWritten;
    while (totalWritten < result.size())
    {
        if (WriteFile(handle, result.data() + totalWritten, static_cast<DWORD>(result.size() - totalWritten), &bytesWritten, nullptr))
        {
            totalWritten += bytesWritten;
        }
        else
        {
            return false;
        }
    }
    return true;
}

void CloseFifo(fifo_handle_t handle)
{
    CloseHandle(handle);
}

#define RemoveFifo(...)

#else // Unix
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#define INVALID_HANDLE_VALUE -1
#define PIPE_WRONLY O_WRONLY
#define PIPE_RDONLY O_RDONLY

constexpr const char* PIPE_NAME = "./my_named_pipe";

typedef int fifo_handle_t;

fifo_handle_t CreateFifo(const char* name, size_t access)
{
    if (mkfifo(name, 0666) == -1)
    {
        return INVALID_HANDLE_VALUE;
    }

    return open(name, access);
}

fifo_handle_t OpenFifo(const char* name, size_t access)
{
    return open(name, access);
}

bool ReadFromFifo(fifo_handle_t handle, std::string& result)
{
    std::vector<char> buffer(result.capacity());
    ssize_t bytes = read(handle, buffer.data(), buffer.size());
    if (bytes > 0)
    {
        result.append(buffer.data(), buffer.data() + bytes);
    }
    return bytes > 0;
}

bool WriteToFifo(fifo_handle_t handle, const std::string& result)
{
    size_t totalWritten = 0;
    ssize_t bytesWritten;
    while (totalWritten < result.size())
    {
        bytesWritten = write(handle, result.data() + totalWritten, result.size() - totalWritten);
        if (bytesWritten > 0)
        {
            totalWritten += bytesWritten;
        }
        else
        {
            return false;
        }
    }
    return true;
}

void CloseFifo(fifo_handle_t handle)
{
    close(handle);
}

void RemoveFifo(const char* name)
{
    unlink(name);
}

#endif // _WIN32

#endif // NAMEDPIPE_H
