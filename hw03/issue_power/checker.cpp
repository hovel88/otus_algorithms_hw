#include "checker_internals.h"
#include "algo_iterative.h"
#include "algo_div_power.h"
#include "algo_binary_power.h"

using func_under_test_t = std::function<double(double A, int64_t N)>;
checker::test_result_t scoped_func(func_under_test_t func_under_test,
                                   const std::string &expected,
                                   const std::string &A_str,
                                   const std::string &N_str)
{
        checker::test_result_t res;
        try {
            double  E = std::stod(expected);
            double  A = std::stod(A_str);
            int64_t N = std::stoll(N_str);
            if (N >= 0) {
                auto start = std::chrono::high_resolution_clock::now();
                double R = func_under_test(A, N);
                auto stop = std::chrono::high_resolution_clock::now();
                res.elapsed_time = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);

                std::stringstream E_ss;
                E_ss << std::fixed << std::setprecision(11) << E;
                std::string E_str = E_ss.str();
                std::stringstream R_ss;
                R_ss << std::fixed << std::setprecision(11) << R;
                std::string R_str = R_ss.str();

                res.fail_details.append("ожидалось '").append(expected).append("' получено '").append(R_str).append("'");

                if (E_str != R_str) {
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
    const auto test_dir = std::filesystem::current_path() / std::filesystem::path("3.Power");
    const auto tests = checker::prepare_tests(test_dir.string());
    checker::tested_2_args_func func1 = [func_under_test=power_algo_iterative](const std::string &expected, const std::string &A_str, const std::string &N_str) -> checker::test_result_t {
        return scoped_func(func_under_test, expected, A_str, N_str);
    };
    checker::tested_2_args_func func2 = [func_under_test=power_algo_div_power](const std::string &expected, const std::string &A_str, const std::string &N_str) -> checker::test_result_t {
        return scoped_func(func_under_test, expected, A_str, N_str);
    };
    checker::tested_2_args_func func3 = [func_under_test=power_algo_binary_power](const std::string &expected, const std::string &A_str, const std::string &N_str) -> checker::test_result_t {
        return scoped_func(func_under_test, expected, A_str, N_str);
    };

    std::map<std::string, checker::test_map_t> results;
    std::cout << "\nРезультаты теста алгоритма через итеративный подход\n" << std::endl;
    results["iterative"] = checker::run_tests(func1, tests);
    std::cout << "\nРезультаты теста алгоритма через деление степени на четные и нечетные с домножением\n" << std::endl;
    results["div power"] = checker::run_tests(func2, tests);
    std::cout << "\nРезультаты теста алгоритма через бинарное возведение в степень через маску\n" << std::endl;
    results["binary power"] = checker::run_tests(func3, tests);

    std::cout << "\nСравнение затраченного времени на выполнение (нс)\n" << std::endl;
    checker::elapsed_time_table(results);

    return 0;
}
