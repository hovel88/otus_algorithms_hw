#pragma once

#include <string>
#include <stdexcept>
#include <cstdlib>

static bool _even(int64_t x) {
    return (x & 1) == 0;
}
static bool _odd(int64_t x) {
    return (x & 1) == 1;
}

// НОД через бинарный алгоритм Евклида (алгоритм Штейна)
// можно было бы реализовать через обычный остаток от деления
// (обычный алгоритм Евклида) - это проще и сложность такая же,
// но в этом нет ничего от динамического программирования.
// а в бинарном алгоритме Евклида есть хотя бы некая функция
// зависимости результата текущего шага от результата прошлого
// с итеративным уменьшением объема входных данных (хотя и нет
// никакой мемоизации)
static int64_t _gcd(int64_t m, int64_t n) {
    if (m == n) return m;
    if (m == 0) return n;
    if (n == 0) return m;
    if (_even(m) && _even(n)) return 2 * _gcd(m/2, n/2);
    if (_even(m) &&  _odd(n)) return     _gcd(m/2, n);
    if ( _odd(m) && _even(n)) return     _gcd(m, n/2);
    // мы тут если и (m), и (n) - нечетные
    if (m > n)
        return _gcd((m - n) / 2, n);
    else
        return _gcd(m, (n - m) / 2);
}

std::string algo(const std::string& in) {
    // фиксированный формат, строка вида "2/100+3/100"
    auto slash1 = in.find('/');
    auto plus   = in.find('+');
    auto slash2 = in.find('/', plus);

    auto a_str = in.substr(0, slash1);
    auto b_str = in.substr(slash1 + 1, plus - slash1 - 1);
    auto c_str = in.substr(plus + 1, slash2 - plus - 1);
    auto d_str = in.substr(slash2 + 1);

    int64_t a = std::stoll(a_str);
    int64_t b = std::stoll(b_str);
    int64_t c = std::stoll(c_str);
    int64_t d = std::stoll(d_str);

    if ((a < 1 || a > 10000)
    ||  (b < 1 || b > 10000)
    ||  (c < 1 || c > 10000)
    ||  (d < 1 || d > 10000)) {
        throw std::runtime_error("ошибка: числа вне допустимого диапазона [1..10000]");
    }

    if (((1.0 * a / b) > 1.0)
    ||  ((1.0 * c / d) > 1.0)) {
        throw std::runtime_error("ошибка: дроби должны быть меньше 1");
    }

    // a/b + c/d => (a*d + c*b)/(b*d)
    int64_t M = a * d + c * b;
    int64_t N = b * d;

    if ((1.0 * M / N) > 1.0) {
        throw std::runtime_error("ошибка: сумма дробей должны быть не больше 1");
    }

    // ищем НОД, если дробь уже несократимая, то НОД == 1.
    // ничего страшного если будем делить на 1 без проверки условия
    int64_t G = _gcd(M, N);
    M /= G;
    N /= G;

    return std::to_string(M) + std::string("/") + std::to_string(N);
}
