#include <iostream>
#include <cstdlib>
#include <csignal>

/*
* One of the simpler IPC methods is to use signals. Processes
* can pass signals between each other. There are many types of
* signals in UNIX, although there are few signals in Windows,
* such as SIGINT, SIGTERM, SIGALRM, SIGABRT and some others.
* 
* This simple app redefines behavior of the signal SIGINT,
* which is passed when process get Ctrl+C input to interrupt
* its execution.
*
* In UNIX executable, signal SIGTERM also has redefined behavior.
* 
*/


// Creating custom SIGINT signal handler
void signal_handler(int signal)
{
#if defined(_WIN32) || defined(WIN32)
#define CONSOLE_QUIT false
    // On Windows signals are not fully supported
	if (signal == SIGINT)
	{
		// Write something to a console and exit
		std::cout << "\nNOOOO, I'm being interrupted!\nGoodbye then..." << std::endl;
        std::exit(1);
    }
#else
#define CONSOLE_QUIT std::cin.eof()
    // On UNIX signals are great
	if (signal == SIGINT)
	{
		// Write something to a console and continue
		std::cout << "\nNOOOO, I'm being interrupted!\nAnyway..." << std::endl;
	}
    else if (signal == SIGTERM)
    {
        // Write something to a console and exit
        std::cout << "\nNOOOO, Im'being terminated!\nGoodbye then..." << std::endl;
        std::exit(1);
    }
#endif // _WIN32
}

int main()
{
	std::cout << "I am process with redefined behavior of signal SIGINT!\n";
	// Override signal handling with custom function
	std::signal(SIGINT, signal_handler);
	std::signal(SIGTERM, signal_handler);

	char option[256];
	while (true)
	{
		// Some gibberish to demonstrate Ctrl+C event
		std::cin >> option;

		if ((option[1] == '\0' && option[0] == '0') || CONSOLE_QUIT)
		{
			break;
		}
	}

	return 0;
}
