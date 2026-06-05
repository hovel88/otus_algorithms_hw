#include <iostream>

#include "algo.h"


int main() {
    std::cout << "Программа для демонстрации алгоритма решения задачи 'Пятью восемь'\n\n";

    std::string res;
    try {
        std::string N_str;
        std::getline(std::cin, N_str); // строка вида "4"

        res = algo(N_str);
    } catch (const std::exception& e) {
        res = e.what();
    }

    std::cout << res << std::endl;
    return 0;
}
