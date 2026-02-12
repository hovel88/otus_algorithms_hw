#include "algo_binary_power.h"

int main(int argc, char** argv)
{
    std::cout << "программа возведения вещественного числа в целую степень (A^N)" << std::endl;
    std::cout << "алгоритм двоичного разложения показателя степени. сложность O(log N)" << std::endl;
    std::string A_str;
    std::string N_str;
    std::cout << "число  : "; std::getline(std::cin, A_str);
    std::cout << "степень: "; std::getline(std::cin, N_str);

    try {
        double  A = std::stod(A_str);
        int64_t N = std::stoll(N_str);
        if (N < 0) {
            std::cout << "значение N должно быть положительным целым числом: введено " << N << std::endl;
        } else {
            double rv = power_algo_binary_power(A, N);
            std::cout << "результат возведения в степень: " << rv << std::endl;
        }
    } catch (const std::invalid_argument& e) {
        std::cerr << "неверный аргумент: " << e.what() << std::endl;
    } catch (const std::out_of_range& e) {
        std::cerr << "значение вне диапазона: " << e.what() << std::endl;
    }

    return 0;
}
