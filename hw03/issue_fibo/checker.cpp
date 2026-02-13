#include "checker_internals.h"
#include "algo_recursive.h"
#include "algo_iterative.h"
#include "algo_gold.h"
#include "algo_matrix.h"

using func_under_test_t = std::function<__int128_t(uint64_t N)>;
checker::test_result_t scoped_func(func_under_test_t func_under_test,
                                   const std::string &expected,
                                   const std::string &N_str)
{
        checker::test_result_t res;
        try {
            uint64_t N = std::stoull(N_str);
            if (N >= 0) {
                auto start = std::chrono::high_resolution_clock::now();
                auto R = func_under_test(N);
                auto stop = std::chrono::high_resolution_clock::now();
                res.elapsed_time = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);

                auto R_str = int128_to_string(R);
                if (expected.size() > 25) {
                    res.fail_details.append("получено '").append(R_str).append("'");
                } else {
                    res.fail_details.append("ожидалось '").append(expected).append("' получено '").append(R_str).append("'");
                }

                if (expected != R_str) {
                    res.success = false;
                } else {
                    res.success = true;
                }
            }
        } catch (const std::invalid_argument& e) {
            res.fail_details.append("неверный аргумент: ").append(e.what());
        } catch (const std::out_of_range& e) {
            res.fail_details.append("значение вне диапазона: ").append(e.what());
        }
        return res;
}

int main(int argc, char** argv)
{
    const auto test_dir = std::filesystem::current_path() / std::filesystem::path("4.Fibo");
    const auto tests = checker::prepare_tests(test_dir.string());
    checker::tested_1_args_func func1 = [func_under_test=fibo_algo_recursive](const std::string &expected, const std::string &N_str) -> checker::test_result_t {
        return scoped_func(func_under_test, expected, N_str);
    };
    checker::tested_1_args_func func2 = [func_under_test=fibo_algo_iterative](const std::string &expected, const std::string &N_str) -> checker::test_result_t {
        return scoped_func(func_under_test, expected, N_str);
    };
    checker::tested_1_args_func func3 = [func_under_test=fibo_algo_gold](const std::string &expected, const std::string &N_str) -> checker::test_result_t {
        return scoped_func(func_under_test, expected, N_str);
    };
    checker::tested_1_args_func func4 = [func_under_test=fibo_algo_matrix](const std::string &expected, const std::string &N_str) -> checker::test_result_t {
        return scoped_func(func_under_test, expected, N_str);
    };

    std::unordered_map<std::string, checker::test_map_t> results;
    std::cout << "\nРезультаты теста рекурсивного алгоритма\n" << std::endl;
    results["recursive"] = checker::run_tests(func1, tests);
    std::cout << "\nРезультаты теста итеративного алгоритма\n" << std::endl;
    results["iterative"] = checker::run_tests(func2, tests);
    std::cout << "\nРезультаты теста алгоритма Золотого Сечения\n" << std::endl;
    results["gold"] = checker::run_tests(func3, tests);
    std::cout << "\nРезультаты теста алгоритма матричного перемножения\n" << std::endl;
    results["matrix"] = checker::run_tests(func4, tests);

    std::cout << "\nСравнение затраченного времени на выполнение (нс)\n" << std::endl;
    checker::elapsed_time_table(results);

    return 0;
}
