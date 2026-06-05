#include <iostream>

#include "algo.h"


int main() {
    std::cout << "Программа для демонстрации алгоритма решения задачи 'Большой остров'\n\n";

    std::string res;
    try {
        std::string N_str;
        std::getline(std::cin, N_str); // строка вида "4"

        int N = std::stoi(N_str);
        std::vector<std::string> data;          // строки вида
        data.resize(N);                         // "1 1 1 1"
        for (int i = 0; i < N; i++) {           // "0 1 0 1"
            std::getline(std::cin, data[i]);    // "0 0 0 0"
        }                                       // "1 0 1 1"

        res = algo(N_str, data);
    } catch (const std::exception& e) {
        res = e.what();
    }

    std::cout << res << std::endl;
    return 0;
}
