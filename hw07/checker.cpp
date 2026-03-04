#include "checker_internals.h"
#include "BubbleSort.h"
#include "InsertionSort.h"
#include "ShellSort.h"
#include "SelectionSort.h"
#include "HeapSort.h"

int main(int argc, char** argv)
{
    checker::common_test_func_t ctf = [](const std::vector<std::string>& in, const std::vector<std::string>& out, std::list<checker::test_output_t>& items) -> void {
        if (in.size() > 1 && out.size() > 0) {
            const auto& in_count_str         = in.at(0);
            const auto& in_arr_str           = in.at(1);
            const auto& expected_out_arr_str = out.at(0);
            try {
                uint64_t in_count = std::stoull(in_count_str);

                std::vector<int> in_arr;
                {
                    std::stringstream ss(in_arr_str);
                    int number{0};
                    while (ss >> number) {
                        in_arr.push_back(number);
                    }
                }
                std::vector<int> out_arr;
                {
                    std::stringstream ss(expected_out_arr_str);
                    int number{0};
                    while (ss >> number) {
                        out_arr.push_back(number);
                    }
                }

                if (in_count >= 0
                &&  in_count == in_arr.size()
                &&  in_count == out_arr.size()) {
                    int sz = 1;
                    if (in_arr.size() >= 20) {
                        sz = 10;
                    } else
                    if (in_arr.size() < 20 && in_arr.size() > 1) {
                        sz = in_arr.size() / 2;
                    } else {
                        sz = in_arr.size();
                    }
                    if (sz > 2) {
                        std::vector<int> arr_first(in_arr.begin(), in_arr.begin() + sz);
                        std::vector<int> arr_last(in_arr.begin(), in_arr.begin() + sz);
                        std::cout << "   первые и последние " << sz << " элементов массива из " << in_arr.size() << " элементов: \n"
                                << ISort::print_array(arr_first.data(), sz, "   ")
                                << ISort::print_array(arr_last.data(), sz, " ... ") << std::endl;
                    } else {
                        std::cout << "   исходный массив из " << sz << " элементов: \n"
                                << ISort::print_array(in_arr.data(), sz, "   ") << std::endl;
                    }

                    for (auto& item : items) {
                        if (item.obj_under_test) {
                            std::vector<int> arr_copy(in_arr.begin(), in_arr.begin() + in_count);
                            item.obj_under_test->sort(arr_copy.data(), in_count);
                            item.stats   = item.obj_under_test->sort_stats();
                            item.success = (arr_copy == out_arr);
                            std::cout << (item.success ? "  OK  " : "  FAIL  ") << "--> " << item.obj_under_test->sort_stats() << std::endl;
                        }
                    }
                }
            } catch (const std::invalid_argument& e) {
                std::cout << "неверный аргумент: " << e.what() << std::endl;
            } catch (const std::out_of_range& e) {
                std::cout << "значение вне диапазона: " << e.what() << std::endl;
            } catch (const std::exception& e) {
                std::cout << "исключение: " << e.what() << std::endl;
            }
        }
    };

    std::list<checker::obj_under_test_t> tested_algos;
    tested_algos.emplace_back(std::make_shared<BubbleSort>(BubbleSort::V1));
    tested_algos.emplace_back(std::make_shared<BubbleSort>(BubbleSort::V2));
    tested_algos.emplace_back(std::make_shared<InsertionSort>(InsertionSort::V1));
    tested_algos.emplace_back(std::make_shared<InsertionSort>(InsertionSort::V2));
    tested_algos.emplace_back(std::make_shared<InsertionSort>(InsertionSort::V3));
    tested_algos.emplace_back(std::make_shared<ShellSort>(ShellSort::V1));
    tested_algos.emplace_back(std::make_shared<ShellSort>(ShellSort::V2));
    tested_algos.emplace_back(std::make_shared<ShellSort>(ShellSort::V3));
    tested_algos.emplace_back(std::make_shared<SelectionSort>(SelectionSort::V1));
    tested_algos.emplace_back(std::make_shared<HeapSort>(HeapSort::V1));

    checker::test_map_t test_map;
    {
        const auto test_dir = std::filesystem::current_path() / std::filesystem::path("sorting-tests/0.random");
        test_map["0.random"] = checker::prepare_tests(test_dir.string(), tested_algos);
    }
    {
        const auto test_dir = std::filesystem::current_path() / std::filesystem::path("sorting-tests/1.digits");
        test_map["1.digits"] = checker::prepare_tests(test_dir.string(), tested_algos);
    }
    {
        const auto test_dir = std::filesystem::current_path() / std::filesystem::path("sorting-tests/2.sorted");
        test_map["2.sorted"] = checker::prepare_tests(test_dir.string(), tested_algos);
    }
    {
        const auto test_dir = std::filesystem::current_path() / std::filesystem::path("sorting-tests/3.revers");
        test_map["3.revers"] = checker::prepare_tests(test_dir.string(), tested_algos);
    }

    auto results = checker::run_tests(test_map, ctf);
    checker::show_table(results);

    return 0;
}
