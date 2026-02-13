#include "algo_matrix.h"

int main(int argc, char** argv)
{
    std::cout << "программа поиска чисел Фибоначчи по его номеру" << std::endl;
    std::cout << "алгоритм перемножения матриц. сложность O(log N)" << std::endl;
    std::string N_str;
    std::cout << "номер: "; std::getline(std::cin, N_str);

    try {
        uint64_t N = std::stoull(N_str);
        if (N < 0) {
            std::cout << "значение N должно быть положительным целым числом: введено " << N << std::endl;
        } else {
            auto rv = fibo_algo_matrix(N);
            auto rv_str = int128_to_string(rv);
            std::cout << "число Фибоначчи: " << rv_str << std::endl;
        }
    } catch (const std::invalid_argument& e) {
        std::cerr << "неверный аргумент: " << e.what() << std::endl;
    } catch (const std::out_of_range& e) {
        std::cerr << "значение вне диапазона: " << e.what() << std::endl;
    }

    return 0;
}
