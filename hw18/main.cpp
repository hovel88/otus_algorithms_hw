#include <iostream>
#include <iomanip>
#include <random>
#include <vector>
#include <functional>

#include "StringSearch.h"

struct TestCase {
    std::string name{};
    std::string text{};
    std::string pattern{};
};

std::string generate_random_text_(size_t len, const std::string& abc = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz ")
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, abc.length() - 1);

    std::string result;
    result.reserve(len);
    for (size_t i = 0; i < len; ++i) {
        result += abc[dis(gen)];
    }
    return result;
}

using common_test_func_t = std::function<Stats(const std::string& text, const std::string& pattern)>;
Stats run_with_counter_(const std::string& text, const std::string& pattern, common_test_func_t& search_algo, int runs = 100)
{
    Stats total;
    if (runs > 0) {
        for (int i = 0; i < runs; ++i) {
            auto  start = std::chrono::high_resolution_clock::now();
            Stats stats = search_algo(text, pattern);
            auto  stop  = std::chrono::high_resolution_clock::now();

            total.matches   = stats.matches;
            total.comps    += stats.comps;
            total.duration += std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
        }

        total.comps    /= runs;
        total.duration /= runs;
    }
    return total;
}

int main(int argc, char** argv)
{
    std::cout << "Программа для сравнения алгоритмов поиска подстроки (наивный поиск, алгоритм Бойера-Мура)\n\n";

    std::vector<TestCase> tests;

    tests.push_back({
        "Шаблон отсутствует",
        generate_random_text_(1000, "ABCDEFGHIJKLMNOPQRSTUVWXYZ"),
        "XYZABC"
    });
    tests.push_back({
        "Шаблон в начале (1 вхождение)",
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ" + std::string(1000, 'X'),
        "ABCDEFGH"
    });
    tests.push_back({
        "Повторяющиеся символы (1 вхождение)",
        std::string(1000, 'A') + "B",
        std::string(250, 'A') + "B"
    });
    tests.push_back({
        "Реальный текст (8 вхождений, case-sensitive)",
        "The Grand Canyon, one of the Seven Wonders of the Natural World, is located in the state of Arizona. It is also a UNESCO World Heritage Site. Formed by over 70 million years of erosion from the Colorado River, the Grand Canyon offers a spectacular view. The canyon spans 277 miles in length, up to 18 miles in width, and it measures over a mile in depth at its deepest points. Carlos always wanted to visit the Grand Canyon, and recently he received the chance to hike some of the trails and take several panoramic photographs during his visit.",
        "the"
    });
    tests.push_back({
        "Частые вхождения",
        std::string(5000, 'a') + std::string(5000, 'b') + std::string(5000, 'a'),
        "aaa"
    });

    std::cout << "Замеры времени усреднены по 100 запускам (кроме наивного поиска, он медленный, поэтому по 10)\n\n";
    std::cout << std::left
              << std::setw(20) << "Algo name"
              << std::setw(15) << "Matches"
              << std::setw(20) << "Avg.Comparisons"
              << std::setw(15) << "Avg.Duration"
              << std::setw(25) << "Test case"
              << std::endl;
    std::cout << std::left
              << std::setw(20) << std::string(10, '=')
              << std::setw(15) << std::string(8, '=')
              << std::setw(20) << std::string(16, '=')
              << std::setw(15) << std::string(13, '=')
              << std::setw(25) << std::string(10, '=')
              << std::endl;
    for (const auto& test : tests) {
        {
            FullScan fs;
            common_test_func_t ctf = [&fs](const std::string& text, const std::string& pattern) -> Stats {
                return fs.search(text, pattern);
            };

            Stats stats = run_with_counter_(test.text, test.pattern, ctf, 10);
            std::cout << std::left
                      << std::setw(20) << "FullScan"
                      << std::setw(15) << stats.matches
                      << std::setw(20) << stats.comps
                      << std::setw(15) << std::string(std::to_string(stats.duration.count()) + " ns")
                      << std::setw(25) << test.name
                      << std::endl;
        }

        {
            PrefixFullScan pfs;
            common_test_func_t ctf = [&pfs](const std::string& text, const std::string& pattern) -> Stats {
                return pfs.search(text, pattern);
            };

            Stats stats = run_with_counter_(test.text, test.pattern, ctf, 10);
            std::cout << std::left
                      << std::setw(20) << "PrefixFullScan"
                      << std::setw(15) << stats.matches
                      << std::setw(20) << stats.comps
                      << std::setw(15) << std::string(std::to_string(stats.duration.count()) + " ns")
                      << std::setw(25) << test.name
                      << std::endl;
        }

        {
            SuffixFullScan sfs;
            common_test_func_t ctf = [&sfs](const std::string& text, const std::string& pattern) -> Stats {
                return sfs.search(text, pattern);
            };

            Stats stats = run_with_counter_(test.text, test.pattern, ctf, 10);
            std::cout << std::left
                      << std::setw(20) << "SuffixFullScan"
                      << std::setw(15) << stats.matches
                      << std::setw(20) << stats.comps
                      << std::setw(15) << std::string(std::to_string(stats.duration.count()) + " ns")
                      << std::setw(25) << test.name
                      << std::endl;
        }

        {
            BoyerMoore bm(test.pattern);
            common_test_func_t ctf = [&bm](const std::string& text, const std::string& pattern) -> Stats {
                return bm.search(text, pattern);
            };

            Stats stats = run_with_counter_(test.text, test.pattern, ctf, 100);
            std::cout << std::left
                      << std::setw(20) << "BoyerMoore"
                      << std::setw(15) << stats.matches
                      << std::setw(20) << stats.comps
                      << std::setw(15) << std::string(std::to_string(stats.duration.count()) + " ns")
                      << std::setw(25) << test.name
                      << std::endl;
        }

        {
            BoyerMooreHorspool bmh(test.pattern);
            common_test_func_t ctf = [&bmh](const std::string& text, const std::string& pattern) -> Stats {
                return bmh.search(text, pattern);
            };

            Stats stats = run_with_counter_(test.text, test.pattern, ctf, 100);
            std::cout << std::left
                      << std::setw(20) << "BoyerMooreHorspool"
                      << std::setw(15) << stats.matches
                      << std::setw(20) << stats.comps
                      << std::setw(15) << std::string(std::to_string(stats.duration.count()) + " ns")
                      << std::setw(25) << test.name
                      << std::endl;
        }

        std::cout << "---------------------------------------------------------------------------------------------------------" << std::endl;
    }

    return 0;
}
