#include <iomanip>
#include <fstream>
#include <random>
#include <chrono>
#include <filesystem>
#include <list>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "CountingSort.h"
#include "RadixSort.h"
#include "BucketSort.h"

std::string _print_array(int arr[], int n, const std::string& prefix = "")
{
    std::stringstream ss;
    ss << prefix;
    for (int i = 0; i < n; ++i) {
        ss << arr[i];
        ss << (i + 1 == n ? "" : " ");
    }
    return ss.str();
}

bool _is_file_sorted(const std::string& filename)
{
    std::ifstream in(filename, std::ios::binary);
    if (!in.is_open()) return false;

    const size_t BUFFER_SIZE = 1024 * 1024;
    std::vector<uint16_t> buffer(BUFFER_SIZE);

    uint16_t prev = 0;
    bool first = true;
    while (in) {
        in.read(reinterpret_cast<char*>(buffer.data()), BUFFER_SIZE * sizeof(uint16_t));
        size_t read = in.gcount() / sizeof(uint16_t);

        for (size_t i = 0; i < read; i++) {
            if (!first && buffer[i] < prev) {
                return false;
            }
            prev = buffer[i];
            first = false;
        }
    }

    return true;
}

void _debug_print_file(const std::string& filename, int count = 10)
{
    std::ifstream in(filename, std::ios::binary);
    if (!in.is_open()) return;

    std::cout << filename << ":" << std::endl;

    std::vector<uint16_t> buffer(count);
    size_t read = 0;

    in.read(reinterpret_cast<char*>(buffer.data()), count * sizeof(uint16_t));
    read = in.gcount() / sizeof(uint16_t);
    std::cout << "   ";
    for (size_t i = 0; i < read; i++) {
        std::cout << buffer[i] << " ";
    }

    in.seekg(-(count * sizeof(uint16_t)), std::ios::end);
    in.read(reinterpret_cast<char*>(buffer.data()), count * sizeof(uint16_t));
    read = in.gcount() / sizeof(uint16_t);
    std::cout << "... ";
    for (size_t i = 0; i < read; i++) {
        std::cout << buffer[i] << " ";
    }

    std::cout << std::endl;

    in.close();
}

void _generate_test_file(const std::string& filename, uint64_t count, int min, int max)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<uint16_t> dist(min, max);

    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "не могу открыть файл " << std::quoted(filename) << std::endl;
        return;
    }

    std::cout << "генерация "             << count
              << " 16-bit чисел в файле " << std::quoted(filename)
              << " (" << (count * 2) / (1024*1024) << " MB)..." << std::endl;
    const size_t BUFFER_SIZE = 1024 * 1024;
    std::vector<uint16_t> buffer(BUFFER_SIZE);

    auto start = std::chrono::high_resolution_clock::now();
    {
        for (uint64_t i = 0; i < count; i += BUFFER_SIZE) {
            size_t to_generate = std::min(BUFFER_SIZE, static_cast<size_t>(count - i));
            for (size_t j = 0; j < to_generate; j++) {
                buffer[j] = dist(gen);
            }
            file.write(reinterpret_cast<char*>(buffer.data()), to_generate * sizeof(uint16_t));
            if (i % (BUFFER_SIZE * 10) == 0) {
                std::cout << "\r..." << (i * 100 / count) << "%" << std::flush;
            }
        }
        std::cout << std::endl;
    }
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
    std::cout << "сгенерировано " << count << " чисел за " << duration.count() << " ns" << std::endl;

    file.close();
}

// --------------------------------------------------------

void test_random_arrays()
{
    std::cout << "\n" << std::string(30, '=') << std::endl;
    std::cout << "Тест на рандомизированных массивах разных размеров" << std::endl;

    const int sizes[] = {100, 1000, 10000, 100000, 1000000};
    // const int sizes[] = {40};
    int max_size = 0;
    for (const auto sz : sizes) {
        if (sz > max_size) max_size = sz;
    }
    int arr[max_size] = {0};
    srand(12345);
    for (int i = 0; i < max_size; ++i) {
        arr[i] = rand() % 1000;
    }
    {
        int sz = 20;
        int arr_first[sz] = {0};
        int arr_last[sz] = {0};
        int bytes = sz * sizeof(int);
        memcpy(arr_first, arr, bytes);
        memcpy(arr_last, arr + (max_size - sz), bytes);
        std::cout << "Первые и последние " << sz << " элементов массива из " << max_size << " элементов: \n"
                  << _print_array(arr_first, sz, "   ")
                  << _print_array(arr_last, sz, " ... ") << std::endl;
    }

    std::list<SortStats> results;
    for (const auto sz : sizes) {
        {
            int bytes = sz * sizeof(int);
            int arr_copy[sz] = {0};
            memcpy(arr_copy, arr, bytes);

            CountingSort sort_algo(/*min=*/0, /*max=*/999);
            auto st = sort_algo.sort(arr_copy, sz);
            // std::string dbg_str = sort_algo.name() + std::string(" out: ");
            // std::cout << _print_array(arr_copy, sz, dbg_str) << std::endl;

            results.emplace_back(st);
        }
        {
            int bytes = sz * sizeof(int);
            int arr_copy[sz] = {0};
            memcpy(arr_copy, arr, bytes);

            RadixSort sort_algo(/*base=*/10, /*max=*/999);
            auto st = sort_algo.sort(arr_copy, sz);
            // std::string dbg_str = sort_algo.name() + std::string(" out: ");
            // std::cout << _print_array(arr_copy, sz, dbg_str) << std::endl;

            results.emplace_back(st);
        }
        {
            int bytes = sz * sizeof(int);
            int arr_copy[sz] = {0};
            memcpy(arr_copy, arr, bytes);

            BucketSort sort_algo(/*buckets=*/100, /*min=*/0, /*max=*/999);
            auto st = sort_algo.sort(arr_copy, sz);
            // std::string dbg_str = sort_algo.name() + std::string(" out: ");
            // std::cout << _print_array(arr_copy, sz, dbg_str) << std::endl;

            results.emplace_back(st);
        }
    }

    std::cout << std::endl;
    std::cout << std::left  << std::setw(30) << "Name"
              << std::right << std::setw(15) << "Array size"
              << std::right << std::setw(15) << "Operations"
              << std::right << std::setw(15) << "Memory"
              << std::right << std::setw(20) << "Duration (ns)"
              << std::endl;
    for (const auto& res : results) {
        std::cout << std::left  << std::setw(30) << res.name
                  << std::right << std::setw(15) << res.size
                  << std::right << std::setw(15) << res.operations
                  << std::right << std::setw(15) << res.memory
                  << std::right << std::setw(20) << res.duration.count()
                  << std::endl;
    }
}

void test_big_file()
{
    std::cout << "\n" << std::string(30, '=') << std::endl;
    std::cout << "Тест с бинарным файлом большого размера" << std::endl;

    // конфигурации:
    // N    - количество чисел в файле,
    // min  - минимальное число в диапазоне,
    // max  - максимальное число в диапазоне,
    // in   - имя файла для генерации входных данных
    // out  - имя файла, в который поместить выходные данные
    std::vector<std::tuple<uint64_t, int, int, std::string, std::string>> tests = {
    //   N              min     max     in  out
        {1000000000,    0,      65535,  "", ""}
    };

    const auto test_dir = std::filesystem::current_path() / std::filesystem::path("tests");
    for (auto& [N, min, max, input_name, output_name] : tests) {
        {
            std::string name    = "input_n" + std::to_string(N) + "_min" + std::to_string(min) + "_max" + std::to_string(max);
            const auto filepath = test_dir / std::filesystem::path(name);
            input_name          = filepath.string();
        }
        {
            std::string name    = "output_n" + std::to_string(N) + "_min" + std::to_string(min) + "_max" + std::to_string(max);
            const auto filepath = test_dir / std::filesystem::path(name);
            output_name         = filepath.string();
        }
    }

    std::list<SortStats> results;
    for (auto& [N, min, max, input_name, output_name] : tests) {
        std::cout << "\nTEST: N=" << N << ", min=" << min << ", max=" << max << std::endl;

        std::string input_filename = input_name + ".bin";
        _generate_test_file(input_filename, N, min, max);
        _debug_print_file(input_filename);

        {
            CountingSort sort_algo(/*min=*/min, /*max=*/max);
            std::string output_filename = output_name + "__cs.bin";
            auto st = sort_algo.sort(input_filename, output_filename);
            _debug_print_file(output_filename);
            // std::filesystem::remove(output_filename);

            results.emplace_back(st);
        }
        {
            RadixSort sort_algo(/*base=*/256, /*max=*/max);
            std::string output_filename = output_name + "__rs.bin";
            auto st = sort_algo.sort(input_filename, output_filename);
            _debug_print_file(output_filename);
            // std::filesystem::remove(output_filename);

            results.emplace_back(st);
        }
        {
            BucketSort sort_algo(/*buckets=*/256, /*min=*/min, /*max=*/max);
            std::string output_filename = output_name + "__bs.bin";
            auto st = sort_algo.sort(input_filename, output_filename);
            _debug_print_file(output_filename);
            // std::filesystem::remove(output_filename);

            results.emplace_back(st);
        }
    }

    std::cout << std::endl;
    std::cout << std::left  << std::setw(30) << "Name"
              << std::right << std::setw(15) << "Array size"
              << std::right << std::setw(15) << "Operations"
              << std::right << std::setw(15) << "Memory"
              << std::right << std::setw(20) << "Duration (ns)"
              << std::endl;
    for (const auto& res : results) {
        std::cout << std::left  << std::setw(30) << res.name
                  << std::right << std::setw(15) << res.size
                  << std::right << std::setw(15) << res.operations
                  << std::right << std::setw(15) << res.memory
                  << std::right << std::setw(20) << res.duration.count()
                  << std::endl;
    }
}

int main(int argc, char** argv)
{
    std::cout << "программа оценки работы алгоритмов линейной контекстно-зависимой сортировки (CountingSort, RadixSort, BucketSort)" << std::endl;

    test_random_arrays();
    test_big_file();
    return 0;
}
