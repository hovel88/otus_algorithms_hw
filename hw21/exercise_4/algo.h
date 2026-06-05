#pragma once

#include <iterator>
#include <queue>
#include <vector>
#include <string>
#include <sstream>
#include <limits>
#include <algorithm>
#include <stdexcept>
#include <cstdlib>

static void _bfs(std::vector<std::vector<int>>& grid, int start_i, int start_j) {
    const int N = grid.size();

    // ходим в четырех направлениях, т.е. делаем дельту по i и по j на одну клетку
    // вправо, влево, вниз, вверх
    const int di[] = {1, -1, 0, 0};
    const int dj[] = {0, 0, 1, -1};

    // вариант с поиском в ширину потребует вместо рекурсии использовать очередь
    std::queue<std::pair<int,int>> q;

    // "погружаем под воду" точку суши на карте
    // и запоминаем в очереди, чтобы начать искать от неё
    grid[start_i][start_j] = 0;
    q.push({start_i, start_j});

    while (!q.empty()) {
        auto [i, j] = q.front();
        q.pop();

        // ходим в 4 направлениях: вправо, влево, вниз, вверх
        for (int d = 0; d < 4; d++) {
            int ni = i + di[d];
            int nj = j + dj[d];

            if ((ni >= 0 && ni < N)
            &&  (nj >= 0 && nj < N)
            &&  (grid[ni][nj] != 0)) {
                // "погружаем под воду" точку суши на карте
                // и запоминаем её, чтобы потом попробовать поискать от неё
                grid[ni][nj] = 0;
                q.push({ni, nj});
            }
        }
    }
}

std::string algo(const std::string& N_str, const std::vector<std::string>& data) {
    int N = std::stoi(N_str);

    if (N < 1 || N > 100) {
        throw std::runtime_error("ошибка: карта островов NxN, где N должно быть в диапазоне [1..100]");
    }

    if (data.size() != static_cast<size_t>(N)) {
        throw std::runtime_error(std::string("ошибка: введено не подходящее число строк, должно быть ") + std::to_string(N));
    }

    std::vector<std::vector<int>> grid(N, std::vector<int>(N));
    for (int n = 0; n < N; ++n) {
        std::stringstream ss(data.at(n));
        std::vector<int> nums( (std::istream_iterator<int>(ss)), std::istream_iterator<int>() );
        if (nums.size() != static_cast<size_t>(N)) {
            throw std::runtime_error(std::string("ошибка: неправильное количество чисел в строке ") + std::to_string(n) + std::string(", должно быть ") + std::to_string(N));
        }
        grid[n] = std::move(nums);
    }

    // даже не будем создавать дополнительную матрицу с флагами посещенных координат карты.
    // будем работать с исходной таблицей и просто считать количество островов
    int islands = 0;

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            if (grid[i][j] != 0) {
                islands++;
                _bfs(grid, i, j);
            }
        }
    }

    return std::to_string(islands);
}
