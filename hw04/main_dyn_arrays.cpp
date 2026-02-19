#include <chrono>
#include <iomanip>
#include <memory>
#include <vector>
#include "SingleArray.h"
#include "VectorArray.h"
#include "FactorArray.h"
#include "MatrixArray.h"
#include "ArrayList.h"

static void _test_add_to_end(std::shared_ptr<IArray<int>> arr, size_t n)
{
    std::string err;

    auto start = std::chrono::high_resolution_clock::now();
    try {
        // добавляем в конец
        for (size_t i = 0; i < n; ++i) {
            arr->add(i, arr->count());
        }
    }
    catch (const std::exception& e) {
        err.append(". обработка остановлена, исключение: ").append(e.what());
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);

    std::cout << std::right
        << std::setw(20) << "add to end"
        << std::setw(9)  << n
        << std::setw(9)  << arr->number_of_reallocations()
        << std::setw(12) << arr->capacity()
        << std::setw(15) << duration.count()
        << "  " << arr->name() << err << std::endl;
}

static void _test_add_to_beginning(std::shared_ptr<IArray<int>> arr, size_t n)
{
    std::string err;

    auto start = std::chrono::high_resolution_clock::now();
    try {
        // добавляем в начало
        for (size_t i = 0; i < n; ++i) {
            arr->add(i, 0);
        }
    }
    catch (const std::exception& e) {
        err.append(". обработка остановлена, исключение: ").append(e.what());
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);

    std::cout << std::right
        << std::setw(20) << "add to beginning"
        << std::setw(9)  << n
        << std::setw(9)  << arr->number_of_reallocations()
        << std::setw(12) << arr->capacity()
        << std::setw(15) << duration.count()
        << "  " << arr->name() << err << std::endl;
}

static void _test_add_to_middle(std::shared_ptr<IArray<int>> arr, size_t n)
{
    std::string err;

    auto start = std::chrono::high_resolution_clock::now();
    try {
        // вставляем элементы в середину
        for (size_t i = 0; i < n; ++i) {
            size_t pos = arr->count() / 2;
            arr->add(i, pos);
        }
    }
    catch (const std::exception& e) {
        err.append(". обработка остановлена, исключение: ").append(e.what());
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);

    std::cout << std::right
        << std::setw(20) << "add to middle"
        << std::setw(9)  << n
        << std::setw(9)  << arr->number_of_reallocations()
        << std::setw(12) << arr->capacity()
        << std::setw(15) << duration.count()
        << "  " << arr->name() << err << std::endl;
}

static void _test_random_access(std::shared_ptr<IArray<int>> arr, size_t n)
{
    std::string err;

    int64_t sum = 0;
    auto start = std::chrono::high_resolution_clock::now();
    try {
        // пытаемся получить доступ к элементам
        for (size_t i = 0; i < n; ++i) {
            sum += arr->get(i, -1);
        }
    }
    catch (const std::exception& e) {
        err.append(". обработка остановлена, исключение: ").append(e.what());
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);

    std::cout << std::right
        << std::setw(20) << "random access"
        << std::setw(9)  << n
        << std::setw(9)  << arr->number_of_reallocations()
        << std::setw(12) << arr->capacity()
        << std::setw(15) << duration.count()
        << "  " << arr->name() << " (sum=" << sum << ") " << err << std::endl;
}

static void _test_remove_from_middle(std::shared_ptr<IArray<int>> arr, size_t n)
{
    std::string err;

    auto start = std::chrono::high_resolution_clock::now();
    // удаляем каждый второй элемент начиная с середины
    try {
        for (size_t i = n/2; i < n; i += 2) {
            if (i < arr->count()) {
                arr->remove(i);
            }
        }
    }
    catch (const std::exception& e) {
        err.append(". обработка остановлена, исключение: ").append(e.what());
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);

    std::cout << std::right
        << std::setw(20) << "remove from middle"
        << std::setw(9)  << n
        << std::setw(9)  << arr->number_of_reallocations()
        << std::setw(12) << arr->capacity()
        << std::setw(15) << duration.count()
        << "  " << arr->name() << err << std::endl;
}

int main(int argc, char** argv)
{
    std::cout << "программа сравнения работы динамических массивов при разных размерах N" << std::endl;
    std::cout << "виды массивов: SingleArray, VectorArray, FactorArray, MatrixArray, ArrayList" << std::endl;

    std::vector<size_t> sizes = {10, 100, 1000, 10000, 100000};

    for (int n : sizes) {
        std::cout << std::endl;
        std::cout << std::setw(20) << "type"       << std::setw(9) << "N"       << std::setw(9) << "realloc" << std::setw(12) << "capacity" << std::setw(15) << "duration (ns)" << "  description" << std::endl;
        std::cout << std::setw(20) << "----------" << std::setw(9) << "-------" << std::setw(9) << "-------" << std::setw(12) << "--------" << std::setw(15) << "-------------" << "  -----------" << std::endl;

        {
            auto sa = std::make_shared<SingleArray<int>>(10);
            auto va = std::make_shared<VectorArray<int>>(10);
            auto fa = std::make_shared<FactorArray<int>>(10);
            auto ma = std::make_shared<MatrixArray<int>>(10);
            auto al = std::make_shared<ArrayList<int>>(10);

            _test_add_to_beginning(sa, n);
            _test_add_to_beginning(va, n);
            _test_add_to_beginning(fa, n);
            _test_add_to_beginning(ma, n);
            _test_add_to_beginning(al, n);
            std::cout << std::endl;
            _test_random_access(sa, n);
            _test_random_access(va, n);
            _test_random_access(fa, n);
            _test_random_access(ma, n);
            _test_random_access(al, n);
            std::cout << std::endl;
            _test_remove_from_middle(sa, n);
            _test_remove_from_middle(va, n);
            _test_remove_from_middle(fa, n);
            _test_remove_from_middle(ma, n);
            _test_remove_from_middle(al, n);
        }
        {
            auto sa = std::make_shared<SingleArray<int>>(10);
            auto va = std::make_shared<VectorArray<int>>(10);
            auto fa = std::make_shared<FactorArray<int>>(10);
            auto ma = std::make_shared<MatrixArray<int>>(10);
            auto al = std::make_shared<ArrayList<int>>(10);

            std::cout << std::endl;
            _test_add_to_middle(sa, n);
            _test_add_to_middle(va, n);
            _test_add_to_middle(fa, n);
            _test_add_to_middle(ma, n);
            _test_add_to_middle(al, n);
        }
        {
            auto sa = std::make_shared<SingleArray<int>>(10);
            auto va = std::make_shared<VectorArray<int>>(10);
            auto fa = std::make_shared<FactorArray<int>>(10);
            auto ma = std::make_shared<MatrixArray<int>>(10);
            auto al = std::make_shared<ArrayList<int>>(10);

            std::cout << std::endl;
            _test_add_to_end(sa, n);
            _test_add_to_end(va, n);
            _test_add_to_end(fa, n);
            _test_add_to_end(ma, n);
            _test_add_to_end(al, n);
        }
    }

    return 0;
}
