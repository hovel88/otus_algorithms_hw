#include "algo_recursive.h"

__int128_t fibo_algo_recursive(uint64_t N)
{
    if (N > 50) return 0; // ограничиваем, чтобы не состариться
    if (N <= 1) return N; // F(0)=0, F(1)=1
    return fibo_algo_recursive(N - 1) + fibo_algo_recursive(N - 2);
}
