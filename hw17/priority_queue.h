#pragma once

#include <memory>
#include <stdexcept>
#include <utility>
#include <algorithm>

// реализация приоритетной очереди на бинарной куче (max-heap)

template <typename T, typename Comparator = std::less<T>>
class PriorityQueue
{
private:
    T*         heap_{nullptr}; // динамический массив для кучи
    size_t     cap_{0};
    size_t     size_{0};
    Comparator comp_; // компаратор для определения приоритета

public:
    ~PriorityQueue() {
        delete[] heap_;
    }
    PriorityQueue() = default;
    PriorityQueue(const PriorityQueue& other)
    :   cap_(other.cap_),
        size_(other.size_),
        comp_(other.comp_)
    {
        if (cap_ > 0) {
            heap_ = new T[cap_];
            for (size_t i = 0; i < size_; ++i) {
                heap_[i] = other.heap_[i];
            }
        } else {
            heap_ = nullptr;
        }
    }
    PriorityQueue(PriorityQueue&& other) noexcept
    :   heap_(nullptr),
        cap_(0),
        size_(0) {
        swap(other);
    }

    PriorityQueue& operator=(const PriorityQueue& other) {
        if (this != &other) {
            PriorityQueue temp(other);
            swap(temp); // пользуем концепцию swap, она noexcept
        }
        return *this;
    }
    PriorityQueue& operator=(PriorityQueue&& other) noexcept {
        if (this != &other) {
            swap(other);
        }
        return *this;
    }

    explicit PriorityQueue(const Comparator& comparator)
    :   heap_(nullptr),
        cap_(0),
        size_(0),
        comp_(comparator)
    {}

    void swap(PriorityQueue& other) noexcept {
        std::swap(heap_, other.heap_);
        std::swap(cap_, other.cap_);
        std::swap(size_, other.size_);
        std::swap(comp_, other.comp_);
    }

    void clear() {
        delete[] heap_;
        heap_ = nullptr;
        cap_  = 0;
        size_ = 0;
    }

    bool empty() const  { return size_ == 0; }
    size_t size() const { return size_; }

    const T& top() const {
        if (empty()) throw std::runtime_error("PriorityQueue пуста!");
        // наверху лежат элементы с наивысшим приоритетом.
        return heap_[0];
    }

    void pop() {
        if (empty()) throw std::runtime_error("PriorityQueue пуста!");
        // снимаем с самого верха элемент, для этого просто заменяем
        // его самым последним элементом, и затем достаточно быстро
        // протолкнём этот элемент вниз
        heap_[0] = std::move(heap_[size_ - 1]);
        size_--;
        if (!empty()) {
            sift_down(0);
        }
    }

    void push(const T& value) {
        if (size_ >= cap_) resize();
        // добавляем в конец массива, а затем достаточно быстро протолкнём
        // наверх в нужную позицию
        heap_[size_] = value;
        size_++;
        sift_up(size_ - 1);
    }

    void push(T&& value) {
        if (size_ >= cap_) resize();
        // добавляем в конец массива, а затем достаточно быстро протолкнём
        // наверх в нужную позицию
        heap_[size_] = std::move(value);
        size_++;
        sift_up(size_ - 1);
    }

    void print() const {
        std::cout << "PriorityQueue (sz=" << size_ << ", cap=" << cap_ << "): [";
        for (size_t i = 0; i < size_; ++i) {
            std::cout << heap_[i];
            if (i < (size_ - 1)) std::cout << ", ";
        }
        std::cout << "]\n";
    }

private:
    void resize() {
        size_t new_cap = cap_ == 0 ? 4 : cap_ * 2;
        T* new_heap = new T[new_cap];

        for (size_t i = 0; i < size_; ++i) {
            new_heap[i] = std::move(heap_[i]);
        }

        delete[] heap_;
        heap_ = new_heap;
        cap_  = new_cap;
    }

    // формулы для быстрого поиска родителя элемента и его потомков
    // при реализации на основе структуры хранения в виде бинарного дерева типа Куча
    size_t parent_(size_t index) const      { return (index - 1) / 2; }
    size_t child_left_(size_t index) const  { return 2 * index + 1; }
    size_t child_right_(size_t index) const { return 2 * index + 2; }

    void sift_up(size_t index) {
        // когда добавляем элемент, его нужно пропихнуть вверх кучи
        // до подходящей позиции, для этого меняем местами с родителем
        while (index > 0 && comp_(heap_[parent_(index)], heap_[index])) {
            std::swap(heap_[parent_(index)], heap_[index]);
            index = parent_(index);
        }
    }

    void sift_down(size_t index) {
        // когда удаляем элемент, его нужно пропихнуть вниз кучи,
        // а по факту это значит - поднять правильные дочерние элементы наверх,
        // тогда можно будет спокойно удалить
        size_t max_index = index;
        size_t left = child_left_(index);
        size_t right = child_right_(index);

        if (left < size_ && comp_(heap_[max_index], heap_[left])) {
            max_index = left;
        }
        if (right < size_ && comp_(heap_[max_index], heap_[right])) {
            max_index = right;
        }

        if (index != max_index) {
            std::swap(heap_[index], heap_[max_index]);
            sift_down(max_index);
        }
    }
};

template <typename T, typename Comparator>
void swap(PriorityQueue<T, Comparator>& a, PriorityQueue<T, Comparator>& b) noexcept {
    a.swap(b);
}
