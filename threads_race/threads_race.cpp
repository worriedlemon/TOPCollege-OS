#include <iostream>
#include <thread>
#include <mutex>

/*
* Program which demonstrates race condition of two threads
* executing same function with different argument
*/

std::mutex mtx;

// Procedure which is executed in different threads (shared 'cout' variable)
// In this case we do not use mutex
void PrintFunction()
{
	for (int i = 0; i < 10; i++)
    {
        // These lines will be written out randomly
		std::cout << "Thread #" << std::this_thread::get_id() << ", iteration " << i << std::endl;
    }
}

// And in this case we do use mutex
void PrintFunctionSync()
{
	for (int i = 0; i < 10; i++)
    {
        // Locking
        mtx.lock();

        // These lines will be written out one by one
		std::cout << "Thread #" << std::this_thread::get_id() << ", iteration " << i << std::endl;

        // Unlocking
        mtx.unlock();
    }
}

int main(int argc, char** argv)
{
    void (*func)() = PrintFunction;
    if (argc >= 2 && argv[1][0] != '\0')
    {
        char opt = argv[1][1];
        if (opt == 's')
        {
            func = PrintFunctionSync;
        }
        else if (opt == 'h')
        {
            std::cout << "Usage:\nthreads_race [-h|-s]\n\n-s   - synchronize\n-h   - print this message" << std::endl;
            return 0;
        }
    }

	// Creating one more thread
	std::thread thr(func);
    
    // Printing in main thread
    func();

	// Waiting for thread to finish
	thr.join();

	return 0;
}
