#include <iostream>

#include "namedpipe.h"

/**
 * This program presents the named pipe, also known as
 * FIFO. Pipe has a specific name in the filesystem,
 * so we can use it as a regular file (by writing and
 * reading) to exchange data between processes.
 *
 * This file is a source code for client side.
 */

int main()
{
	std::cout << "Opening Fifo " << PIPE_NAME << "..." << std::endl;
	fifo_handle_t fifo = OpenFifo(PIPE_NAME, O_WRONLY);
	if (fifo == INVALID_HANDLE_VALUE)
	{
		std::cerr << "Could not open fifo " << PIPE_NAME << "!" << std::endl;
		return 1;
	}

	std::cout << "Start sending data..." << std::endl;
	std::string string;
	while (true)
	{
		std::cout << "> ";
		std::cin >> string;
		if (std::cin.eof())
		{
			break;
		}

		if (WriteToFifo(fifo, string))
		{
			std::cout << "Data written..." << std::endl;
		}
		else
		{
			std::cerr << "Pipe is closed or an error occured!" << std::endl;
			break;
		}
	}

	CloseFifo(fifo);

	return 0;
}