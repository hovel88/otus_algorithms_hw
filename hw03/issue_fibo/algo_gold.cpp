#include <cmath>
#include "algo_gold.h"

__int128_t fibo_algo_gold(uint64_t N)
{
    if (N > 100) return 0; // ограничиваем, чтобы не состариться
    // if (N <= 1) return N; // F(0)=0, F(1)=1
    // формула Бине
    //              n        n
    //           PHI   -  PSI
    //      Fn = -------------
    //              sqrt(5)
    // где
    //            1 + sqrt(5)
    //      PHI = -----------       PSI = 1 - PHI
    //                2
    double phi = (1 + sqrt(5)) / 2;
    double psi = 1 - phi;
    auto Fn = (std::pow(phi, N) - std::pow(psi, N)) / sqrt(5);
    return static_cast<__int128_t>(std::round(Fn));
}
