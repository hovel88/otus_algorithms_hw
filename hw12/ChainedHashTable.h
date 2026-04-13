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

        slot_t(const SK& k, const SV& v)
        :   key(k), value(v)
        {}
    };
    using chain_t = std::forward_list<slot_t<K, V>>;

    // для структуры данных с прямым доступом по хэш-ключу - используем std::vector.
    // а для реализации односвязного списка, для сохранения нескольких элементов
    // при коллизиях - будем использовать односвязный список std::forward_list
    const double         chain_load_{3.0};
    std::vector<chain_t> chains_{};
    Hash<K>              hasher_{};
    size_t               elements_{0};
    size_t               collisions_{0};
    size_t               rehashes_{0};

public:
    virtual ~ChainedHashTable() = default;
    ChainedHashTable() = default;
    ChainedHashTable(const ChainedHashTable&) = delete;
    ChainedHashTable(ChainedHashTable&&) = delete;
    ChainedHashTable& operator=(const ChainedHashTable&) = delete;
    ChainedHashTable& operator=(ChainedHashTable&&) = delete;

    explicit ChainedHashTable(double load, size_t cap = 16)
    :   chain_load_(load),
        chains_(cap)
    {
        if (cap == 0) throw std::invalid_argument("capacity must be > 0");
    }

    void insert(const K& key, const V& value) override {
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

    std::optional<V> get(const K& key) const override {
        size_t idx = get_index_(hasher_(key), chains_.size());
        for (const auto& slot : chains_[idx]) {
            if (slot.key == key) {
                return slot.value;
            }
        }
        return std::nullopt;
    }

    bool remove(const K& key) override {
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
        // очищаем, но сохраняем ёмкость хэш-таблицы при этом
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
        std::string out;
        int i = 0;
        for (const auto& chain : chains_) {
            std::cout << i++ << ": ";
            if (!chain.empty()) {
                for (const auto& slot : chain) {
                    std::cout << "[K='"  << slot.key << "' V='" << slot.value << "'] => ";
                }
            }
            std::cout << std::endl;
        }
    }

private:
    void rehash_(size_t cap) {
        std::vector<chain_t> old_chains;
        old_chains.swap(chains_);
        chains_.resize(cap);

        elements_   = 0;
        collisions_ = 0;
        ++rehashes_;

        // перевставляем элементы из старой таблицы в новую,
        // чтобы пересчитать все хэш ключи
        for (auto& chain : old_chains) {
            for (auto& slot : chain) {
                insert(slot.key, slot.value);
            }
        }
    }

    size_t get_index_(size_t hash, size_t N) const {
        return hash % N;
    }
};
