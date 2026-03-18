#include <iostream>
#include <fstream>
#include <iomanip>
#include <random>
#include <chrono>
#include <filesystem>
#include <list>

#include "ExternalSort.h"

void _generate_test_file(const std::string& filename, int N, int T)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(1, T);

    std::ofstream out_stream(filename);
    if (!out_stream.is_open()) {
        std::cerr << "не могу создать файл " << std::quoted(filename) << std::endl;
        return;
    }

    std::cout << "генерация " << N << " чисел (1.." << T << ") в файле " << std::quoted(filename) << "... " << std::endl;
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N; i++) {
        out_stream << dist(gen) << "\n";
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    std::cout << "сгенерировано " << N << " чисел за " << duration.count() << " ns" << std::endl;

    out_stream.close();
}

bool _is_file_sorted(const std::string& filename)
{
    std::ifstream in_stream(filename);
    if (!in_stream.is_open()) {
        std::cerr << "не могу открыть файл " << std::quoted(filename) << std::endl;
        return false;
    }

    int prev = 0;
    int curr = 0;
    if (!(in_stream >> prev)) return true; // пустой файл
    while (in_stream >> curr) {
        if (prev > curr) {
            std::cout << "файл " << std::quoted(filename) << " не отсортирован: " << prev << " > " << curr << std::endl;
            return false;
        }
        prev = curr;
    }
    return true;
}

void _print_file_head(const std::string& filename, int count = 20)
{
    std::ifstream in_stream(filename);
    if (!in_stream.is_open()) {
        std::cout << "не могу открыть файл " << std::quoted(filename) << std::endl;
        return;
    }

    std::cout << filename << ":" << std::endl;
    std::cout << "   ";
    int value;
    for (int i = 0; i < count && in_stream >> value; i++) {
        std::cout << value << " ";
    }
    if (in_stream >> value) std::cout << "...";
    std::cout << std::endl;
    in_stream.close();
}

int main(int argc, char** argv)
{
    std::cout << "программа оценки работы алгоритмов внешней сортировки (ExternalSort)" << std::endl;

    // конфигурации:
    // N    - количество строк в файле,
    // T    - числа от 1 до T,
    // in   - имя файла для генерации входных данных
    // out  - имя файла, в который поместить выходные данные
    // всего 10 комбинаций N и T
    std::vector<std::tuple<uint64_t, int, std::string, std::string>> tests = {
    //   N        T         in  out
        {100,     10,       "", ""},
        {1000,    10,       "", ""},
        {10000,   10,       "", ""},
        {100000,  10,       "", ""},
        {1000000, 10,       "", ""},
        {100,     100,      "", ""},
        {1000,    1000,     "", ""},
        {10000,   10000,    "", ""},
        {100000,  100000,   "", ""},
        {1000000, 1000000,  "", ""}
    };

    const auto test_dir = std::filesystem::current_path() / std::filesystem::path("external_tests");
    for (auto& [N, T, input_name, output_name] : tests) {
        {
            std::string name    = "input_n" + std::to_string(N) + "_t" + std::to_string(T);
            const auto filepath = test_dir / std::filesystem::path(name);
            input_name          = filepath.string();
        }
        {
            std::string name    = "output_n" + std::to_string(N) + "_t" + std::to_string(T);
            const auto filepath = test_dir / std::filesystem::path(name);
            output_name         = filepath.string();
        }
    }

    std::list<ExternalSort::SortStats> results;
    for (auto& [N, T, input_name, output_name] : tests) {
        std::cout << "\n" << std::string(30, '=') << std::endl;
        std::cout << "TEST: N=" << N << ", T=" << T << std::endl;

        std::string input_filename = input_name + ".txt";
        _generate_test_file(input_filename, N, T);
        _print_file_head(input_filename);

        // V1: ограничу для T не больше 1000, чтобы не создавать слишком много файлов/дескрипторов
        {
            ExternalSort sort_algo(ExternalSort::V1, test_dir);
            if (T <= 1000) {
                std::string output_filename = output_name + "__v1.txt";
                if (sort_algo.sort(input_filename, output_filename)) {
                    auto st = sort_algo.get_stats();
                    st.array_size    = N;
                    st.unique_values = T;
                    results.emplace_back(st);
                    _print_file_head(output_filename);
                    // std::filesystem::remove(output_filename);
                }
            } else {
                auto st = sort_algo.get_stats();
                st.array_size    = N;
                st.unique_values = T;
                results.emplace_back(st);
                std::cout << sort_algo.name() << " пропущено: T=" << T << " слишком большое" << std::endl;
            }
        }

        {
            ExternalSort sort_algo(ExternalSort::V2, test_dir);
            std::string output_filename = output_name + "__v2.txt";
            if (sort_algo.sort(input_filename, output_filename)) {
                auto st = sort_algo.get_stats();
                st.array_size    = N;
                st.unique_values = T;
                results.emplace_back(st);
                _print_file_head(output_filename);
                // std::filesystem::remove(output_filename);
            }
        }

        {
            ExternalSort sort_algo(ExternalSort::V3, test_dir);
            std::string output_filename = output_name + "__v3.txt";
            if (sort_algo.sort(input_filename, output_filename)) {
                auto st = sort_algo.get_stats();
                st.array_size    = N;
                st.unique_values = T;
                results.emplace_back(st);
                _print_file_head(output_filename);
                // std::filesystem::remove(output_filename);
            }
        }
    }

    std::cout << std::endl;
    std::cout << std::left  << std::setw(20) << "Name"
              << std::right << std::setw(15) << "Processing"
              << std::right << std::setw(15) << "Unique values"
              << std::right << std::setw(15) << "Array size"
              << std::right << std::setw(20) << "Duration (ns)"
              << std::endl;
    for (const auto& res : results) {
        std::cout << std::left  << std::setw(20) << res.name
                  << std::right << std::setw(15) << (res.processed ? "processed" : "skipped")
                  << std::right << std::setw(15) << res.unique_values
                  << std::right << std::setw(15) << res.array_size
                  << std::right << std::setw(20) << res.duration.count()
                  << std::endl;
    }

    return 0;
}
