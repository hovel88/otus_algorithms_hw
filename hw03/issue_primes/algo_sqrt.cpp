#include <cmath>
#include "algo_sqrt.h"

static bool _is_prime(uint64_t n)
{
    if (n < 2) return false;
    uint64_t sqrt_n = std::sqrt(n);
    for (uint64_t d = 2; d <= sqrt_n; ++d) {
        if (n % d == 0) return false;
    }
    return true;
}

uint64_t primes_algo_sqrt(uint64_t N)
{
    if (N > 100000) return 0; // ограничиваем, чтобы не состариться
    uint64_t count = 0;
    for (uint64_t n = 2; n <= N; ++n) {
        if (_is_prime(n)) ++count;
    }
    return count;
}
