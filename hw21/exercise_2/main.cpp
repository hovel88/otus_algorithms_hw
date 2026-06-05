#include <iostream>

#include "algo.h"


int main() {
    std::cout << "Программа для демонстрации алгоритма решения задачи 'Ёлочка программиста'\n\n";

    std::string res;
    try {
        std::string N_str;
        std::getline(std::cin, N_str); // строка вида "4"

        int N = std::stoi(N_str);
        std::vector<std::string> data;
        data.resize(N);
        for (int i = 0; i < N; i++) {
            std::getline(std::cin, data[i]);
        }

        res = algo(N_str, data);
    } catch (const std::exception& e) {
        res = e.what();
    }

    std::cout << res << std::endl;
    return 0;
}
