#include <iostream>
#include <csignal>
#include <thread>

#include "namedpipe.h"

/**
 * This program presents the named pipe, also known as
 * FIFO. Pipe has a specific name in the filesystem,
 * so we can use it as a regular file (by writing and
 * reading) to exchange data between processes.
 * 
 * This file is a source code for server side.
 */

std::atomic_bool run = true;

int main()
{
	std::signal(SIGINT, [](int) { run = false; });

	std::cout << "Creating Fifo " << PIPE_NAME << "..." << std::endl;
	fifo_handle_t fifo = CreateFifo(PIPE_NAME, PIPE_RDONLY);
	if (fifo == INVALID_HANDLE_VALUE)
	{
		std::cerr << "Could not create fifo " << PIPE_NAME << "!" << std::endl;
		return 1;
	}

	std::cout << "Waiting for data from client..." << std::endl;

	constexpr size_t readCapacity = 256;
	std::string string;
    string.reserve(readCapacity);
	while (run)
	{
		string.clear();
		if (ReadFromFifo(fifo, string))
		{
			std::cout << "Data received: " << string << std::endl;
		}
		else
		{
			std::cerr << "\nError occurred during receive data!" << std::endl;
			run = false;
		}
	}

	CloseFifo(fifo);
	RemoveFifo(PIPE_NAME);

	return 0;
}
