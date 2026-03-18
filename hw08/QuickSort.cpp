#include "QuickSort.h"

bool QuickSort::sort_v1(int arr[], int n)
{
    if (n < 0) return false;
    if (n > 100000) return false;

    sort_recursive(arr, /*low=*/0, /*high=*/(n - 1));
    return true;
}

int QuickSort::split(int arr[], int low, int high)
{
    // на самом деле тут всё очень сильно зависит от выбора опорного элемента.
    // его можно выбрать как левый (первый) элемент в диапазоне [low, high],
    // его можно выбрать как правый (последний) элемент в диапазоне [low, high],
    // его можно выбрать как средний элемент в диапазоне [low, high],
    // еще какие-то варианты (например, медианный из этих трех выше)...
    // от этого будет зависеть результативность алгоритма для конкретного массива

    int pivot = arr[high]; // выбираем опорным элементов - правый
    int i = low - 1;
    for (int j = low; j <= high; ++j) {
        ISort::inc_comps_cnt();
        if (arr[j] <= pivot) {
            i++;
            if (i != j) {
                ISort::swap(arr, i, j); // XXX: счетчик обменов у нас увеличивается внутри метода
            }
        }
    }
    return i;
}

void QuickSort::sort_recursive(int arr[], int low, int high)
{
    if (low >= high) return;
    // определяем опорный элемент в массиве между границами [low, high],
    // т.о. делим массив на две части и рекурсивно уходим обрабатывать каждую из них
    int pivot = split(arr, low, high);
    sort_recursive(arr, low, (pivot - 1));
    sort_recursive(arr, (pivot + 1), high);
}
