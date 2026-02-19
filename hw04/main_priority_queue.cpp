#include <chrono>
#include <iomanip>
#include "PriorityQueue.h"

static void _test_basic()
{
    std::cout << std::endl;
    std::cout << "===" << std::endl;
    std::cout << "=== проверка базовых операций" << std::endl;
    std::cout << "===" << std::endl;

    std::cout << "заполнение..." << std::endl;
    PriorityQueue<std::string> queue(5);
    queue.enqueue(1, "low priority");
    queue.enqueue(3, "high priority");
    queue.enqueue(2, "medium priority");
    queue.enqueue(3, "other high priority");
    queue.enqueue(1, "other low priority");

    queue.print_debug();
    std::cout << "извлечение:" << std::endl;
    while (!queue.empty()) {
        std::cout << "  '" << queue.dequeue()<< "'";
    }
    std::cout << std::endl;
}

static void _test_correct_order()
{
    std::cout << std::endl;
    std::cout << "===" << std::endl;
    std::cout << "=== проверка правильной последовательности" << std::endl;
    std::cout << "===" << std::endl;

    std::cout << "заполнение..." << std::endl;
    PriorityQueue<int> queue(8);
    queue.enqueue(5, 50);
    queue.enqueue(1, 10);
    queue.enqueue(3, 30);
    queue.enqueue(5, 51);
    queue.enqueue(2, 20);
    queue.enqueue(4, 40);
    queue.enqueue(3, 31);

    queue.print_stats();
    std::cout << "ожидается: 50, 51, 40, 30, 31, 20, 10" << std::endl;
    std::cout << "извлечено: ";
    while (!queue.empty()) {
        std::cout << queue.dequeue() << ", ";
    }
    std::cout << std::endl;
}

static void _test_fifo_priority()
{
    std::cout << std::endl;
    std::cout << "===" << std::endl;
    std::cout << "=== проверка функции FIFO (одинаковый приоритет)" << std::endl;
    std::cout << "===" << std::endl;

    // добавляем элементы с одинаковым приоритетом
    PriorityQueue<int> queue(5);
    for (int i = 1; i <= 5; ++i) {
        queue.enqueue(1, i * 10);
        std::cout << "помещено в очередь (приоритет 1): " << i * 10 << std::endl;
    }
    queue.print_stats();
    std::cout << "ожидается тот же порядок: 10, 20, 30, 40, 50" << std::endl;
    for (int i = 1; i <= 5; ++i) {
        std::cout << "извлечено из очереди (приоритет 1): " << queue.dequeue() << std::endl;
    }
}

static void _test_stress()
{
    std::cout << std::endl;
    std::cout << "===" << std::endl;
    std::cout << "=== проверка под нагрузкой" << std::endl;
    std::cout << "===" << std::endl;

    const int SIZES[] = {100, 1000, 10000, 100000, 155555};

    for (int size : SIZES) {
        std::cout << "--- size: " << size << " ---" << std::endl;

        PriorityQueue<int> queue(10);
        {
            auto start = std::chrono::high_resolution_clock::now();
            for (int i = 0; i < size; ++i) {
                queue.enqueue(i % 10, i);
            }
            auto end = std::chrono::high_resolution_clock::now();
            std::cout << "время заполнения (нс): " << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() << std::endl;
        }
        queue.print_stats();
        {
            auto start = std::chrono::high_resolution_clock::now();
            while (!queue.empty()) {
                queue.dequeue();
            }
            auto end = std::chrono::high_resolution_clock::now();
            std::cout << "время извлечения (нс): " << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() << std::endl;
        }
        std::cout << std::endl;
    }
}

int main(int argc, char** argv)
{
    std::cout << "программа оценки работы структуры дынных PriorityQueue" << std::endl;

    _test_basic();
    _test_correct_order();
    _test_fifo_priority();
    _test_stress();

    return 0;
}
