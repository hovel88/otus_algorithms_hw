#include <chrono>
#include <iomanip>

#include "SpaceArray.h"

static void _test_basic()
{
    std::cout << std::endl;
    std::cout << "===" << std::endl;
    std::cout << "=== проверка базовых операций" << std::endl;
    std::cout << "===" << std::endl;

    SpaceArray<int> space(5, 5, 0);
    std::cout << "инициализированное состояние:" << std::endl;
    space.print_debug();
    
    // Устанавливаем несколько значений
    space.set(0, 0, 1);
    space.set(1, 2, 5);
    space.set(2, 4, 3);
    space.set(4, 4, 7);
    
    std::cout << "установили значения:" << std::endl;
    space.print_debug();
    
    std::cout << "статистика до сброса ячейки (1,2):" << std::endl;
    space.print_stats();
    space.set(1, 2, 0);
    std::cout << "статистика после сброса ячейки (1,2):" << std::endl;
    space.print_stats();
}

static void _test_gray_image()
{
    std::cout << std::endl;
    std::cout << "===" << std::endl;
    std::cout << "=== проверка на картинке оттенков серого" << std::endl;
    std::cout << "===" << std::endl;

    const size_t SIZE = 100;
    const int dark = 255;
    const int white = 0;
    SpaceArray<int> image(SIZE, SIZE, dark);

    int point_X = 50;
    int point_Y = 50;
    int radius = 20;

    auto start = std::chrono::high_resolution_clock::now();
    for (size_t y = 0; y < SIZE; ++y) {
        for (size_t x = 0; x < SIZE; ++x) {
            int dx = x - point_X;
            int dy = y - point_Y;
            if (dx*dx + dy*dy <= radius*radius) {
                image.set(x, y, white);
            }
        }
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "время заполнения (нс): " << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() << std::endl;
    std::cout << "размер для обычного массива: " << image.size() << std::endl;
    std::cout << "статистика для разряженного массива:" << std::endl;
    image.print_stats();
}

int main(int argc, char** argv)
{
    std::cout << "программа оценки работы структуры дынных SpaceArray" << std::endl;

    _test_basic();
    _test_gray_image();

    return 0;
}
