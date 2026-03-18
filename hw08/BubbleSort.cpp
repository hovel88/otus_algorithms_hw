#include "BubbleSort.h"

bool BubbleSort::sort_v1(int arr[], int n)
{
    if (n < 0) return false;
    if (n > 100000) return false;

    // наименьший элемент всплывает влево.
    // идём с конца массива в начало, сравниваем пары (j-1, j) и меняем, если j < j-1
    for (int i = 0; i < (n - 1); ++i) {
        for (int j = (n - 1); j > i; --j) {
            ISort::inc_comps_cnt();
            if (arr[j] < arr[j - 1]) {
                ISort::swap(arr, j, (j - 1)); // XXX: счетчик обменов у нас увеличивается внутри метода
            }
        }
    }
    return true;
}

bool BubbleSort::sort_v2(int arr[], int n)
{
    if (n < 0) return false;
    if (n > 100000) return false;

    // наименьший элемент всплывает влево.
    // идём с конца массива в начало, сравниваем пары (j-1, j) и меняем, если j < j-1.
    // но в этот раз учитываем, были ли операции обмена за одну итерацию
    for (int i = 0; i < (n - 1); ++i) {
        bool swapped = false;
        for (int j = (n - 1); j > i; --j) {
            ISort::inc_comps_cnt();
            if (arr[j] < arr[j - 1]) {
                swapped = true;
                ISort::swap(arr, j, (j - 1)); // XXX: счетчик обменов у нас увеличивается внутри метода
            }
        }

        ISort::inc_comps_cnt();
        if (!swapped) {
            break;
        }
    }
    return true;
}
