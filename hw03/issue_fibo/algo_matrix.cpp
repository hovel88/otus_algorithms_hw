#include "algo_matrix.h"

#include <vector>

using Matrix = std::vector<std::vector<__int128_t>>;

static Matrix multiply(const Matrix &m1, const Matrix &m2)
{
    return {
        {m1[0][0] * m2[0][0] + m1[0][1] * m2[1][0], m1[0][0] * m2[0][1] + m1[0][1] * m2[1][1]},
        {m1[1][0] * m2[0][0] + m1[1][1] * m2[1][0], m1[1][0] * m2[0][1] + m1[1][1] * m2[1][1]}
    };
}

static Matrix power(Matrix m, uint64_t n)
{
    Matrix res = {{1, 0}, {0, 1}}; // единичная матрица
    while (n > 0) {
        // проверяем биты в разложенной по маске степени
        // и если выставлен 1, то нужно дополнительно домножить
        // иначе - просто перемножить (возвести в квадрат)
        if (n & 1) {
            res = multiply(res, m);
        }
        m = multiply(m, m);
        n >>= 1;
    }
    return res;
}

__int128_t fibo_algo_matrix(uint64_t N)
{
    if (N > 100) return 0; // ограничиваем, чтобы не состариться
    if (N <= 1) return N;

    // тождество
    //                  n  
    //          | 1  1 |  = | F(n+1)  F(n)   |
    //          | 1  0 |    | F(n)    F(n-1) |
    // если
    //            | 1  1 |
    //      MAT = |      |
    //            | 1  0 |
    // тогда
    //      F(n) = (MAT^(n))   элемент с координатами 0,1
    //  или F(n) = (MAT^(n-1)) элемент с координатами 0,0
    Matrix mat = {{1, 1}, {1, 0}};
    Matrix res = power(mat, N - 1);
    return res[0][0];
}
