#include <iostream>
#include <cstring>

#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

/**
 * This program demonstrates the use of POSIX Shared Memory
 * mechanism. This file removes shared memory object.
 */

constexpr int dataSize = 4096;

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cerr << "Not enough arguments!\nUsage: unlink_shared <path>" << std::endl;
    }

    const char* name = argv[1];

    // Removing shared memory object
    int rv = shm_unlink(name);
    if (rv == -1)
    {
        std::cerr << "Could not remove shared memory " << name << ": " << std::strerror(errno) << std::endl;
        return 1;
    }

    std::cout << "Removed shared memory " << name << std::endl;
    return 0;
}

