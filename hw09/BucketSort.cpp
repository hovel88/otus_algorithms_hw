#include <memory>
#include <fstream>
#include <filesystem>
#include <vector>
#include <algorithm>
#include <iterator>
#include <cmath>

#include "BucketSort.h"

SortStats BucketSort::sort(int arr[], int n)
{
    SortStats stats = make_stats();
    auto start = std::chrono::high_resolution_clock::now();
    if (n > 0) {
        // нам потребуется некоторое количество вспомогательным массивов:
        // - по количеству блоков (корзин)
        // однако заранее не известно сколько элементов будет в каждом
        // из массивов-корзин.
        // очень не экономичный подход в лоб - выделить матрицу NxN, зная
        // заранее, что всё выделенное место не будет израсходовано.
        // но лучше, если это будет массив динамических массивов, или массив
        // связанных списков например.
        // ПРИМЕЧАНИЕ: т.к. мы исследуем именно сортировку, я позволю себе
        // ради простоты (чтобы легче прослеживать именно алгоритм сортировки,
        // а не вспомогательные штуки) использовать std::vector в качестве
        // динамического массива массивов

        std::vector<std::vector<int>> buckets(num_buckets_);
        // по факту, дополнительной памяти потребуется столько же, сколько
        // и размер входных данных, и ещё немного сверху, на поддержание
        // работы связных списков
        stats.memory += n;
        stats.memory += num_buckets_ * sizeof(std::vector<int>);

        // шаг 1: распределяем элементы по корзинам
        int range = max_value_ - min_value_ + 1;
        for (int i = 0; i < n; ++i) {
            int index = (arr[i] - min_value_) * num_buckets_ / range;
            if (index >= num_buckets_) index = num_buckets_ - 1;
            buckets[index].push_back(arr[i]);
            stats.operations += 2;
        }

        // шаг 2: сортируем каждую корзину
        // тоже позволю себе использовать std::stable_sort, чтобы алгоритм остался
        // прозрачнее сложность std::sort будем считать как O(N log N)
        for (auto& bucket : buckets) {
            std::stable_sort(bucket.begin(), bucket.end());
            auto N     = bucket.size();
            auto Log_N = (std::log2(bucket.size()) < 0) ? 0 : std::log2(bucket.size());
            stats.operations += (N * Log_N);
        }

        // шаг 3: собираем данные из корзин обратно
        int index = 0;
        for (auto& bucket : buckets) {
            for (int num : bucket) {
                arr[index++] = num;
                stats.operations += 1;
            }
        }
    }
    auto stop = std::chrono::high_resolution_clock::now();

    stats.duration  = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
    stats.processed = true;
    stats.size      = n;
    return stats;
}

SortStats BucketSort::sort(const std::string& in_file, const std::string& out_file)
{
    SortStats stats = make_stats();
    uint64_t total_n = 0;
    auto start = std::chrono::high_resolution_clock::now();
    {
        // шаг 1: распределение по временным файлам-корзинам
        std::vector<std::string>   bucket_files;
        std::vector<std::uint64_t> bucket_sizes(num_buckets_, 0);

        {
            std::ifstream in(in_file, std::ios::binary);
            if (!in.is_open()) return stats;

            // создаем выходные файлы для каждой корзины
            std::vector<std::ofstream> buckets;
            for (int i = 0; i < num_buckets_; i++) {
                std::string filename = out_file + ".bucket_" + std::to_string(i) + ".tmp";
                bucket_files.push_back(filename);
                buckets.emplace_back(filename, std::ios::binary);
            }
            stats.memory += buckets.size() * sizeof(buckets); // дополнительные накладные расходы

            const size_t BUFFER_SIZE = 1024 * 1024;
            std::vector<uint16_t> buffer(BUFFER_SIZE);
            stats.memory += BUFFER_SIZE * sizeof(uint16_t);

            int range = max_value_ - min_value_ + 1;
            while (in) {
                in.read(reinterpret_cast<char*>(buffer.data()), BUFFER_SIZE * sizeof(uint16_t));
                size_t read = in.gcount() / sizeof(uint16_t);
                stats.operations += 1; // добавим чтение буфером как одну операцию

                for (size_t i = 0; i < read; i++) {
                    uint16_t value = buffer[i];
                    int index = (value - min_value_) * num_buckets_ / range;
                    if (index >= num_buckets_) index = num_buckets_ - 1;
                    buckets[index].write(reinterpret_cast<char*>(&value), sizeof(uint16_t));
                    bucket_sizes[index]++;
                    stats.operations += 3;
                }

                total_n += read;
            }
            in.close();
            for (auto& bucket : buckets) bucket.close();

            stats.memory += bucket_files.size() * sizeof(bucket_files); // дополнительные накладные расходы
            stats.memory += bucket_sizes.size() * sizeof(bucket_sizes); // дополнительные накладные расходы
        }

        // шаг 2: сортировка каждой корзины
        {
            std::ofstream out(out_file, std::ios::binary);
            if (!out.is_open()) return stats;

            for (int index = 0; index < num_buckets_; ++index) {
                if (bucket_sizes[index] == 0) continue;

                // читаем корзину, а затем сортируем этот массив и записываем его в итоговый файл
                std::vector<int> arr;
                {
                    std::ifstream bucket(bucket_files[index], std::ios::binary);
                    std::vector<uint16_t> bucket_data(bucket_sizes[index]);
                    bucket.read(reinterpret_cast<char*>(bucket_data.data()), bucket_sizes[index] * sizeof(uint16_t));
                    stats.operations += 1; // добавим чтение буфером как одну операцию
                    bucket.close();

                    std::transform(bucket_data.begin(), bucket_data.end(), std::back_inserter(arr), [](uint16_t val){ return static_cast<int>(val); });
                }
                auto st = sort(arr.data(), arr.size());
                stats.operations += st.operations;
                stats.memory += st.memory;
                stats.memory += arr.size() * sizeof(uint16_t); // дополнительные расходы на буфер

                const size_t BUFFER_SIZE = 1024 * 1024;
                std::vector<uint16_t> buffer;
                buffer.reserve(BUFFER_SIZE);
                stats.memory += BUFFER_SIZE * sizeof(uint16_t);

                for (auto value : arr) {
                    buffer.push_back(static_cast<uint16_t>(value));
                    stats.operations += 1;

                    if (buffer.size() >= BUFFER_SIZE) {
                        out.write(reinterpret_cast<char*>(buffer.data()), buffer.size() * sizeof(uint16_t));
                        stats.operations += 1; // добавим запись буфером как одну операцию
                        buffer.clear();
                    }
                }
                if (!buffer.empty()) {
                    out.write(reinterpret_cast<char*>(buffer.data()), buffer.size() * sizeof(uint16_t));
                    stats.operations += 1; // добавим запись буфером как одну операцию
                }

                std::filesystem::remove(bucket_files[index]);
            }

            out.close();
        }
    }
    auto stop = std::chrono::high_resolution_clock::now();

    stats.duration  = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
    stats.processed = true;
    stats.size      = total_n;
    return stats;
}
