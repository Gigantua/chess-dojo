#pragma once
#include <iostream>
#include <bit>
#include <unordered_set>
#include <chrono>
#include "../Core/Robin_hood.hpp"


#define Bitloop(X) while (X) { const uint64_t pawn = PopBit(X);
static uint64_t PopBit(uint64_t& val)
{
    uint64_t lsb = val & -val;
    val ^= lsb;
    return lsb;
}

static constexpr uint64_t File1 = 0b1000000010000000100000001000000010000000100000001000000010000000ul;
static constexpr uint64_t File8 = 0b0000000100000001000000010000000100000001000000010000000100000001ul;
static constexpr uint64_t Rank7 = 0b0000000011111111000000000000000000000000000000000000000000000000ul;
static constexpr uint64_t Rank2 = 0b0000000000000000000000000000000000000000000000001111111100000000ul;
static constexpr uint64_t Rank_18 = 0xFF000000000000FF;

robin_hood::unordered_set<uint64_t> patterns;


bool doPrint = false;
auto start_time = std::chrono::high_resolution_clock::now();

static std::string _map(uint64_t value)
{
    static std::string str(64 + 8, 'o');
    for (uint64_t i = 0, c = 0; i < 64; i++)
    {
        uint64_t bitmask = (1ull << 63) >> i;

        if ((bitmask & value) != 0) str[c++] = 'X';
        else str[c++] = '.';

        if ((i + 1) % 8 == 0) str[c++] = '\n';
    }
    return str;
}


template<bool color>
static void perftPawns(uint64_t currentPattern)
{
    currentPattern &= ~Rank_18;

    if (!patterns.insert(currentPattern).second)
        return;

    if (doPrint)
    {
        constexpr int block = (1ull << 18);
        if (patterns.size() % block == 0)
        {
            auto end_time = std::chrono::high_resolution_clock::now();
            auto time = end_time - start_time;
            float perf = block * 1.0 / std::chrono::duration_cast<std::chrono::microseconds>(time).count();

            std::cout << patterns.size() << "\n";
            std::cout << perf << " M/s" << "\n";
            std::cout << _map(currentPattern) << "\n";

            start_time = end_time;
        }
    }

    if constexpr (color)
    {
        uint64_t bitloop = currentPattern & ~Rank7 & ~currentPattern >> 8;
        Bitloop(bitloop)
            uint64_t forward = currentPattern ^ (pawn | (pawn << 8));
        perftPawns<color>(forward);
    }
    bitloop = currentPattern;
    Bitloop(bitloop)
        uint64_t taken = currentPattern & ~pawn;
    perftPawns<color>(taken);
}

bitloop = currentPattern & ~Rank7 & ~File8 & ~currentPattern >> 7;
Bitloop(bitloop)
uint64_t right = currentPattern ^ (pawn | (pawn << 7));
perftPawns<color>(right);
}
bitloop = currentPattern & ~Rank7 & ~File1 & ~currentPattern >> 9;
Bitloop(bitloop)
uint64_t left = currentPattern ^ (pawn | (pawn << 9));
perftPawns<color>(left);
        }
    }
    else
    {
    uint64_t bitloop = currentPattern & ~Rank2 & ~currentPattern << 8;
    Bitloop(bitloop)
        uint64_t forward = currentPattern ^ (pawn | (pawn >> 8));
    perftPawns<color>(forward);
        }
        bitloop = currentPattern;
        Bitloop(bitloop)
            uint64_t taken = currentPattern & ~pawn;
        perftPawns<color>(taken);
        }

        bitloop = currentPattern & ~Rank2 & ~File8 & ~currentPattern << 9;
        Bitloop(bitloop)
            uint64_t right = currentPattern ^ (pawn | (pawn >> 9));
        perftPawns<color>(right);
        }
        bitloop = currentPattern & ~Rank2 & ~File1 & ~currentPattern << 7;
        Bitloop(bitloop)
            uint64_t left = currentPattern ^ (pawn | (pawn >> 7));
        perftPawns<color>(left);
        }

    }
}

int main2()
{
    doPrint = true;
    perftPawns<false>(0b11111111ull << 48ull);
    perftPawns<true>(0b11111111ull << 8ull);
    std::cout << "Total Patterns: " << patterns.size() << "\n";
}