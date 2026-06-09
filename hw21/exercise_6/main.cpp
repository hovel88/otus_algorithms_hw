#include <iostream>

#include "algo.h"


int main() {
    std::cout << "Программа для демонстрации алгоритма решения задачи 'Большой сарай' (высота сарая, ДП), сложность O(N^3)\n\n";

    std::string res;
    try {
        std::string N_M_str;
        std::getline(std::cin, N_M_str); // строка вида "4 3"
        auto space = N_M_str.find(' ');
        auto N_str = N_M_str.substr(0, space);
        auto M_str = N_M_str.substr(space + 1);

        std::string T_str;
        std::getline(std::cin, T_str); // строка вида "2"

        int T = std::stoi(T_str);

        std::vector<std::string> data;
        data.resize(T);                         // строки вида
        for (int i = 0; i < T; i++) {           // "1 1"
            std::getline(std::cin, data[i]);    // "0 2"
        }

        res = algo(N_str, M_str, T_str, data);
    } catch (const std::exception& e) {
        res = e.what();
    }

    std::cout << "\nМаксимальная площадь: " << res << std::endl;
    return 0;
}
