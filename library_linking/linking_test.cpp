/*
* This program describes dynamic linking with project MyTestLibrary.
* Dynamic linking can be executed on load or in runtime.
* Runtime linking can be turned on by setting _runtime define to 1,
* load linking - setting _runtime to 0.
*/

#include <iostream>
#include <cstdlib>

#include "loadlib.h"

#ifdef _runtime
class MyLibLoader
{
public:
    MyLibLoader()
    {
        std::cout << "Program runs in dynamic linking runtime mode.\n\n";
        handle = LoadMyLibrary(module_name);
        if (handle == nullptr)
        {
		    std::cerr << "Could not load module " << module_name << std::endl;
            std::exit(1);
        }
	    std::cout << "Loaded module " << module_name << std::endl;
    }

    ~MyLibLoader()
    {
        if (handle != nullptr)
        {
            UnloadMyLibrary(handle);
        }
    }

    double CalculateRoot(double value) const
    {
        static sqrtfunc_t my_sqrt = GetSqrtFunction(handle);
        if (my_sqrt == nullptr)
        {
            std::cerr << "Could not find symbol 'my_sqrt'" << std::endl;
            return -1;
        }
        return my_sqrt(value);
    }

private:
    libhandle_t handle;
#if defined(_WIN32) || defined(WIN32)
    static constexpr const char* module_name = "mytestlib_shared.dll";
#else
    static constexpr const char* module_name = "libmytestlib_shared.so";
#endif // _WIN32
};

#else
struct MyLibLoader
{
    MyLibLoader()
    {
	    std::cout << "Program runs in dynamic linking startup mode.\n" << std::endl;
    }
    
    double CalculateRoot(double value) const
    {
        return ::my_sqrt(value);
    }
};
#endif // _runtime


int main()
{
	std::cout << "This program calculates square root of inputted value.\n";

    MyLibLoader ld;

	// Program itself, calculates square root
	double value;
	std::cout << "Enter your value:\n";
	std::cin >> value;
	
	double result = ld.CalculateRoot(value);
	std::cout << "Square root of " << value << " is ";

	if (result == -1)
	{
		std::cout << "undefined";
	}
	else
	{
		std::cout << result;
	}

	std::cout << std::endl;

	return 0;
}
