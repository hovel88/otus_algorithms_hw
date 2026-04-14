#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <forward_list>
#include <optional>
#include <stdexcept>

#include "IHashTable.h"
#include "Hasher.h"

template<typename K, typename V>
class ChainedHashTable : public IHashTable<K, V>
{
private:
    template<typename SK, typename SV>
    struct slot_t {
        SK key;
        SV value;

        slot_t() = default;
        slot_t(const SK& k, const SV& v) : key(k), value(v) {}
        slot_t(SK&& k, SV&& v) : key(std::move(k)), value(std::move(v)) {}
    };
    using chain_t = std::forward_list<slot_t<K, V>>;

    static constexpr size_t DEFAULT_CAPACITY    = 16;
    static constexpr double DEFAULT_LOAD_FACTOR = 3.0;

    // для структуры данных с прямым доступом по хэш-ключу - используем std::vector.
    // а для реализации односвязного списка, для сохранения нескольких элементов
    // при коллизиях - будем использовать односвязный список std::forward_list
    const double         chain_load_{0};
    std::vector<chain_t> chains_{};
    Hash<K>              hasher_{};
    size_t               elements_{0};
    size_t               collisions_{0};
    size_t               rehashes_{0};

public:
    virtual ~ChainedHashTable() = default;
    ChainedHashTable() : chain_load_(DEFAULT_LOAD_FACTOR), chains_(DEFAULT_CAPACITY) {}
    ChainedHashTable(const ChainedHashTable&) = delete;
    ChainedHashTable(ChainedHashTable&&) = delete;
    ChainedHashTable& operator=(const ChainedHashTable&) = delete;
    ChainedHashTable& operator=(ChainedHashTable&&) = delete;

    explicit ChainedHashTable(double load, size_t cap = DEFAULT_CAPACITY)
    :   chain_load_(load), chains_(cap > 0 ? cap : DEFAULT_CAPACITY)
    {
        if (cap == 0) throw std::invalid_argument("capacity must be > 0");
        if (load <= 0) throw std::invalid_argument("load must be > 0");
    }

    void insert(const K& key, const V& value) override {
        if (chains_.empty()) chains_.resize(DEFAULT_CAPACITY);

        // проверяем, есть ли уже такой ключ в цепочке, и если да,
        // то просто заменяем значение. это не будем считать за коллизию
        size_t idx = get_index_(hasher_(key), chains_.size());
        for (auto& slot : chains_[idx]) {
            if (slot.key == key) {
                slot.value = value;
                return;
            }
        }

        // если в цепочке уже есть элементы - будем считать, что это коллизия
        if (!chains_[idx].empty()) ++collisions_;

        chains_[idx].emplace_front(key, value);
        ++elements_;

        // если средняя длина цепочки больше заданного значения, производим расширение таблицы
        if (load_factor() > chain_load_) {
            rehash_(chains_.size() * 2);
        }
    }

    void insert(K&& key, V&& value) {
        //
        // ПРИМЕЧАНИЕ: версия с move-семантикой для эффективного rehash
        //
        if (chains_.empty()) chains_.resize(DEFAULT_CAPACITY);

        // проверяем, есть ли уже такой ключ в цепочке, и если да,
        // то просто заменяем значение. это не будем считать за коллизию
        size_t idx = get_index_(hasher_(key), chains_.size());
        for (auto& slot : chains_[idx]) {
            if (slot.key == key) {
                slot.value = std::move(value);
                return;
            }
        }

        // если в цепочке уже есть элементы - будем считать, что это коллизия
        if (!chains_[idx].empty()) ++collisions_;

        chains_[idx].emplace_front(std::move(key), std::move(value));
        ++elements_;

        // если средняя длина цепочки больше заданного значения, производим расширение таблицы
        if (load_factor() > chain_load_) {
            rehash_(chains_.size() * 2);
        }
    }

    std::optional<V> get(const K& key) const override {
        if (chains_.empty()) return std::nullopt;

        size_t idx = get_index_(hasher_(key), chains_.size());
        for (const auto& slot : chains_[idx]) {
            if (slot.key == key) {
                return slot.value;
            }
        }
        return std::nullopt;
    }

    bool remove(const K& key) override {
        if (chains_.empty()) return false;

        size_t idx  = get_index_(hasher_(key), chains_.size());
        auto& chain = chains_[idx];

        auto prev = chain.before_begin();
        auto it   = chain.begin();
        while (it != chain.end()) {
            if (it->key == key) {
                chain.erase_after(prev);
                --elements_;
                return true;
            }
            prev = it;
            ++it;
        }
        return false;
    }

    size_t size() const override { return elements_; }
    bool empty() const override  { return elements_ == 0; }
    void clear() override {
        if (chains_.empty()) return;

        for (auto& chain : chains_) {
            chain.clear();
        }
        elements_   = 0;
        collisions_ = 0;
        rehashes_   = 0;
    }
    std::string name() const override {
        return std::string("ChainedHashTable (avg_load=") + std::to_string(int(chain_load_)) + std::string(")");
    }

    size_t collision_count() const override { return collisions_; }
    size_t rehashes_count() const override  { return rehashes_; }
    double load_factor() const override     { return static_cast<double>(elements_) / chains_.size(); } // некая средняя длина цепочки

    void debug_print() const override {
        for (size_t i = 0; i < chains_.size(); ++i) {
            std::cout << i << ": ";
            for (const auto& slot : chains_[i]) {
                std::cout << "[K='" << slot.key << "' V='" << slot.value << "'] => ";
            }
            std::cout << std::endl;
        }
    }

private:
    void rehash_(size_t cap) {
        if (cap == 0) cap = DEFAULT_CAPACITY;

        std::vector<chain_t> old_chains;
        old_chains.swap(chains_);
        chains_.resize(cap);

        elements_   = 0;
        collisions_ = 0;
        ++rehashes_;

        // перевставляем элементы из старой таблицы в новую,
        // чтобы пересчитать все хэш ключи.
        // используем move-семантику для ускорения
        for (auto& chain : old_chains) {
            for (auto& slot : chain) {
                insert(std::move(slot.key), std::move(slot.value));
            }
        }
    }

    size_t get_index_(size_t hash, size_t N) const {
        return ( (N != 0) ? (hash % N) : 0 );
    }
};
