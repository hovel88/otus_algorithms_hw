#include "algo_base.h"

static bool _is_prime(uint64_t n)
{
    if (n < 2) return false;
    for (uint64_t d = 2; d < n; ++d) {
        if (n % d == 0) return false;
    }
    return true;
}

uint64_t primes_algo_base(uint64_t N)
{
    if (N > 100000) return 0; // ограничиваем, чтобы не состариться
    uint64_t count = 0;
    for (uint64_t n = 2; n <= N; ++n) {
        if (_is_prime(n)) ++count;
    }
    return count;
}
