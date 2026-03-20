#include <memory>
#include <fstream>
#include <cstring>
#include <vector>
#include <filesystem>

#include "RadixSort.h"

SortStats RadixSort::sort(int arr[], int n)
{
    SortStats stats = make_stats();
    auto start = std::chrono::high_resolution_clock::now();
    if (n > 0) {
        // нам потребуется два вспомогательных массива:
        // - для хранения счетчиков количества каждого элемента (по каждому разряду)
        // - для формирования отсортированного массива

        std::unique_ptr<int[]> output(new int[n]()); // воспользуюсь умным указателем (даст нам RAII)
        stats.memory += n * sizeof(int);
        stats.memory += base_ * sizeof(int); // этот будет выделяться в цикле

        // для каждого разряда (начиная с младшего) выполняем
        // CountingSort применительно к конкретному разряду числа
        for (int exp = 1; (max_value_ / exp) > 0; exp *= base_) {
            std::unique_ptr<int[]> counts(new int[base_]()); // воспользуюсь умным указателем (даст нам RAII)

            // шаг 1: подсчет количества элементов для значения разряда
            for (int i = 0; i < n; ++i) {
                int digit = (arr[i] / exp) % base_;
                counts[digit]++;
                stats.operations += 2;
            }

            // шаг 2: преобразуем counts в массив позиций, для этого используем
            // скользящее окно из двух соседних элементов.
            // получается, чем дальше к концу массива, тем больше номер позиции элемента
            // т.е. сортировка по возрастанию.
            // принимаем, что:
            // первый элемент в counts - соответствует минимальному значению во входном массиве
            // последний элемент в counts - соответствует максимальному значению во входном массиве
            // само значение в counts - это количество появлений этого элемента во входном массиве
            for (int i = 1; i < base_; ++i) {
                counts[i] += counts[i - 1];
                stats.operations += 1;
            }

            // шаг 3: построение отсортированного по разряду массива
            // проход выполняем с конца для стабильности,
            // т.о. сохраним порядок элементов
            for (int i = n - 1; i >= 0; --i) {
                int digit = (arr[i] / exp) % base_;
                int pos = --counts[digit];
                output[pos] = arr[i];
                stats.operations += 3;
            }

            // шаг 4: копируем отсортированный массив обратно
            for (int i = 0; i < n; ++i) {
                arr[i] = output[i];
                stats.operations++;
            }

            // после выполнения каждой итерации мы получаем частично отсортированный
            // массив, с каждым разрядом всё более и более отсортированный.
            // только сортировка не по значению в целом, а по цифре в разряде.
            // например, после первой итерации получим такой массив
            //      50, 20, 91, 21, 51, ..., 48, 28, 49, 69, 79, 69
            // видно, что числа не отсортированы, но расположены группами: сначала те,
            // где в младшем разряде 0, затем - где 1 и т.д.
            // после второй итерации получим такой массив
            //      20, 21, 28, 48, 49, ..., 69, 79
            // теперь в нужную позицию выстраиваются числа по цифрам второго разряда.
            // тоже по возрастанию. при этом правильный порядок по младшему разряду
            // обеспечивается за счет стабильности алгоритма CountingSort, т.к.
            // на прошлой итерации вначале шли числа с цифрой 0, затем с 1...
            // именно в такой последовательности они и начнут появляться на этой итерации
        }
    }
    auto stop = std::chrono::high_resolution_clock::now();

    stats.duration  = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
    stats.processed = true;
    stats.size      = n;
    return stats;
}

SortStats RadixSort::sort(const std::string& in_file, const std::string& out_file)
{
    SortStats stats = make_stats();
    auto start = std::chrono::high_resolution_clock::now();
    {
        std::string temp_in_file;
        std::string prev_in_file;
        bool first_step = true;
        for (int exp = 1; (max_value_ / exp) > 0; exp *= base_) {
            temp_in_file = out_file + "." + std::to_string(exp) + ".tmp";
            const std::string& dest_file   = temp_in_file;
            const std::string& source_file = (first_step) ? in_file : prev_in_file;
            if (first_step) first_step = false;

            auto st = _sort_iteration(source_file, dest_file, exp);
            stats.size = st.size;
            stats.memory += st.memory;
            stats.operations += st.operations;

            if (!prev_in_file.empty()) std::filesystem::remove(prev_in_file);
            prev_in_file = temp_in_file;
        }
        if (!prev_in_file.empty()) std::filesystem::rename(prev_in_file, out_file);
    }
    auto stop = std::chrono::high_resolution_clock::now();

    stats.duration  = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
    stats.processed = true;
    return stats;
}

SortStats RadixSort::_sort_iteration(const std::string& in_file, const std::string& out_file, int exp)
{
    SortStats stats = make_stats();
    uint64_t total_n = 0;

    std::vector<int> counts(base_, 0);
    stats.memory += base_ * sizeof(int);

    // Шаг 1: подсчет количества элементов для значения разряда
    {
        std::ifstream in(in_file, std::ios::binary);
        if (!in.is_open()) return stats;

        const size_t BUFFER_SIZE = 1024 * 1024;
        std::vector<uint16_t> buffer(BUFFER_SIZE);
        stats.memory += BUFFER_SIZE * sizeof(uint16_t);

        while (in) {
            in.read(reinterpret_cast<char*>(buffer.data()), BUFFER_SIZE * sizeof(uint16_t));
            size_t read = in.gcount() / sizeof(uint16_t);
            stats.operations += 1; // добавим чтение буфером как одну операцию

            for (size_t i = 0; i < read; i++) {
                int digit = (buffer[i] / exp) % base_;
                counts[digit]++;
                stats.operations += 2;
            }
            total_n += read;
        }
        
        in.close();
    }

    // шаг 2: преобразуем counts в позиции (cumulative sum)
    for (int i = 1; i < base_; ++i) {
        counts[i] += counts[i - 1];
        stats.operations += 1;
    }

    // шаг 3: построение отсортированного по разряду массива в файле
    // бегать по файлу и вставлять элементы из исходного файла в нужные
    // позиции - не самый быстрой процесс, да еще и снова перечитывать
    // исходный файл приходится. операции ввода-вывода очень дорогие.
    // но без этого алгоритм не сможет отсортировать, для конкретно
    // этой реализации "стабильность" крайне важна и от нее не отказаться
    {
        std::ifstream in(in_file, std::ios::binary);
        std::ofstream out(out_file, std::ios::binary);
        if (!in.is_open() || !out.is_open()) return stats;

        // нужно знать общее количество элементов
        in.seekg(0, std::ios::end);
        uint64_t file_sz  = in.tellg();
        uint64_t elements = file_sz / sizeof(uint16_t);
        // in.seekg(0, std::ios::beg);

        // еще один временный буфер для выходных данных
        std::vector<uint16_t> temp_buffer(elements);
        stats.memory += elements * sizeof(uint16_t);

        const size_t BUFFER_SIZE = 1024 * 1024;
        std::vector<uint16_t> buffer(BUFFER_SIZE);
        stats.memory += BUFFER_SIZE * sizeof(uint16_t);

        int block = 1;
        while (in) {
            in.seekg(-(block++ * BUFFER_SIZE * sizeof(uint16_t)), std::ios::end);
            in.read(reinterpret_cast<char*>(buffer.data()), BUFFER_SIZE * sizeof(uint16_t));
            size_t read = in.gcount() / sizeof(uint16_t);

            for (int i = read - 1; i >= 0; --i) {
                int digit = (buffer[i] / exp) % base_;
                int pos = --counts[digit];
                temp_buffer[pos] = buffer[i];
                stats.operations += 3;
            }
        }

        out.write(reinterpret_cast<char*>(temp_buffer.data()), elements * sizeof(uint16_t));

        in.close();
        out.close();
    }

    stats.processed = true;
    stats.size      = total_n;
    return stats;
}
