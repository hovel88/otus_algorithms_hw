#include <iomanip>
#include <list>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "SelectionSort.h"
#include "HeapSort.h"

static ISort::SortStats _test_func(const int arr[], int n, ISort* sort_algo)
{
    int bytes = n * sizeof(int);
    int arr_copy[n] = {0};
    memcpy(arr_copy, arr, bytes);

    sort_algo->sort(arr_copy, n);
    // std::string dbg_str = sort_algo->name() + std::string(" out: ");
    // std::cout << sort_algo->print_array(arr_copy, n, dbg_str) << std::endl;
    return sort_algo->sort_stats();
}

int main(int argc, char** argv)
{
    std::cout << "программа оценки работы алгоритмов сортировки (SelectionSort, HeapSort) в базовых версиях" << std::endl;

    const int sizes[] = {100, 1000, 10000, 100000};
    // const int sizes[] = {40};
    int max_size = 0;
    for (const auto sz : sizes) {
        if (sz > max_size) max_size = sz;
    }
    int arr[max_size] = {0};
    srand(12345);
    for (int i = 0; i < max_size; ++i) {
        arr[i] = rand() % 100;
    }
    {
        int sz = 20;
        int arr_first[sz] = {0};
        int arr_last[sz] = {0};
        int bytes = sz * sizeof(int);
        memcpy(arr_first, arr, bytes);
        memcpy(arr_last, arr + (max_size - sz), bytes);
        std::cout << "Первые и последние " << sz << " элементов массива из " << max_size << " элементов: \n"
                  << ISort::print_array(arr_first, sz, "   ")
                  << ISort::print_array(arr_last, sz, " ... ") << std::endl;
    }

    std::list<ISort::SortStats> results;
    for (const auto sz : sizes) {
        {
            SelectionSort sort_algo(SelectionSort::V1);
            results.emplace_back(_test_func(arr, sz, &sort_algo));
        }
        {
            HeapSort sort_algo(HeapSort::V1);
            results.emplace_back(_test_func(arr, sz, &sort_algo));
        }
    }

    std::cout << std::left  << std::setw(20) << "Name"
              << std::right << std::setw(15) << "Array size"
              << std::right << std::setw(15) << "Comparisons"
              << std::right << std::setw(15) << "Swaps"
              << std::right << std::setw(20) << "Duration (ns)"
              << std::endl;
    for (const auto& res : results) {
        std::cout << std::left  << std::setw(20) << res.name
                  << std::right << std::setw(15) << res.size
                  << std::right << std::setw(15) << res.comps
                  << std::right << std::setw(15) << res.swaps
                  << std::right << std::setw(20) << res.duration.count()
                  << std::endl;
    }

    return 0;
}
