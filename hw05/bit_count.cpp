#include "bit_count.h"

uint32_t popcount_v1(uint64_t num)
{
    uint32_t count = 0;
    uint64_t mask = 0x01;
    while (mask) {
        if (num & mask) ++count;
        mask <<= 1;
    }
    return count;
}

// ----------------------------------------------------------------------------

uint32_t popcount_v2(uint64_t num)
{
    uint32_t count = 0;
    while (num) {
        ++count;
        num &= (num - 1);
    }
    return count;
}

// ----------------------------------------------------------------------------

static inline uint32_t _popcount_cached_by_bytes(uint64_t num)
{
    static bool first_run = false;
    static uint8_t byte_ones[256] = {0};
    if (!first_run) {
        first_run = true;
        for (int i = 0; i < 256; ++i) {
            byte_ones[i] = static_cast<uint8_t>(popcount_v2(i));
        }
    }
    return byte_ones[num];
}

uint32_t popcount_v3(uint64_t num)
{
    return (_popcount_cached_by_bytes((num >> 0 ) & 0xff)
        +   _popcount_cached_by_bytes((num >> 8 ) & 0xff)
        +   _popcount_cached_by_bytes((num >> 16) & 0xff)
        +   _popcount_cached_by_bytes((num >> 24) & 0xff)
        +   _popcount_cached_by_bytes((num >> 32) & 0xff)
        +   _popcount_cached_by_bytes((num >> 40) & 0xff)
        +   _popcount_cached_by_bytes((num >> 48) & 0xff)
        +   _popcount_cached_by_bytes((num >> 56) & 0xff));
}

// ----------------------------------------------------------------------------

static inline uint32_t _popcount_cached_by_nibbles(uint64_t num)
{
    static bool first_run = false;
    static uint8_t nibble_ones[16] = {0};
    if (!first_run) {
        first_run = true;
        for (int i = 0; i < 16; ++i) {
            nibble_ones[i] = static_cast<uint8_t>(popcount_v2(i));
        }
    }
    return nibble_ones[(num >> 0) & 0x0f] + nibble_ones[(num >> 4) & 0x0f];
}

uint32_t popcount_v4(uint64_t num)
{
    return (_popcount_cached_by_bytes((num >> 0 ) & 0xff)
        +   _popcount_cached_by_bytes((num >> 8 ) & 0xff)
        +   _popcount_cached_by_bytes((num >> 16) & 0xff)
        +   _popcount_cached_by_bytes((num >> 24) & 0xff)
        +   _popcount_cached_by_bytes((num >> 32) & 0xff)
        +   _popcount_cached_by_bytes((num >> 40) & 0xff)
        +   _popcount_cached_by_bytes((num >> 48) & 0xff)
        +   _popcount_cached_by_bytes((num >> 56) & 0xff));
}
