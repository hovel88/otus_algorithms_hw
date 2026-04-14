#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <list>
#include <chrono>
#include <random>
#include <algorithm>

#include "ChainedHashTable.h"
#include "AddressingHashTable.h"
#include "TrieTable.h"

struct Stats {
    std::string              name{};            // название алгоритма
    std::chrono::nanoseconds time_insert{};     // длительность выполнения вставки
    std::chrono::nanoseconds time_search{};     // длительность выполнения поиска
    std::chrono::nanoseconds time_remove{};     // длительность выполнения удаления
    size_t                   collisions{0};     // количество коллизий при вставке
    size_t                   rehash_cout{0};    // количество проведенных перестроений таблицы
    double                   load_factor{0.0};  // коэффициент загруженности хэш-таблицы
    size_t                   size_insert{0};    // количество элементов для вставки
    size_t                   size_inserted{0};  // количество вставленных элементов
    size_t                   size_search{0};    // количество элементов для поиска
    size_t                   size_searched{0};  // количество найденных элементов
    size_t                   size_remove{0};    // количество элементов для удаления
    size_t                   size_removed{0};   // количество удаленных элементов
    size_t                   size_final{0};     // итоговый размер
};

template<typename K, typename V>
Stats test(IHashTable<K, V>& hash_table,
           const std::vector<std::pair<K, V>>& insert_data,
           const std::vector<K>& search_data,
           const std::vector<K>& remove_data)
{
    Stats stat;

    // тест вставки
    auto start = std::chrono::high_resolution_clock::now();
    for (const auto& [key, val] : insert_data) {
        hash_table.insert(key, val);
    }
    auto end = std::chrono::high_resolution_clock::now();
    stat.time_insert   = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    stat.size_insert   = insert_data.size();
    stat.size_inserted = hash_table.size();
    stat.collisions    = hash_table.collision_count();
    stat.rehash_cout   = hash_table.rehashes_count();
    stat.load_factor   = hash_table.load_factor();

    // тест поиска
    start = std::chrono::high_resolution_clock::now();
    size_t found_count = 0;
    for (const auto& key : search_data) {
        if (hash_table.get(key).has_value()) ++found_count;
    }
    end = std::chrono::high_resolution_clock::now();
    stat.time_search   = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    stat.size_search   = search_data.size();
    stat.size_searched = found_count;

    // тест удаления
    start = std::chrono::high_resolution_clock::now();
    int removed_count = 0;
    for (const auto& key : remove_data) {
        if (hash_table.remove(key)) ++removed_count;
    }
    end = std::chrono::high_resolution_clock::now();
    stat.time_remove  = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    stat.size_remove  = remove_data.size();
    stat.size_removed = removed_count;

    stat.size_final = hash_table.size();
    stat.name       = hash_table.name();
    return stat;
}

void sanity_check()
{
    std::unique_ptr<IHashTable<std::string, std::string>> chain_ht = std::make_unique<ChainedHashTable<std::string, std::string>>(3.0, 7);
    std::cout << "\n"
              << "--- Оценка адекватности " << chain_ht->name() << " ---" << std::endl;
    chain_ht->insert(std::string("k_") + std::to_string(76), std::string("v_") + std::to_string(76));
    chain_ht->insert(std::string("k_") + std::to_string(93), std::string("v_") + std::to_string(93));
    chain_ht->insert(std::string("k_") + std::to_string(40), std::string("v_") + std::to_string(40));
    chain_ht->insert(std::string("k_") + std::to_string(47), std::string("v_") + std::to_string(47));
    chain_ht->insert(std::string("k_") + std::to_string(10), std::string("v_") + std::to_string(10));
    chain_ht->insert(std::string("k_") + std::to_string(55), std::string("v_") + std::to_string(55));
    chain_ht->debug_print();

    std::unique_ptr<IHashTable<std::string, std::string>> open_ht1 = std::make_unique<AddressingHashTable<std::string, std::string>>(hash_table_version_t::V1, 7);
    std::cout << "\n"
              << "--- Оценка адекватности " << open_ht1->name() << " ---" << std::endl;
    open_ht1->insert(std::string("k_") + std::to_string(76), std::string("v_") + std::to_string(76));
    open_ht1->insert(std::string("k_") + std::to_string(93), std::string("v_") + std::to_string(93));
    open_ht1->insert(std::string("k_") + std::to_string(40), std::string("v_") + std::to_string(40));
    open_ht1->insert(std::string("k_") + std::to_string(47), std::string("v_") + std::to_string(47));
    open_ht1->insert(std::string("k_") + std::to_string(10), std::string("v_") + std::to_string(10));
    open_ht1->insert(std::string("k_") + std::to_string(55), std::string("v_") + std::to_string(55));
    open_ht1->debug_print();

    std::unique_ptr<IHashTable<std::string, std::string>> open_ht2 = std::make_unique<AddressingHashTable<std::string, std::string>>(hash_table_version_t::V2, 7);
    std::cout << "\n"
              << "--- Оценка адекватности " << open_ht2->name() << " ---" << std::endl;
    open_ht2->insert(std::string("k_") + std::to_string(76), std::string("v_") + std::to_string(76));
    open_ht2->insert(std::string("k_") + std::to_string(93), std::string("v_") + std::to_string(93));
    open_ht2->insert(std::string("k_") + std::to_string(40), std::string("v_") + std::to_string(40));
    open_ht2->insert(std::string("k_") + std::to_string(47), std::string("v_") + std::to_string(47));
    open_ht2->insert(std::string("k_") + std::to_string(10), std::string("v_") + std::to_string(10));
    open_ht2->insert(std::string("k_") + std::to_string(55), std::string("v_") + std::to_string(55));
    open_ht2->debug_print();
}

std::string random_string(int length, std::mt19937& gen)
{
    static const std::string chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    std::uniform_int_distribution<> dis(0, chars.size() - 1);
    std::string result;
    result.reserve(length);
    for (int i = 0; i < length; ++i) {
        result += chars[dis(gen)];
    }
    return result;
}

int main(int argc, char** argv)
{
    std::cout << "Программа оценки работы алгоритмов префиксного дерева и хэш-таблиц (Trie, ChainedHashTable, AddressingHashTable)" << std::endl;

    sanity_check();

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(1, 100000);

    std::vector<std::pair<std::string, std::string>> insert_data;
    std::vector<std::string>                         search_data;
    std::vector<std::string>                         remove_data;

    const int KEY_LENGTH = 8;
    const int N = 10000;
    for (int i = 0; i < N; ++i) {
        std::string key = random_string(KEY_LENGTH, gen);
        insert_data.emplace_back(key, "value_" + key);
        if (i % 2 == 0) { // сохраним 50% ключей для удаления
            remove_data.push_back(key);
        }
        if (i % 10 == 0) { // сохраним 10% ключей для поиска
            search_data.push_back(key);
        }
    }
    // для теста поиска добавим немного ключей, которых не существует
    for (int i = 0; i < 100; ++i) {
        search_data.push_back(random_string(KEY_LENGTH, gen) + std::string("_fake"));
    }

    std::unique_ptr<IHashTable<std::string, std::string>> chain_ht1 = std::make_unique<ChainedHashTable<std::string, std::string>>(16.0);
    std::unique_ptr<IHashTable<std::string, std::string>> chain_ht2 = std::make_unique<ChainedHashTable<std::string, std::string>>(2.0);
    std::unique_ptr<IHashTable<std::string, std::string>> open_ht1  = std::make_unique<AddressingHashTable<std::string, std::string>>(hash_table_version_t::V1);
    std::unique_ptr<IHashTable<std::string, std::string>> open_ht2  = std::make_unique<AddressingHashTable<std::string, std::string>>(hash_table_version_t::V2);
    std::unique_ptr<IHashTable<std::string, std::string>> trie_tbl  = std::make_unique<TrieTable<std::string>>();

    std::vector<Stats> stats;
    stats.push_back(test(*chain_ht1, insert_data, search_data, remove_data));
    stats.push_back(test(*chain_ht2, insert_data, search_data, remove_data));
    stats.push_back(test(*open_ht1,  insert_data, search_data, remove_data));
    stats.push_back(test(*open_ht2,  insert_data, search_data, remove_data));
    stats.push_back(test(*trie_tbl,  insert_data, search_data, remove_data));


    std::cout << "\n"
              << std::left  << std::setw(33) << "Hash table"
              << std::right << std::setw(25) << "Inserted/Size/Time"
              << std::right << std::setw(12) << "Load"
              << std::right << std::setw(12) << "Collisions"
              << std::right << std::setw(12) << "Rehashes"
              << std::right << std::setw(25) << "Found/Size/Time"
              << std::right << std::setw(25) << "Removed/Size/Time"
              << std::right << std::setw(12) << "FinalSize"
              << std::endl;

    for (const auto& st : stats) {
        std::string insert_str;
        insert_str.append(std::to_string(st.size_inserted)).append("/")
                  .append(std::to_string(st.size_insert)).append("/")
                  .append(std::to_string(st.time_insert.count())).append(" ns");
        std::string search_str;
        search_str.append(std::to_string(st.size_searched)).append("/")
                  .append(std::to_string(st.size_search)).append("/")
                  .append(std::to_string(st.time_search.count())).append(" ns");
        std::string remove_str;
        remove_str.append(std::to_string(st.size_removed)).append("/")
                  .append(std::to_string(st.size_remove)).append("/")
                  .append(std::to_string(st.time_remove.count())).append(" ns");

        std::cout << std::left  << std::setw(33) << st.name
                  << std::right << std::setw(25) << insert_str
                  << std::right << std::setw(12) << st.load_factor
                  << std::right << std::setw(12) << st.collisions
                  << std::right << std::setw(12) << st.rehash_cout
                  << std::right << std::setw(25) << search_str
                  << std::right << std::setw(25) << remove_str
                  << std::right << std::setw(12) << st.size_final
                  << std::endl;
    }

    return 0;
}
