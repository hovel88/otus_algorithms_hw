#pragma once

#include <fstream>
#include <iostream>
#include <iomanip>
#include <chrono>
#include <filesystem>
#include <exception>
#include <string>
#include <vector>
#include <tuple>

class ExternalSort
{
public:
    typedef enum {
        V1 = 0,
        V2,
        V3
    } version_t;

    struct SortStats {
        bool                     processed{false};  // обработали или скипнули?
        int                      array_size{0};     // размер массива, на котором производилась сортировка, сколько обработали элементов
        int                      unique_values{0};  // количество уникальных значений
        std::string              name{};            // название алгоритма сортировки
        std::chrono::nanoseconds duration{};        // длительность, затраченная в основном на операции ввода-вывода
    };

public:
    virtual ~ExternalSort() = default;
    ExternalSort() = delete;
    ExternalSort(const ExternalSort&) = delete;
    ExternalSort(ExternalSort&&) = delete;
    ExternalSort& operator=(const ExternalSort&) = delete;
    ExternalSort& operator=(ExternalSort&&) = delete;

    explicit ExternalSort(version_t v, const std::filesystem::path& dir = std::filesystem::temp_directory_path())
    :   name_( (v == version_t::V1) ? ("ExternalSort_v1") : ( (v == version_t::V2) ? ("ExternalSort_v2") : ( (v == version_t::V3) ? ("ExternalSort_v3") : ("ExternalSort_v???") ) ) ),
        ver_(v),
        temp_dir_(dir)
    {}

    const std::string& name() const {
        return name_;
    }

    void clear_stats() {
        stats_ = {};
    }
    SortStats get_stats() {
        stats_.name = name();
        return stats_;
    }

    bool sort(const std::string& in_file, const std::string& out_file) {
        clear_stats();
        bool rv = false;
        auto start = std::chrono::high_resolution_clock::now();
        switch (ver_) {
        case version_t::V1:
            rv = sort_v1(in_file, out_file);
            break;
        case version_t::V2:
            rv = sort_v2(in_file, out_file);
            break;
        case version_t::V3:
            rv = sort_v3(in_file, out_file);
            break;
        default:
            break;
        }
        auto end = std::chrono::high_resolution_clock::now();
        if (rv) stats_.duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
        stats_.processed = rv;
        return rv;
    }

private:
    const std::string           name_{};
    const version_t             ver_{V1};
    const std::filesystem::path temp_dir_{};
    SortStats                   stats_{};

    bool sort_v1(const std::string& in_file, const std::string& out_file);
    bool sort_v2(const std::string& in_file, const std::string& out_file, uint64_t segment_size = 1); // начинаем с отрезка (сегмента) из одного элемента, он упорядочен
    bool sort_v3(const std::string& in_file, const std::string& out_file);

    std::string _make_temp_filepath(int number);
    uint64_t _get_elements_total(const std::string& in_file);
    int _split(uint64_t segment_size, std::ifstream& input, std::ofstream& output1, std::ofstream& output2);
    void _merge(uint64_t number_of_series, uint64_t segment_size, std::ifstream& input1, std::ifstream& input2, std::ofstream& output);
    void _merge_segment(uint64_t segment_size, std::ifstream& input1, std::ifstream& input2, std::ofstream& output);
};
