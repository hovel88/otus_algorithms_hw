#include "SelectionSort.h"

bool SelectionSort::sort_v1(int arr[], int n)
{
    if (n < 0) return false;
    if (n > 100000) return false;

    // на каждой итерации ищем индекс минимального элемента в неотсортированной части
    // (неотсортированной считается правая часть), и если такой элемент нашли,
    // то меняем местами его с начальным элементом в неотсортированной части
    for (int i = 0; i < (n - 1); ++i) {
        int min = i;
        for (int j = (i + 1); j < n; ++j) {
            ISort::inc_comps_cnt();
            if (arr[j] < arr[min]) {
                min = j;
            }
        }
        if (min != i) {
            ISort::swap(arr, i, min); // XXX: счетчик обменов у нас увеличивается внутри метода
        }
    }
    return true;
}
