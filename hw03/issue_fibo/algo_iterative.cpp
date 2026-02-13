#include "algo_iterative.h"

__int128_t fibo_algo_iterative(uint64_t N)
{
    if (N > 100) return 0; // ограничиваем, чтобы не состариться
    if (N <= 1) return N; // F(0)=0, F(1)=1
    __int128_t prev2 = 0; // инициализируем значением F(0)
    __int128_t prev1 = 1; // инициализируем значением F(1)
    __int128_t cur = 0;
    for (uint64_t i = 2; i <= N; ++i) {
        cur = prev1 + prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return cur;
}
