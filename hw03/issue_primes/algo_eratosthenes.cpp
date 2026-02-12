#include <cmath>
#include <vector>
#include "algo_eratosthenes.h"

uint64_t primes_algo_eratosthenes(uint64_t N)
{
    if (N > 1000000000) return 0; // ограничиваем, чтобы не состариться
    std::vector<bool> is_prime(N + 1, true);
    is_prime[0] = is_prime[1] = false; // не простые
    if (N >= 2) {
        for (uint64_t p = 2; (p*p) <= N; ++p) {
            // если p - простое, то вычеркиваем все кратные p числа, начиная с p*p
            if (is_prime[p]) {
                for (uint64_t i = p * p; i <= N; i+=p) {
                    is_prime[i] = false;
                }
            }
        }
    }
    uint64_t count = 0;
    for (uint64_t i = 2; i <= N; ++i) {
        if (is_prime[i]) count++;
    }
    return count;
}
