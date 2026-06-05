#pragma once

#include <iterator>
#include <vector>
#include <string>
#include <sstream>
#include <limits>
#include <algorithm>
#include <stdexcept>
#include <cstdlib>

std::string algo(const std::string& N_str, const std::vector<std::string>& data) {
    int N = std::stoi(N_str);

    if (N < 1 || N > 100) {
        throw std::runtime_error("ошибка: высота ёлочки должна быть в диапазоне [1..100]");
    }

    if (data.size() != static_cast<size_t>(N)) {
        throw std::runtime_error(std::string("ошибка: введено не подходящее число строк, должно быть ") + std::to_string(N));
    }

    // массив для мемоизации при динамическом программировании
    std::vector<int> dp_memo(N, 0);

    for (int n = 0; n < N; ++n) {
        std::stringstream ss(data.at(n));
        // парсим числа очередной исходной строки
        std::vector<int> nums( (std::istream_iterator<int>(ss)), std::istream_iterator<int>() );
        if (nums.size() != static_cast<size_t>(n + 1)) {
            throw std::runtime_error(std::string("ошибка: неправильное количество чисел в строке ") + std::to_string(n) + std::string(", должно быть ") + std::to_string(n + 1));
        }

        if (n == 0) { // инициализация
            dp_memo[0] = nums[0];
            continue;
        }

        // ёлочка выглядит вот так, по факту у нас это треугольная матрица (построчно)
        //   1           1
        //  2 3     ==>  2 3
        // 4 5 6         4 5 6
        // 9 8 0 3       9 8 0 3
        // предыдущая строка будет замемоизирована в dp_memo
        // спускаемся вниз от строки к строке.
        // в каждый элемент можно попасть только спускаясь вниз-влево или вниз-вправо
        // т.е. в 5 можно попасть только из 2 или из 3 (это его родители).
        // в случае треугольного представления, это могут быть либо j-1 (если спускаемся
        // от родителя 2) или j (если спускаемся от родителя 3).
        // задача - спускаться вниз, подсчитывать суммы пути, выбирать максимальную
        // и сохранять в ячейку (мемоизация) для следующей итерации, т.е. помнить итог
        // предыдущих шагов и не пересчитывать.
        // например. для третей строки. предыдущие будут выглядеть так (накопленная сумма)
        //   1       (total sum)
        //   3 4     (total sum)
        //   4 5 6   <
        //   9 8 0 3
        // для элемента 5 смотрим накопленную сумму от родителя слева (3) и от родителя
        // справа (4), выбираем максимум (4), суммируем с текущим элементом (5), получаем
        // 4+5=9 и сохраняем в dp_memo для следующей итерации (последующей строки).
        // итого, при переходе на следующую строку получится
        //   1       (total sum)
        //   3 4     (total sum)
        //   7 9 10  (total sum)
        //   9 8 0 3 <
        // дойдя до конца, по итогу получим строку из N чисел с накопленными суммами.
        // выберем максимальное значение

        std::vector<int> dp(n+1, 0);
        for (int j = 0; j <= n; j++) {
            int max_parent = std::numeric_limits<int>::min();
            // значение от предка слева
            if (j > 0) max_parent = std::max(max_parent, dp_memo[j-1]);
            // значение от предка справа
            if (j < n) max_parent = std::max(max_parent, dp_memo[j]);
            // формируем накопленную сумму
            dp[j] = nums[j] + max_parent;
        }

        dp_memo = std::move(dp); // мемоизируем
    }

    // интересует максимум в итоговой строке
    int S = *std::max_element(dp_memo.begin(), dp_memo.end());
    return std::to_string(S);
}
