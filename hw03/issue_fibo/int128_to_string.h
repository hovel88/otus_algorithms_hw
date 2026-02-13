#pragma once

#include <iostream>
#include <string>
#include <algorithm>

static inline std::string int128_to_string(__int128_t n)
{
    if (n == 0) return "0";
    std::string s = "";
    bool neg = false;
    if (n < 0) {
        neg = true;
        n = -n;
    }
    while (n > 0) {
        s += (char)('0' + (n % 10));
        n /= 10;
    }
    if (neg) s += '-';
    std::reverse(s.begin(), s.end());
    return s;
}
