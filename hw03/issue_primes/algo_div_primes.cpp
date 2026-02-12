#include <cmath>
#include <vector>
#include "algo_div_primes.h"

uint64_t primes_algo_div_primes(uint64_t N)
{
    if (N > 100000000) return 0; // ограничиваем, чтобы не состариться
    std::vector<uint64_t> primes;
    if (N >= 2) {
        primes.push_back(2); // первое простое число
        // далее проверяем только нечетные
        for (uint64_t d = 3; d <= N; d+=2) {
            bool is_prime = true;
            // ограничиваем поиск до квадратного корня,
            // и проверяем деление только на найденные простые числа
            // (которые уже известно, что делятся ТОЛЬКО на себя и на 1)
            uint64_t limit = std::sqrt(d);
            for (uint64_t p : primes) {
                if (p > limit) break;
                if (d % p == 0) {
                    is_prime = false;
                    break;
                }
            }
            if (is_prime) {
                primes.push_back(d);
            }
        }
    }
    return primes.size();
}
