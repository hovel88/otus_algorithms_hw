#include "algo_div_primes.h"

int main(int argc, char** argv)
{
    std::cout << "программа поиска количества простых чисел до указанного числа N" << std::endl;
    std::cout << "алгоритм перебора делителей (с оптимизацией поиска c делением на простые числа). сложность O(N sqrt(N) / log(N))" << std::endl;
    std::string N_str;
    std::cout << "число: "; std::getline(std::cin, N_str);

    try {
        uint64_t N = std::stoull(N_str);
        if (N < 0) {
            std::cout << "значение N должно быть положительным целым числом: введено " << N << std::endl;
        } else {
            uint64_t rv = primes_algo_div_primes(N);
            std::cout << "количество простых чисел: " << rv << std::endl;
        }
    } catch (const std::invalid_argument& e) {
        std::cerr << "неверный аргумент: " << e.what() << std::endl;
    } catch (const std::out_of_range& e) {
        std::cerr << "значение вне диапазона: " << e.what() << std::endl;
    }

    return 0;
}
