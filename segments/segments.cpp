#include <iostream>
#include <cstdint>

// How many values fit in 2 bytes
constexpr uint32_t valuesInTwoBytes = (1 << 16);

// Function for taking first 20 bits
constexpr uintptr_t Mod20(uintptr_t F)
{
    return F % (1 << 20);
}

int main()
{
    // Random address
    constexpr uintptr_t initAddr = Mod20(0x1FA0DBC);

    // Counter
    size_t cnt = 0;

    // Taking every segment possible
    for (uint32_t seg = 0; seg < valuesInTwoBytes; seg++)
    {
        // Taking every offset possible
        for (uint32_t off = 0; off < valuesInTwoBytes; off++)
        {
            // Comparing obtained address with original
            uintptr_t addr = Mod20((seg << 4) + off);
            if (addr == initAddr)
            {
                cnt++;
            }
        }
    }

    std::cout << "Segmentation. Address 0x" << std::hex << std::uppercase << initAddr
              << " can be represented in " << std::dec << cnt << " ways" << std::endl;

    return 0;
}
