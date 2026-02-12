#include <cmath>
#include "algo_sqrt_div_2_3.h"

static bool _is_prime(uint64_t n)
{
    if (n <= 1) return false;
    if (n <= 3) return true;
    if ((n % 2) == 0
    ||  (n % 3) == 0) return false;
    for (uint64_t d = 5; (d * d) <= n; d+=6) {
        if (n % d == 0
        ||  n % (d + 2) == 0) return false;
    }
    return true;
}

uint64_t primes_algo_sqrt_div_2_3(uint64_t N)
{
    if (N > 10000000) return 0; // ограничиваем, чтобы не состариться
    uint64_t count = 0;
    for (uint64_t n = 2; n <= N; ++n) {
        if (_is_prime(n)) ++count;
    }
    return count;
}
