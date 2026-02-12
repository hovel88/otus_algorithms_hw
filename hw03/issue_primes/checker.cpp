#include "checker_internals.h"
#include "algo_base.h"
#include "algo_sqrt.h"
#include "algo_odd.h"
#include "algo_sqrt_div_2_3.h"
#include "algo_div_primes.h"
#include "algo_eratosthenes.h"

using func_under_test_t = std::function<uint64_t(uint64_t N)>;
checker::test_result_t scoped_func(func_under_test_t func_under_test,
                                   const std::string &expected,
                                   const std::string &N_str)
{
        checker::test_result_t res;
        try {
            uint64_t E = std::stoull(expected);
            uint64_t N = std::stoull(N_str);
            if (N >= 0) {
                auto start = std::chrono::high_resolution_clock::now();
                uint64_t R = func_under_test(N);
                auto stop = std::chrono::high_resolution_clock::now();
                res.elapsed_time = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);

                res.fail_details.append("ожидалось '").append(expected).append("' получено '").append(std::to_string(R)).append("'");

                if (E != R) {
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
    const auto test_dir = std::filesystem::current_path() / std::filesystem::path("5.Primes");
    const auto tests = checker::prepare_tests(test_dir.string());
    checker::tested_1_args_func func1 = [func_under_test=primes_algo_base](const std::string &expected, const std::string &N_str) -> checker::test_result_t {
        return scoped_func(func_under_test, expected, N_str);
    };
    checker::tested_1_args_func func2 = [func_under_test=primes_algo_sqrt](const std::string &expected, const std::string &N_str) -> checker::test_result_t {
        return scoped_func(func_under_test, expected, N_str);
    };
    checker::tested_1_args_func func3 = [func_under_test=primes_algo_odd](const std::string &expected, const std::string &N_str) -> checker::test_result_t {
        return scoped_func(func_under_test, expected, N_str);
    };
    checker::tested_1_args_func func4 = [func_under_test=primes_algo_sqrt_div_2_3](const std::string &expected, const std::string &N_str) -> checker::test_result_t {
        return scoped_func(func_under_test, expected, N_str);
    };
    checker::tested_1_args_func func5 = [func_under_test=primes_algo_div_primes](const std::string &expected, const std::string &N_str) -> checker::test_result_t {
        return scoped_func(func_under_test, expected, N_str);
    };
    checker::tested_1_args_func func6 = [func_under_test=primes_algo_eratosthenes](const std::string &expected, const std::string &N_str) -> checker::test_result_t {
        return scoped_func(func_under_test, expected, N_str);
    };

    std::unordered_map<std::string, checker::test_map_t> results;
    std::cout << "\nРезультаты теста алгоритма через перебор делителей\n" << std::endl;
    results["base"] = checker::run_tests(func1, tests);
    std::cout << "\nРезультаты теста алгоритма через перебор делителей (оптимизация с квадратным корнем)\n" << std::endl;
    results["sqrt"] = checker::run_tests(func2, tests);
    std::cout << "\nРезультаты теста алгоритма через перебор делителей (оптимизация с поиском по нечетным)\n" << std::endl;
    results["odd"] = checker::run_tests(func3, tests);
    std::cout << "\nРезультаты теста алгоритма через перебор делителей (оптимизация с квадратным корнем исключая кратные 2 и 3)\n" << std::endl;
    results["sqrt div 2,3"] = checker::run_tests(func4, tests);
    std::cout << "\nРезультаты теста алгоритма через перебор делителей (оптимизация с делением на простые числа и квадратным корнем)\n" << std::endl;
    results["div primes"] = checker::run_tests(func5, tests);
    std::cout << "\nРезультаты теста алгоритма через решето Эрастофена\n" << std::endl;
    results["div primes"] = checker::run_tests(func6, tests);

    std::cout << "\nСравнение затраченного времени на выполнение (нс)\n" << std::endl;
    checker::elapsed_time_table(results);

    return 0;
}
