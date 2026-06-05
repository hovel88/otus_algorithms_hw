#include <iostream>

#include "checker_internals.h"
#include "algo.h"

using func_under_test_t = std::function<std::string(const std::string& in)>;
checker::test_result_t scoped_func(func_under_test_t func_under_test,
                                   const std::string& expected,
                                   const std::vector<std::string>& input)
{
        checker::test_result_t res;
        try {
            const auto& N_str = input.front();

            auto start = std::chrono::high_resolution_clock::now();
            auto result = func_under_test(N_str);
            auto stop = std::chrono::high_resolution_clock::now();
            res.elapsed_time = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);

            res.fail_details.append("ожидалось '").append(expected).append("' получено '").append(result).append("'");
            res.success = (expected != result) ? false : true;
        } catch (const std::invalid_argument& e) {
            res.fail_details.append("неверный аргумент: ").append(e.what());
        } catch (const std::out_of_range& e) {
            res.fail_details.append("значение вне диапазона: ").append(e.what());
        } catch (const std::exception& e) {
            res.fail_details.append(e.what());
        }
        return res;
}

int main(int argc, char** argv)
{
    const auto test_dir = std::filesystem::current_path() / std::filesystem::path("test_data");
    const auto tests = checker::prepare_tests(test_dir.string());
    checker::tested_func func = [func_under_test=algo](const std::string& expected, const std::vector<std::string>& input) -> checker::test_result_t {
        return scoped_func(func_under_test, expected, input);
    };

    std::cout << "\nРезультаты теста алгоритма\n" << std::endl;
    checker::run_tests(func, tests);

    return 0;
}
