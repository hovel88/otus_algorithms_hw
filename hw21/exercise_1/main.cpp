#include <iostream>

#include "algo.h"


int main() {
    std::cout << "Программа для демонстрации алгоритма решения задачи 'Раз горох, два горох'\n\n";

    std::string res;
    try {
        std::string input;
        std::getline(std::cin, input); // строка вида "2/100+3/100"

        res = algo(input);
    } catch (const std::exception& e) {
        res = e.what();
    }

    std::cout << res << std::endl;;
    return 0;
}
