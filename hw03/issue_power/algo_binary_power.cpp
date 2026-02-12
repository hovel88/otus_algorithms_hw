#include "algo_binary_power.h"

double power_algo_binary_power(double A, int64_t N)
{
    if (N == 0) return 1.0;
    uint64_t mask = 0x8000000000000000;
    while (((uint32_t)N & mask) == 0) {
        mask = mask >> 1;
    }
    double res = 1.0;
    while (mask > 0) {
        if (((uint32_t)N & mask) > 0) {
            // сначала удваиваем степень (квадрат) а потом добавляем текущую степень двойки
            res = res * res * A;
        } else {
            res = res * res;
        }
        mask = mask >> 1;
    }
    return res;
}
