#include <iostream>

#include "allocate_address.h"


/*
* This program converts input string into address. Then this address
* is being dereferenced (or allocated first), user writes random data
* and program tries to print the value stored at this address.
*/


int main()
{
	// Example of an address
	int a = 42;
	std::cout << "Example of an address in stack: " << &a << "\n";
    int *b = new int{42};
    std::cout << "Example of an address in heap: " << b << "\n";
    delete b;

	std::cout << "\nEnter your desired address:\n";
	
	// String, which contains address
	std::string address_str;
	std::cin >> address_str;

	// Reinterpreting obtained integer as some address in memory
	int* address_of_smth = reinterpret_cast<int*>(GetAddressFromStr(address_str));
	if (address_of_smth == nullptr)
	{
        std::cerr << "Value given does not represent a valid address!" << std::endl;
		return -1;
	}

	std::cout << "Given address is " << address_of_smth << "\n";

	// Is is possible for us to get unreadable address
    address_of_smth = AllocateAddress(address_of_smth, sizeof(int));

    // Operating system allocates data in pages, so the address, probably, is different
    std::cout << "Real address is " << address_of_smth << "\n";
    if (address_of_smth == nullptr)
    {
        std::cerr << "Address is inaccessible" << std::endl;
        return -1;
    }

	// Enter some value
	std::cout << "Enter value to be stored:\n";
	std::cin >> *address_of_smth;

	// Check for stored value in cycle
	char cont;
	do
	{
		std::cout << "Value stored is " << *address_of_smth << "\n";
		std::cout << "Read again? [Y/N]\n";
		std::cin >> cont;
	}
	while (cont == 'Y' || cont == 'y');

	// Free virtual allocated memory if the pointer had no read access
    DeallocateAddress(address_of_smth, sizeof(int));

	return 0;
}
