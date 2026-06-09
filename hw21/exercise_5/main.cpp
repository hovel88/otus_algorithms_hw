#include <iostream>

#include "algo.h"


int main() {
    std::cout << "Программа для демонстрации алгоритма решения задачи 'Маленький сарай' (метод перебора), сложность O(N^4)\n\n";

    std::string res;
    try {
        std::string N_M_str;
        std::getline(std::cin, N_M_str); // строка вида "4 3"
        auto space = N_M_str.find(' ');
        auto N_str = N_M_str.substr(0, space);
        auto M_str = N_M_str.substr(space + 1);

        int M = std::stoi(M_str);

        std::vector<std::string> data;          // строки вида
        data.resize(M);                         // "0 0 0 0"
        for (int i = 0; i < M; i++) {           // "0 1 0 0"
            std::getline(std::cin, data[i]);    // "0 0 0 1"
        }

        res = algo(N_str, M_str, data);
    } catch (const std::exception& e) {
        res = e.what();
    }

    std::cout << "\nМаксимальная площадь: " << res << std::endl;
    return 0;
}
