#pragma once

#include <optional>
#include <string>

template<typename K, typename V>
class IHashTable {
public:
    virtual ~IHashTable() = default;

    virtual void insert(const K& key, const V& value) = 0;
    virtual std::optional<V> get(const K& key) const = 0;
    virtual bool remove(const K& key) = 0;

    virtual size_t size() const = 0;
    virtual bool empty() const = 0;
    virtual void clear() = 0;
    virtual std::string name() const = 0;

    virtual size_t collision_count() const = 0;
    virtual size_t rehashes_count() const = 0;
    virtual double load_factor() const = 0;

    virtual void debug_print() const = 0;
};
