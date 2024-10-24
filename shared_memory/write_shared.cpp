#include <iostream>
#include <string>
#include <cstdlib>
#include <cstdint>
#include <cstring>

#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

/**
 * This program demonstrates the use of POSIX Shared Memory
 * mechanism. This file write data to shared memory block.
 */

constexpr int dataSize = 4096;

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cerr << "Not enough arguments!\nUsage: write_shared <path>" << std::endl;
    }

    const char* name = argv[1];

    // Creating new or opening existing shared memory object
    int shmfd = shm_open(name, O_CREAT | O_RDWR, 0666);
    if (shmfd == -1)
    {
        std::cerr << "Open shared memory at " << name << " failed: " << std::strerror(errno) << std::endl;
        return 1;
    }

    // Changing shared memory size by truncating it
    if (ftruncate(shmfd, dataSize) == -1)
    {
        close(shmfd);
        std::cerr << "Could not resize shared memory: " << std::strerror(errno) << std::endl;
        return 1;
    }

    // Mapping shared memory into process virtual address space (write-only)
    void* ptr = mmap(0, dataSize, PROT_WRITE, MAP_SHARED, shmfd, 0);
    if (ptr == MAP_FAILED)
    {
        close(shmfd);
        std::cerr << "Memory mapping failed: " << std::strerror(errno) << std::endl;
        return 1;
    }

    std::cout << "Successfully opened/created shared memory " << name << ".\n"
                 "It is ready to store data:" << std::endl;

    // Writing data
    std::string line;
    auto* address = static_cast<uint8_t*>(ptr);
    while (std::getline(std::cin, line))
    {
        std::copy(line.begin(), line.end(), address);
        address += line.size();
    }

    close(shmfd);
    
    std::cout << "Done!" << std::endl;

    return 0;
}
