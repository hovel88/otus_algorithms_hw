#include "algo_iterative.h"

double power_algo_iterative(double A, int64_t N)
{
    if (N == 0) return 1.0;
    double res = 1.0;
    for (int64_t i = 0; i < N; ++i) {
        res *= A;
    }
    return res;
}
