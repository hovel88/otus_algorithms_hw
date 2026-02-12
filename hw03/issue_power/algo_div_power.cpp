#include "algo_div_power.h"

double power_algo_div_power(double A, int64_t N)
{
    if (N == 0) return 1.0;
    if ((N % 2) == 0) {
        // четная степень
        double res = power_algo_div_power(A, (N / 2));
        return res * res;
    } else {
        // нечетная степень
        double res = power_algo_div_power(A, (N / 2));
        return res * res * A; // с домножением
    }
}
