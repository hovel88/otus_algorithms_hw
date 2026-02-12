#include "algo_eratosthenes.h"

int main(int argc, char** argv)
{
    std::cout << "программа поиска количества простых чисел до указанного числа N" << std::endl;
    std::cout << "алгоритм решета Эрастофена. сложность O(N log log N)" << std::endl;
    std::string N_str;
    std::cout << "число: "; std::getline(std::cin, N_str);

    try {
        uint64_t N = std::stoull(N_str);
        if (N < 0) {
            std::cout << "значение N должно быть положительным целым числом: введено " << N << std::endl;
        } else {
            uint64_t rv = primes_algo_eratosthenes(N);
            std::cout << "количество простых чисел: " << rv << std::endl;
        }
    } catch (const std::invalid_argument& e) {
        std::cerr << "неверный аргумент: " << e.what() << std::endl;
    } catch (const std::out_of_range& e) {
        std::cerr << "значение вне диапазона: " << e.what() << std::endl;
    }

    return 0;
}
