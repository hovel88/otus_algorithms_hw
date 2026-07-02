#pragma once

#include <algorithm>
#include <limits>
#include <numeric>
#include <queue>
#include <unordered_set>

#include "i_probabilistic_counter.h"

// реализация алгоритма CMS (CountMinSketch)

class CountMinSketch : public IProbabilisticCounter {
public:
    CountMinSketch(int h = 4, int w = 2048, uint64_t seed_base = 0xBAADF00DCAFEBABEULL)
    :   h_(h),  // h - число строк, т.е. хеш-функций
        w_(w),  // w - ширина столбцов, т.е. количество счетчиков
        total_(0)
    {
        table_.assign(h_, std::vector<uint32_t>(w_, 0));
        seeds_.resize(h_);
        for (int i = 0; i < h_; i++) {
            seeds_[i] = seed_base + i * 0xDEADBEEF;
        }
    }

    std::string name() const override {
        return "Count-Min Sketch (h=" + std::to_string(h_) + ", w=" + std::to_string(w_) + ")";
    }

    size_t get_memory_usage() const override {
        size_t table_mem = 0;
        for (const auto& r : table_) {
            table_mem += r.size() * sizeof(uint32_t);
        }
        return table_mem + sizeof(*this);
    }

    void print_stats() const override {
        uint64_t sum = 0;
        for (int i = 0; i < h_; i++) {
            sum = std::accumulate(table_[i].begin(), table_[i].end(), sum);
        }
        // чем больше средневзвешенное значение ячейки, тем чаше в нее
        // попадают значения, потенциально из-за коллизий. чем это значение
        // меньше, чем точнее оценка
        double avg = static_cast<double>(sum) / (h_ * w_);
        // теоретическая граница CMS рассчитывается как
        // P(|estimate_frequency(key) - real_frequency(key)| > 2 * N / w) < e^(-h)
        // т.е. вероятность ошибки при заданных параметрах и размере данных не должна превысить (2 * N / w)
        // что в процентном соотношении соответствует (e^(-h))
        double theoretical_bound = 2.0 * total_ / w_;
        double theoretical_percent = exp(-h_) * 100;
        std::cout << "  память для хранения              - " << (h_ * w_ * sizeof(uint32_t)) / 1024.0 << " KB" << std::endl;
        std::cout << "  всего было добавлено             - " << total_ << std::endl;
        std::cout << "  количестве хеш-функций           - " << h_ << std::endl;
        std::cout << "  количество ячеек-счетчиков       - " << w_ << std::endl;
        std::cout << "  средневзвешенное значение ячейки - " << std::fixed << std::setprecision(2) << avg << std::endl;
        std::cout << "  величина теор. границы (95%)     - +" << std::llround(theoretical_bound) << std::endl;
        std::cout << "  теор. вероятность превышения     - " << std::fixed << std::setprecision(2) << theoretical_percent << "%" << std::endl;
    }

    void add(const std::string& key) override {
        total_++;
        // для уменьшения влияния коллизий используется несколько хеш-функций.
        // у нас это строки в таблице. при этом каждая строка состоит из большого
        // количества ячеек-счетчиков.
        // когда приходит новый элемент из потока, примеряем его к каждой строке
        // таблице (т.е. обрабатываем каждой хеш-функцией с соответствующим seed).
        // каждая хеш-функция превращает входной элемент к число от 0 до w-1,
        // т.е. в индекс ячейки. в итоге каждый входной элемент увеличивает по одному
        // счетчику в каждой строке таблицы, по индексу, куда произошел хит.
        // тогда, если элемент встречается часто, он будет часто увеличивать одни
        // и те же ячейки-счетчики. в каждой строке используются разные хеш-функции
        // и разные seed, чтоб гарантированно получать разные индексы ячеек
        for (int i = 0; i < h_; i++) {
            uint64_t hash = hash64(key, seeds_[i]);
            int idx = hash % w_;
            table_[i][idx]++;
        }
    }

    int get_width_amount() const { return w_; }
    int get_hashes_amount() const { return h_; }

    uint32_t estimate_frequency(const std::string& key) const {
        // оценка частоты элемента в потоке.
        // для этого надо пройти по всем строкам таблицы, в каждой сроке вычисляем
        // хеш от IP (с использованием соответствующего seed), и смотрим на счетчик,
        // который стоит в ячейке строки с этим индексом.
        // берем МИНИМАЛЬНОЕ значение из всех строк.
        // минимум берем потому что могут случаться коллизии, и для других IP-адресов
        // какая-то хеш-функция может дать хит (увеличится счетчик), при этому в других
        // хеш-функциях хит будет в других счетчиках. выбирая минимум - мы выберем
        // строку с наименьшей коллизией, то есть самую точную оценку.
        // при этом надо помнить, что оценка частоты всегда >= реальной частоте, т.е.
        // никогда не занижаем (нет false negative), но можем завысить (есть false positive)
        uint32_t min_val = std::numeric_limits<uint32_t>::max();
        for (int i = 0; i < h_; i++) {
            uint64_t hash = hash64(key, seeds_[i]);
            int idx = hash % w_; // формируем индекс ячейки в строке
            min_val = std::min(min_val, table_[i][idx]);
        }
        return min_val;
    }

    std::vector<std::pair<std::string, uint32_t>>
    get_top_n(const std::vector<std::string>& keys, size_t n = 10) {
        // у нас есть вероятностная структура, которая позволяет оценить частоту встречи
        // любого элемента, но сами элементы мы не храним.
        // поэтому нам надо получить список тех элементов, которые мы выстроим в Top-N.
        // для этого будем использовать структуру для min-heap - храним {частота, ключ}.
        // в таком случае структура сама обеспечит сортировку по частоте (по возрастанию,
        // вершина будет минимальной), а затем развернем порядок.
        // куча - эффективная структура данных:
        // - хранит только N элементов из всех (формируя Top-N)
        // - память - O(N) для хранения Top-N (ну и сама таблица O(h * w))
        // - вставка O(log N). в нашем случае нужно сформировать кучу из N элементов,
        //   вставив в нее M элементов. сложность будет O(M * h хешей + M * log N операций с кучей).
        using Data = std::pair<uint32_t, std::string>;
        auto cmp = [](const Data& a, const Data& b) {
            return a.first > b.first; // min-heap: меньшая частота наверху кучи
        };
        std::priority_queue<Data, std::vector<Data>, decltype(cmp)> min_heap(cmp);

        std::unordered_set<std::string> unique_keys;
        unique_keys.reserve(keys.size());
        for (const auto& key : keys) {
            unique_keys.insert(key);
        }

        for (const auto& key : unique_keys) {
            uint32_t freq = estimate_frequency(key);

            if (min_heap.size() < n) {
                // если куча еще не заполнена до N, просто добавляем
                min_heap.push({freq, key});
            } else
            if (freq > min_heap.top().first) {
                // если куча заполнена, и текущий элемент чаще, чем самый редкий в куче:
                // - удаляем самый редкий (он выпадает из Top-N)
                // - добавляем текущий (он входит в Top-N), вставится в нужную позицию
                min_heap.pop();
                min_heap.push({freq, key});
            } else {
                // иначе: текущий элемент реже, чем все в куче, просто игнорируем
            }
        }

        // извлекаем все элементы из кучи в вектор
        // и разворачиваем, т.к. в куче они по возрастаю частоты
        std::vector<std::pair<std::string, uint32_t>> result;
        while (!min_heap.empty()) {
            auto [freq, key] = min_heap.top();
            min_heap.pop();
            result.emplace_back(key, freq);
        }
        std::reverse(result.begin(), result.end());
        return result;
    }

private:
    int h_{0};
    int w_{0};
    uint64_t total_{0};
    std::vector<std::vector<uint32_t>> table_{};
    std::vector<uint64_t> seeds_{};
};
