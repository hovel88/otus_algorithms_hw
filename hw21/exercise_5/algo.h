#pragma once

#include <iterator>
#include <vector>
#include <string>
#include <sstream>
#include <stdexcept>

std::string algo(const std::string& N_str, const std::string& M_str, const std::vector<std::string>& data) {
    int N = std::stoi(N_str);
    int M = std::stoi(M_str);

    if ((N < 1 || N > 30)
    ||  (M < 1 || M > 30)) {
        throw std::runtime_error("ошибка: карта участка фермы NxM, где N и M должны быть в диапазоне [1..30]");
    }

    if (data.size() != static_cast<size_t>(M)) {
        throw std::runtime_error(std::string("ошибка: введено не подходящее число строк, должно быть ") + std::to_string(M));
    }

    std::vector<std::vector<int>> grid(M, std::vector<int>(N));
    for (int m = 0; m < M; ++m) {
        std::stringstream ss(data.at(m));
        std::vector<int> nums( (std::istream_iterator<int>(ss)), std::istream_iterator<int>() );
        if (nums.size() != static_cast<size_t>(N)) {
            throw std::runtime_error(std::string("ошибка: неправильное количество элементов в строке ") + std::to_string(m) + std::string(", должно быть ") + std::to_string(N));
        }
        grid[m] = std::move(nums);
    }

    int height   = M;
    int width    = N;
    int max_area = 0;
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            // последовательно применяем алгоритм к каждому элементу массива.
            // для элементов каждой строчки вычисляем максимальную возможную высоту
            // сарая (количество свободных клеток над рассматриваемой (x;y), т.е. пока
            // не достигнем границы участка или не встретим препятствие).
            // в параллель отслеживаем самую минимальную высоту, ведь нам надо
            // вычислить максимально возможную площадь сарая, т.е. ограниченную
            // всякими препятствиями. для этого вводим параметр "limit_h", который
            // после прохода очередной строки станет минимальной найденной высотой
            // для элементов пройденной строки.
            // также сразу считаем площадь, ограниченную по высоте - найденным limit_h,
            // а по ширине - количеством проверенных ячеек. при этом отслеживаем
            // максимальное значение этой площади.
            // такой алгоритм дает сложность O(N^2 * M^2) ~ O(N^4)
            int area = 0;
            {
                int limit_h = height;
                for (int i = 0; (x + i) < width; ++i) {
                    int h = 0;
                    {
                        for (int j = y; j >= 0; --j) {
                            if (grid[j][x + i] == 0)
                                h++;
                            else
                                break;
                        }
                    }
                    if (h < limit_h) limit_h = h;
                    int square = limit_h * (i + 1);
                    if (square > area) area = square;
                }
            }
            if (area > max_area) max_area = area;
        }
    }

    return std::to_string(max_area);
}
