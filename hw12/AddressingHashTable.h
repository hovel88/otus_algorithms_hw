#pragma once

#include <vector>
#include <optional>

#include "IHashTable.h"
#include "Hasher.h"

enum class hash_table_version_t {
    V1 = 1, // реализация с последовательным пробингом
    V2      // реализация с квадратичным пробингом
};

template<typename K, typename V>
class AddressingHashTable : public IHashTable<K, V>
{
private:
    enum class state_t { EMPTY, OCCUPIED, DELETED };

    template<typename SK, typename SV>
    struct slot_t {
        SK      key;
        SV      value;
        state_t state{state_t::EMPTY}; // слоты хранят информацию о статусе

        slot_t() = default;
        slot_t(const K& k, const V& v, state_t s) : key(k), value(v), state(s) {}
        slot_t(K&& k, V&& v, state_t s) : key(std::move(k)), value(std::move(v)), state(s) {}
    };

    static constexpr size_t DEFAULT_CAPACITY = 16;
    static constexpr double DEFAULT_LOAD_FACTOR = 0.7;

    // для структуры данных с прямым доступом по хэш-ключу - используем std::vector
    const hash_table_version_t ver_{hash_table_version_t::V1};
    std::vector<slot_t<K, V>>  table_{};
    Hash<K>                    hasher_{};
    size_t                     elements_{0};
    size_t                     collisions_{0};
    size_t                     rehashes_{0};

public:
    virtual ~AddressingHashTable() = default;
    AddressingHashTable() = delete;
    AddressingHashTable(const AddressingHashTable&) = delete;
    AddressingHashTable(AddressingHashTable&&) = delete;
    AddressingHashTable& operator=(const AddressingHashTable&) = delete;
    AddressingHashTable& operator=(AddressingHashTable&&) = delete;

    explicit AddressingHashTable(hash_table_version_t ver = hash_table_version_t::V1, size_t cap = DEFAULT_CAPACITY)
    :   ver_(ver), table_(get_probe_capacity_(cap))
    {
        if (cap == 0) throw std::invalid_argument("capacity must be > 0");
    }

    void insert(const K& key, const V& value) override {
        if (table_.empty()) rehash_(DEFAULT_CAPACITY);

        size_t hash = hasher_(key);
        size_t N    = table_.size();

        size_t insert_idx = N;
        size_t collisions = 0;
        bool   found_key  = false;
        for (size_t attempt = 0; attempt < N; ++attempt) {
            size_t idx = probe_(hash, attempt, N);
            auto& slot = table_[idx];

            if (slot.state == state_t::OCCUPIED) {
                // если OCCUPIED, проверяем, совпадает ли ключ в таблице, и если да,
                // то просто заменяем значение - это не будем считать за коллизию.
                // но если ключ не совпал - коллизия
                if (slot.key == key) {
                    slot.value = value;
                    return;
                }
                if (!found_key) {
                    found_key = true; // буду считать только один раз коллизию
                    ++collisions;
                }
            } else
            if (slot.state == state_t::EMPTY) {
                // нашли свободный слот, используем его
                insert_idx = idx;
                break;
            } else
            if (slot.state == state_t::DELETED
            &&  insert_idx == N) {
                // запоминаем первый DELETED, но продолжаем искать EMPTY.
                // возможно это улучшит производительность: если мы переиспользуем
                // память в DELETED слотах, тем самым потом потребуется меньше
                // попыток при последующем поиске
                insert_idx = idx;
            }
        }
        if (insert_idx == N) {
            // не нашли место - расширяем таблицу и пробуем снова
            rehash_(table_.size() * 2);
            insert(key, value);
            return;
        }

        collisions_ += collisions;

        auto& slot = table_[insert_idx];
        slot.key   = key;
        slot.value = value;
        slot.state = state_t::OCCUPIED;
        ++elements_;

        // если средняя плотность/загруженность > 0.7 производим расширение таблицы
        if (load_factor() > DEFAULT_LOAD_FACTOR) {
            rehash_(table_.size() * 2);
        }
    }

    void insert(K&& key, V&& value) {
        //
        // ПРИМЕЧАНИЕ: версия с move-семантикой для эффективного rehash
        //
        if (table_.empty()) rehash_(DEFAULT_CAPACITY);

        size_t hash = hasher_(key);
        size_t N    = table_.size();

        size_t insert_idx = N;
        size_t collisions = 0;
        bool   found_key  = false;
        for (size_t attempt = 0; attempt < N; ++attempt) {
            size_t idx = probe_(hash, attempt, N);
            auto& slot = table_[idx];

            if (slot.state == state_t::OCCUPIED) {
                // если OCCUPIED, проверяем, совпадает ли ключ в таблице, и если да,
                // то просто заменяем значение - это не будем считать за коллизию.
                // но если ключ не совпал - коллизия
                if (slot.key == key) {
                    slot.value = std::move(value);
                    return;
                }
                if (!found_key) {
                    found_key = true; // буду считать только один раз коллизию
                    ++collisions;
                }
            } else
            if (slot.state == state_t::EMPTY) {
                insert_idx = idx;
                break;
            } else
            if (slot.state == state_t::DELETED
            &&  insert_idx == N) {
                // запоминаем первый DELETED, но продолжаем искать EMPTY.
                // возможно это улучшит производительность: если мы переиспользуем
                // память в DELETED слотах, тем самым потом потребуется меньше
                // попыток при последующем поиске
                insert_idx = idx;
            }
        }
        if (insert_idx == N) {
            // не нашли место - расширяем таблицу и пробуем снова
            rehash_(table_.size() * 2);
            insert(std::move(key), std::move(value));
            return;
        }

        collisions_ += collisions;

        auto& slot = table_[insert_idx];
        slot.key   = std::move(key);
        slot.value = std::move(value);
        slot.state = state_t::OCCUPIED;
        ++elements_;

        // если средняя плотность/загруженность > 0.7 производим расширение таблицы
        if (load_factor() > DEFAULT_LOAD_FACTOR) {
            rehash_(table_.size() * 2);
        }
    }

    std::optional<V> get(const K& key) const override {
        if (table_.empty()) return std::nullopt;

        size_t hash = hasher_(key);
        size_t N    = table_.size();

        for (size_t attempt = 0; attempt < N; ++attempt) {
            size_t idx = probe_(hash, attempt, N);
            const auto& slot = table_[idx];

            if (slot.state == state_t::EMPTY) {
                return std::nullopt;
            }
            if (slot.state == state_t::OCCUPIED
            &&  slot.key   == key) {
                return slot.value;
            }
        }
        return std::nullopt;
    }

    bool remove(const K& key) override {
        if (table_.empty()) return false;

        size_t hash = hasher_(key);
        size_t N    = table_.size();

        for (size_t attempt = 0; attempt < N; ++attempt) {
            size_t idx = probe_(hash, attempt, N);
            auto& slot = table_[idx];

            if (slot.state == state_t::EMPTY) {
                return false;
            }
            if (slot.state == state_t::OCCUPIED
            &&  slot.key   == key) {
                slot.state = state_t::DELETED;
                --elements_;
                return true;
            }
        }
        return false;
    }

    size_t size() const override { return elements_; }
    bool empty() const override  { return elements_ == 0; }
    void clear() override {
        auto cap = table_.capacity();
        cap = get_probe_capacity_(cap);

        table_.clear();
        table_.resize(cap);
        elements_   = 0;
        collisions_ = 0;
        rehashes_   = 0;
    }
    std::string name() const override {
        switch (ver_) {
        case hash_table_version_t::V1: return "AddressingHashTable (sequential)";
        case hash_table_version_t::V2: return "AddressingHashTable (quadratic)";
        default:                       return "UNKNOWN";
        }
    }

    size_t collision_count() const override { return collisions_; }
    size_t rehashes_count() const override  { return rehashes_; }
    double load_factor() const override     { return table_.empty() ? 0.0 : static_cast<double>(elements_) / table_.size(); } // некая плотность/заполненность таблицы

    void debug_print() const override {
        for (size_t i = 0; i < table_.size(); ++i) {
            std::cout << i << ": ";
            const auto& slot = table_[i];
            if (slot.state == state_t::OCCUPIED) {
                std::cout << "[K='"  << slot.key << "' V='" << slot.value << "']";
            } else
            if (slot.state == state_t::DELETED) {
                std::cout << "DELETED [K='"  << slot.key << "' V='" << slot.value << "']";
            }
            std::cout << std::endl;
        }
    }

private:
    void rehash_(size_t cap) {
        cap = get_probe_capacity_(cap);

        std::vector<slot_t<K, V>> old_table;
        old_table.swap(table_);
        table_.resize(cap);

        elements_   = 0;
        collisions_ = 0;
        ++rehashes_;

        // перевставляем элементы из старой таблицы в новую,
        // чтобы пересчитать все хэш ключи.
        // используем move-семантику для ускорения.
        // все DELETED слоты отбрасываем на этом этапе.
        for (const auto& slot : old_table) {
            if (slot.state == state_t::OCCUPIED) {
                insert(std::move(slot.key), std::move(slot.value));
            }
        }
    }

    // особая версия вставки. без проверки load_factor (для использования в rehash)
    void insert_no_rehash_(const K& key, const V& value) {
        size_t hash = hasher_(key);
        size_t N    = table_.size();

        size_t insert_idx = N;
        size_t collisions = 0;
        bool   found_key  = false;

        for (size_t attempt = 0; attempt < N; ++attempt) {
            size_t idx = probe_(hash, attempt, N);
            auto& slot = table_[idx];

            if (slot.state == state_t::OCCUPIED) {
                if (slot.key == key) {
                    slot.value = value;
                    return;
                }
                if (!found_key) {
                    found_key = true;
                    ++collisions;
                }
            } else
            if (slot.state == state_t::EMPTY) {
                insert_idx = idx;
                break;
            } else
            if (slot.state == state_t::DELETED
            &&  insert_idx == N) {
                insert_idx = idx;
            }
        }

        if (insert_idx == N) {
            throw std::runtime_error("Hash table full during rehash");
        }

        collisions_ += collisions;

        auto& slot = table_[insert_idx];
        slot.key   = key;
        slot.value = value;
        slot.state = state_t::OCCUPIED;
        ++elements_;
    }

    size_t get_probe_capacity_(size_t req) const {
        if (req == 0) req = DEFAULT_CAPACITY;

        if (ver_ == hash_table_version_t::V2) {
            // для квадратичного пробинга используем степень двойки.
            // ближайшая степень двойки бОльшая или равная запрошенной ёмкости
            req-- ;
            req |= req >> 1 ;
            req |= req >> 2 ;
            req |= req >> 4 ;
            req |= req >> 8 ;
            req |= req >> 16 ;
            req |= req >> 32 ;
            req++ ;
            return req;
        }
        // для линейного пробинга любой размер сгодится
        return req;
    }

    size_t probe_(size_t hash, size_t attempt, size_t N) const {
        switch (ver_) {
        case hash_table_version_t::V1: return sequential_probe_(hash, attempt, N);
        case hash_table_version_t::V2: return quadratic_probe_(hash, attempt, N);
        default: break;
        }
        throw std::runtime_error("incorrect HashTable version");
    }

    // последовательный пробинг:
    //      h(k, i) = (hash(k) + i) % N
    size_t sequential_probe_(size_t hash, size_t attempt, size_t N) const {
        return (hash + attempt) % N;
    }

    // квадратичный пробинг:
    //      h(k, i) = (hash(k) + (i + i^2)/2) % N
    size_t quadratic_probe_(size_t hash, size_t attempt, size_t N) const {
        return (hash + (attempt + attempt * attempt) / 2) % N;
    }
};
