#include <memory>
#include <fstream>
#include <vector>

#include "CountingSort.h"

SortStats CountingSort::sort(int arr[], int n)
{
    SortStats stats = make_stats();
    auto start = std::chrono::high_resolution_clock::now();
    if (n > 0) {
        // нам потребуется два вспомогательных массива:
        // - для хранения счетчиков количества каждого элемента
        // - для формирования отсортированного массива

        int range = max_value_ - min_value_ + 1;         // нам нужно на один элемент больше, чтобы воспользоваться окном на шаге 2
        std::unique_ptr<int[]> counts(new int[range]()); // воспользуюсь умным указателем (это даст нам RAII)
        std::unique_ptr<int[]> output(new int[n]());     // воспользуюсь умным указателем (это даст нам RAII)
        stats.memory += range * sizeof(int);
        stats.memory += n * sizeof(int);

        // шаг 1: подсчет количества каждого элемента (частот)
        for (int i = 0; i < n; ++i) {
            counts[arr[i] - min_value_]++;
            stats.operations += 1;
        }

        // шаг 2: преобразуем counts в массив позиций, для этого используем
        // скользящее окно из двух соседних элементов.
        // получается, чем дальше к концу массива, тем больше номер позиции элемента
        // т.е. сортировка по возрастанию.
        // принимаем, что:
        // первый элемент в counts - соответствует минимальному значению во входном массиве
        // последний элемент в counts - соответствует максимальному значению во входном массиве
        // само значение в counts - это количество появлений этого элемента во входном массиве
        for (int i = 1; i < range; ++i) {
            counts[i] += counts[i - 1];
            stats.operations += 1;
        }

        // шаг 3: строим отсортированный массив
        // проход выполняем с конца для стабильности,
        // т.о. сохраним порядок элементов
        for (int i = n - 1; i >= 0; --i) {
            int pos = --counts[arr[i] - min_value_];
            output[pos] = arr[i];
            stats.operations += 2;
        }

        // шаг 4: копируем отсортированный массив обратно
        for (int i = 0; i < n; ++i) {
            arr[i] = output[i];
            stats.operations += 1;
        }
    }
    auto stop = std::chrono::high_resolution_clock::now();

    stats.duration  = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
    stats.processed = true;
    stats.size      = n;
    return stats;
}

SortStats CountingSort::sort(const std::string& in_file, const std::string& out_file)
{
    SortStats stats = make_stats();
    uint64_t total_n = 0;
    auto start = std::chrono::high_resolution_clock::now();
    {
        int range = max_value_ - min_value_ + 1;         // в нашем случае счетчики: 65536 * 8 байт = 0.5 MB (для uint64_t)
        std::unique_ptr<int[]> counts(new int[range]()); // воспользуюсь умным указателем (это даст нам RAII)
        stats.memory += range * sizeof(int);

        // шаг 1: подсчет количества каждого элемента (частот)
        {
            const size_t BUFFER_SIZE = 1024 * 1024;
            std::vector<uint16_t> buffer(BUFFER_SIZE);
            stats.memory += BUFFER_SIZE * sizeof(uint16_t);

            std::ifstream in(in_file, std::ios::binary);
            if (!in.is_open()) return stats;
            while (in) {
                in.read(reinterpret_cast<char*>(buffer.data()), BUFFER_SIZE * sizeof(uint16_t));
                size_t read = in.gcount() / sizeof(uint16_t);
                stats.operations += 1; // добавим чтение буфером как одну операцию

                for (size_t i = 0; i < read; i++) {
                    counts[buffer[i]]++;
                    stats.operations += 1;
                }
                total_n += read;
            }
            in.close();
        }

        // шаг 2: запись результата в итоговый файл
        // в отличие от алгоритма на массивах, нам приходится работать с неограниченными
        // по размеру файлами, тут для упрощения мы исключим свойство "стабильности"
        // из алгоритма. т.е. не будем преобразовывать из частот появления значений
        // из позиции в итоговом файле, и не будем при формировании отсортирвоанного
        // файла повторно считывать данные из исходного файла в обратном порядке,
        // а также делать постоянные fseek(), чтобы записать число в нужную позицию.
        // если мы начнем так делать - работа алгоритма сильно замедлится, т.к. операции
        // ввода-вывода очень дорогие. вместо этого мы наоборот даже чтение и запись
        // делаем через буферы, а не отдельными числами, чтобы ускориться.
        // итого, мы теряем стабильность алгоритма, но приобретаем скорость.
        {
            const size_t BUFFER_SIZE = 1024 * 1024;
            std::vector<uint16_t> buffer;
            buffer.reserve(BUFFER_SIZE);
            stats.memory += BUFFER_SIZE * sizeof(uint16_t);

            std::ofstream out(out_file, std::ios::binary);
            if (!out.is_open()) return stats;
            for (int value = 0; value < range; value++) {
                for (int j = 0; j < counts[value]; j++) {
                    buffer.push_back(static_cast<uint16_t>(value));
                    stats.operations += 1;

                    if (buffer.size() >= BUFFER_SIZE) {
                        out.write(reinterpret_cast<char*>(buffer.data()), buffer.size() * sizeof(uint16_t));
                        stats.operations += 1; // добавим запись буфером как одну операцию
                        buffer.clear();
                    }
                }
            }
            if (!buffer.empty()) {
                out.write(reinterpret_cast<char*>(buffer.data()), buffer.size() * sizeof(uint16_t));
                stats.operations += 1; // добавим запись буфером как одну операцию
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
