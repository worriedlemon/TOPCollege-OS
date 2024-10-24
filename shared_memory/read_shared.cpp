#include <iostream>
#include <cstdlib>
#include <cstring>

#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

/**
 * This program demonstrates the use of POSIX Shared Memory
 * mechanism. This file read data from shared memory block.
 */

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cerr << "Not enough arguments!\nUsage: read_shared <path>" << std::endl;
    }

    const char* name = argv[1];

    // Opening shared memory object
    int shmfd = shm_open(name, O_RDONLY, 0666);
    if (shmfd == -1)
    {
        std::cerr << "Open shared memory at " << name << " failed: " << std::strerror(errno) << std::endl;
        return 1;
    }

    // Getting file attributes
    struct stat sb;
    if (fstat(shmfd, &sb) == -1)
    {
        close(shmfd);
        std::cerr << "Cannot get attributes: " << std::strerror(errno) << std::endl;
        return 1;
    }

    // Mapping shared memory into process virtual address space (read-only)
    void* ptr = mmap(0, sb.st_size, PROT_READ, MAP_SHARED, shmfd, 0);
    if (ptr == MAP_FAILED)
    {
        close(shmfd);
        std::cerr << "Memory mapping failed: " << std::strerror(errno) << std::endl;
        return 1;
    }

    // Representing stored data as string and printing it out
    std::string_view view(static_cast<char*>(ptr), sb.st_size);
    std::cout << "Data stored:\n" << view << std::endl;

    close(shmfd);

    return 0;
}

