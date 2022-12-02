
#include "cuda_runtime.h"
#include "device_launch_parameters.h"

#include <stdio.h>
#include <iostream>
#include <fstream>
#include "../Core/math.h"
#include "memmap.h"

using namespace std;

const char* bit_filename = "all_bits.dat";

void ensure_bits_file(int bits)
{
    FILE* file;
    file = fopen(bit_filename, "r");
    if (file) {
        fclose(file);
        return;
    }

    file = fopen(bit_filename, "wb");
    uint64_t buf[4096];
    uint64_t* ptr = buf;
    const uint64_t* end = buf + 4096;

    uint64_t total = 0;
    for (int bits = 0; bits <= 8; bits++) {
        total += ncr(64, bits);
    }
    std::cout << 8ull * total << " Bytes\n";

    for (int bits = 0; bits <= 8; bits++)
    {
        Timer t;
        std::cout << "Writing: " << bits << "...";
        uint64_t count = ncr(64, bits);
        uint64_t value = (1ull << bits) - 1;

        for (uint64_t i = 0; i < count; i++)
        {
            *ptr++ = value;
            if (ptr == end) {
                fwrite(buf, sizeof(uint64_t), ptr - buf, file);
                ptr = buf;
            }

            bit_twiddle_permute(value);
        }
        fwrite(buf, sizeof(uint64_t), ptr - buf, file);
        ptr = buf;
        std::cout << t.ms() << "ms\n";
    }
    fclose(file);
    std::cout << "OK";
}


int main()
{
    ensure_bits_file(8);
    memmap m(bit_filename);

}
