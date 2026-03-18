#include <iostream>
#include <fstream>
#include <algorithm>
#include <exception>
#include <iomanip>
#include <cctype>

#include "checker_internals.h"

namespace checker {

using filter_func = std::function<bool(const std::filesystem::path&)>;

static bool _collect_files(const std::string& dir, std::vector<std::string>& items, filter_func predicate)
{
    try {
        std::error_code ec;
        if (std::filesystem::exists(dir, ec)
        &&  std::filesystem::is_directory(dir, ec)) {
            for (const auto& entry : std::filesystem::directory_iterator(dir)) {
                const auto& entry_path = entry.path();
                if (!std::filesystem::is_regular_file(entry_path, ec)
                &&  !std::filesystem::is_symlink(entry_path, ec)) {
                    continue;
                }

                if (predicate(entry_path)) {
                    items.push_back(entry_path.string());
                }
            }
        }
        std::sort(items.begin(), items.end());
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Не удалось получить список файлов в каталоге " << std::quoted(dir) << ": " << e.what() << std::endl;
    }

    return false;
}

static std::vector<std::string> _read_file(const std::string& filename)
{
    std::vector<std::string> buf;

    try {
        if (std::filesystem::exists(filename)
        &&  std::filesystem::is_regular_file(filename)) {
            std::ifstream ifs(filename, (std::ifstream::in | std::ifstream::binary));
            if (ifs.is_open()) {
                std::string line;
                while (std::getline(ifs, line)) {
                    std::ptrdiff_t first = 0;
                    std::ptrdiff_t last  = static_cast<std::ptrdiff_t>(line.size()) - 1;

                    while (first <= last && !std::isprint(line[first]) && line[first] != ' ') ++first;
                    while (last >= first && !std::isprint(line[last])  && line[last] != ' ')  --last;
                    auto str = std::string(line, first, last - first + 1);

                    buf.push_back(str);
                }
                ifs.close();
                return buf;
            } else {
                std::cerr << "Ошибка открытия файла " << std::quoted(filename) << std::endl;
            }
        } else {
            std::cerr << "Файл " << std::quoted(filename) << " не существует или это не файл" << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Произошло исключение при работе с файлом " << std::quoted(filename) << ": " << e.what() << std::endl;
    }

    return {};
}

static std::vector<std::string> _list_files(const std::string& dir, const std::string& filter)
{
    std::vector<std::string> items;

    bool rv = _collect_files(dir, items, [&filter](const std::filesystem::path& entry) -> bool {
        std::string filename = entry.filename().string();
        if (!filter.empty() && filename.find(filter) == std::string::npos) {
            return false;
        }
        return true;
    });
    if (!rv) return {};

    return items;
}

// static void _show_results(const test_map_t& test_map)
// {
//     size_t total  = test_map.size();
//     size_t ok_cnt = 0;
//     for (const auto& t : test_map) {
//         if (t.result.success) ++ok_cnt;
//         std::cout << "Test " << t.num << " (" << std::setw(12) << t.result.elapsed_time.count() << " нс) : "
//                   << ( t.result.success              ? std::string("успех! ") : std::string("неудача! "))
//                   << ( t.result.fail_details.empty() ? std::string("")        : t.result.fail_details )
//                   << std::endl;
//     }
//     size_t fail_cnt = total - ok_cnt;
//     std::cout << "------------------"               << std::endl;
//     std::cout << "Количество тестов: " << total     << std::endl;
//     std::cout << "Успешно          : " << ok_cnt    << std::endl;
//     std::cout << "Провалено        : " << fail_cnt  << std::endl;
// }

//
//-------------------------------------
//

test_list_t prepare_tests(const std::string& dir, std::list<obj_under_test_t> objects)
{
    const auto in_files  = _list_files(dir, ".in");
    const auto out_files = _list_files(dir, ".out");
    if (in_files.size() != out_files.size()) {
        std::cerr << "Не совпадает количество in/out тестовых файлов в каталоге " << std::quoted(dir) << std::endl;
        return {};
    }

    std::list<test_output_t> empty_test_outputs;
    for (const auto& obj : objects) {
        test_output_t output;
        output.obj_under_test = obj;
        empty_test_outputs.push_back(output);
    }

    test_list_t test_list;
    for (size_t i = 0; i != in_files.size(); ++i) {
        const auto& out_elem = out_files.at(i);
        const auto& in_elem = in_files.at(i);
        auto in_pos = in_elem.rfind(".in");
        if (in_pos == std::string::npos) continue;
        std::string filename = in_elem.substr(0, in_pos);
        auto num_pos = filename.rfind(".");
        if (num_pos == std::string::npos) continue;
        std::string num = filename.substr(num_pos + 1);
        std::string out = filename.append(".out");
        if (out_elem != out) continue;

        test_input_t input_data;
        input_data.num      = std::stol(num);
        input_data.in_file  = in_elem;
        input_data.out_file = out_elem;

        test_data_t test_data;
        test_data.data   = std::move(input_data);
        test_data.result = empty_test_outputs;

        test_list.emplace_back(std::move(test_data));
    }
    std::cerr << "Подготовлено " << test_list.size() << " тестов из каталога " << std::quoted(dir) << std::endl;
    return test_list;
}

test_map_t run_tests(const test_map_t& prepared, common_test_func_t ctf)
{
    test_map_t test_map(prepared);
    for (auto& [group_name, td] : test_map) {
        std::cout << "Запускаем тест группы: " << std::quoted(group_name) << std::endl;
        for (auto& t : td) {
            const auto in  = _read_file(t.data.in_file);
            const auto out = _read_file(t.data.out_file);
            ctf(in, out, t.result);
        }
        std::cout << std::endl;
    }
    return test_map;
}

void show_table(const test_map_t& result_test_map)
{
    for (const auto& [group_name, tl] : result_test_map) {
        std::cout << "----------- [Group name: " << std::quoted(group_name) << "] -----------" << std::endl;
        std::cout << std::left  << std::setw(20) << "Name"
                  << std::right << std::setw(10) << "Test #"
                  << std::right << std::setw(15) << "Processed"
                  << std::right << std::setw(15) << "Array size"
                  << std::right << std::setw(15) << "Comparisons"
                  << std::right << std::setw(15) << "Swaps"
                  << std::right << std::setw(20) << "Duration (ns)"
                  << std::endl;
        for (const auto& item : tl) {
            for (const auto& r : item.result) {
                std::string tn("Test "); tn.append(std::to_string(item.data.num));

                std::cout << std::left << std::setw(20) << r.stats.name
                          << std::right << std::setw(10) << tn
                          << std::right << std::setw(15) << (r.stats.processed ? "processed" : "skipped")
                          << std::right << std::setw(15) << r.stats.size
                          << std::right << std::setw(15) << r.stats.comps
                          << std::right << std::setw(15) << r.stats.swaps
                          << std::right << std::setw(20) << r.stats.duration.count()
                          << std::endl;
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }
}


} // namespace checker
