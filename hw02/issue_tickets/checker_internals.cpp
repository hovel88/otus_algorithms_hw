#include <iostream>
#include <fstream>
#include <algorithm>
#include <exception>
#include <iomanip>
#include <cctype>

#include "checker_internals.h"

namespace checker {

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

//
//-------------------------------------
//

test_map prepare_tests(const std::string& dir)
{
    const auto in_files  = _list_files(dir, ".in");
    const auto out_files = _list_files(dir, ".out");
    if (in_files.size() != out_files.size()) {
        std::cerr << "Не совпадает количество in/out тестовых файлов в каталоге " << std::quoted(dir) << std::endl;
        return {};
    }

    test_map tm;
    for (size_t i = 0; i != in_files.size(); ++i) {
        const auto& out_elem = out_files.at(i);
        const auto& in_elem = in_files.at(i);
        auto in_pos = in_elem.find(".in");
        if (in_pos == std::string::npos) continue;
        std::string out = in_elem.substr(0, in_pos).append(".out");
        if (out_elem != out) continue;
        tm[i] = std::make_pair(in_elem, out_elem);
    }
    return tm;
}

void run_tests(tested_func fun, const test_map& tm)
{
    size_t success = 0;
    for (const auto& [num, data] : tm) {
        std::cout << "Тест " << num << ": ";
        const auto in  = _read_file(data.first);
        const auto out = _read_file(data.second);
        const auto& in_str  = in.at(0);
        const auto& out_str = out.at(0);
        auto rv = fun(in_str);
        if (rv == out_str) {
            ++success;
            std::cout << "успех" << std::endl;
        } else {
            std::cout << "неудача! ожидалось " << std::quoted(out_str) << " получено " << std::quoted(rv) << std::endl;
        }
        // std::cout << "---- in =" << std::quoted(data.first) << std::endl;
        // std::cout << "---- out=" << std::quoted(data.second) << std::endl;
    }
    std::cout << "------------------"                           << std::endl;
    std::cout << "Количество тестов: " << tm.size()             << std::endl;
    std::cout << "Успешно          : " << success               << std::endl;
    std::cout << "Провалено        : " << (tm.size() - success) << std::endl;
}

} // namespace checker
