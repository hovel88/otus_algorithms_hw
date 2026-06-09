#pragma once

#include <iostream>
#include <iterator>
#include <vector>
#include <string>
#include <sstream>
#include <stdexcept>

std::string algo(const std::string& N_str, const std::string& M_str, const std::string& T_str, const std::vector<std::string>& data) {
    int N = std::stoi(N_str);
    int M = std::stoi(M_str);
    int T = std::stoi(T_str);

    if ((N < 1 || N > 1000)
    ||  (M < 1 || M > 1000)) {
        throw std::runtime_error("ошибка: карта участка фермы NxM, где N и M должны быть в диапазоне [1..1000]");
    }

    if (T < 0 || T > 10000 || T > (N*M)) {
        int r = ((N*M) > 10000) ? 10000 : (N*M);
        throw std::runtime_error(std::string("ошибка: введено некорректное количество построек на карте, должно быть в диапазоне [0..") + std::to_string(r) + std::string("]"));
    }

    if (data.size() != static_cast<size_t>(T)) {
        throw std::runtime_error(std::string("ошибка: введено не подходящее число строк с постройками, должно быть ") + std::to_string(T));
    }

    std::vector<std::vector<int>> grid(M, std::vector<int>(N, 0));
    for (int t = 0; t < T; ++t) {
        std::stringstream ss(data.at(t));
        std::vector<int> nums( (std::istream_iterator<int>(ss)), std::istream_iterator<int>() );
        if (nums.size() != 2) {
            throw std::runtime_error(std::string("ошибка: неправильное количество элементов в строке ") + std::to_string(t) + std::string(", должно быть 2 (координаты X и Y)"));
        }
        int x = nums.at(0);
        int y = nums.at(1);
        if (x < 0 || x >= N) {
            throw std::runtime_error(std::string("ошибка: для постройки введена неправильная координата X (") + std::to_string(x) + std::string("), должно быть в диапазоне [0.." + std::to_string(N) + std::string(")")));
        }
        if (y < 0 || y >= M) {
            throw std::runtime_error(std::string("ошибка: для постройки введена неправильная координата Y (") + std::to_string(y) + std::string("), должно быть в диапазоне [0.." + std::to_string(M) + std::string(")")));
        }
        grid[y][x] = 1;
    }

    std::stringstream ss_out;

    int height   = M;
    int width    = N;
    int max_area = 0;

    // чтобы решение стало O(N^3) вместо O(N^4), нужно избавиться от одного из внутренних циклов.
    // в частности, будем на каждом проходе вычислять высоты, аккумулируя строку за строкой.
    // это станет этапом мемоизации для динамического программирования. позволяя переиспользовать
    // результаты и значительно сокращать количество расчетов
    std::vector<int> memo_heights(width, 0);
    for (int y = 0; y < height; ++y) {
        {
            for (int i = 0; i < width; ++i) {
                if (grid[y][i] == 1) {
                    memo_heights[i] = 0;
                } else {
                    memo_heights[i] += 1;
                }
            }
            ss_out << "H: " << memo_heights[0];
            for (size_t i = 1; i < memo_heights.size(); ++i) ss_out << " " << memo_heights[i];
            ss_out << "\n";
        }
        for (int x = 0; x < width; ++x) {
            int area = 0;
            {
                int limit_h = height;
                for (int i = 0; (x + i) < width; ++i) {
                    // если мы знаем, что при текущих ограничениях высоты области
                    // и при известном расстоянии до границы участка потенциальная
                    // площадь окажется меньше, чем уже вычисленная максимальная
                    // площадь участка на предыдущих шагах, то и продолжать нет смысла
                    if ((limit_h * (width - x)) < max_area) break;
                    int h = memo_heights[x + i];
                    if (h < limit_h) limit_h = h;
                    int square = limit_h * (i + 1);
                    if (square > area) area = square;
                }
            }
            if (area > max_area) max_area = area;
        }
    }

    std::cout << "\nКарта вычисленных 'высот' для каждой ячейки:\n" << ss_out.str();
    return std::to_string(max_area);
}
