#pragma once

#include <iostream>
#include <exception>
#include <string>
#include <iomanip>
#include <chrono>
#include <functional>

class ISort
{
public:
    using sort_func = std::function<bool(int*, int)>;

    struct SortStats {
        uint64_t    comps{0};                // количество операций сравнений
        uint64_t    swaps{0};                // количество обменов (перестановок)
        bool        processed{false};        // обработали или скипнули?
        int         size{0};                 // размер массива, на котором производилась сортировка
        std::string name{};                  // название алгоритма сортировки
        std::chrono::nanoseconds duration{}; // длительность выполнения

        friend std::ostream& operator <<(std::ostream& os, const SortStats& stat);
    };

public:
    virtual ~ISort() = default;
    ISort() = delete;
    ISort(const ISort&) = delete;
    ISort(ISort&&) = delete;
    ISort& operator=(const ISort&) = delete;
    ISort& operator=(ISort&&) = delete;

    explicit ISort(const char* name)
    :   ISort(std::string(name))
    {}
    explicit ISort(const std::string& name)
    :   name_(name)
    {}

    const std::string& name() const {
        return name_;
    }

    void inc_comps_cnt() {
        ++stats_.comps;
    }
    void inc_swaps_cnt() {
        ++stats_.swaps;
    }
    void clear_stats() {
        stats_.comps = 0;
        stats_.swaps = 0;
        stats_.duration = std::chrono::nanoseconds{};
        stats_.name = name();
    }
    SortStats sort_stats() const {
        return stats_;
    }

    void swap(int arr[], int i, int j) {
        inc_swaps_cnt();
        // XXX: небезопасно, но будем считать что за корректность индексов отвечает код выше
        int temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }

    static std::string print_array(int arr[], int n, const std::string& prefix = "") {
        std::stringstream ss;
        ss << prefix;
        for (int i = 0; i < n; ++i) {
            ss << arr[i];
            ss << (i + 1 == n ? "" : " ");
        }
        return ss.str();
    }

    bool scoped_sort(sort_func sort_fn, int arr[], int n = 0) {
        clear_stats();
        auto start = std::chrono::high_resolution_clock::now();
        bool rv = sort_fn(arr, n);
        auto stop = std::chrono::high_resolution_clock::now();
        if (rv) stats_.duration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
        stats_.size = n;
        stats_.processed = rv;
        return rv;
    }

    virtual bool sort(int arr[], int n = 0) = 0;

private:
    const std::string name_{};
    SortStats         stats_{};
    std::chrono::system_clock::time_point start_{};
    std::chrono::system_clock::time_point stop_{};
};

inline std::ostream &operator<< (std::ostream& os, const ISort::SortStats& stats)
{
    return os << stats.name << " (size=" << stats.size << "): comps="
              << stats.comps << "  swaps=" << stats.swaps << "  duration=" << stats.duration.count();
}
