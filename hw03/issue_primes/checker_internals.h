#include <chrono>
#include <filesystem>
#include <functional>
#include <sstream>
#include <iomanip>
#include <vector>
#include <list>
#include <unordered_map>
#include <utility>

namespace checker {

typedef struct {
    bool success{false};
    std::chrono::nanoseconds elapsed_time;
    std::string fail_details;
} test_result_t;
typedef struct {
    int num;
    std::string in_file;
    std::string out_file;
    test_result_t result;
} test_data_t;
using test_map_t = std::list<test_data_t>;
//                                                                    out                 in1                 in2
using tested_1_args_func = std::function<test_result_t(const std::string&, const std::string&)>;
using tested_2_args_func = std::function<test_result_t(const std::string&, const std::string&, const std::string&)>;

test_map_t prepare_tests(const std::string& dir);
void elapsed_time_table(const std::unordered_map<std::string, test_map_t>& tests);
test_map_t run_tests(tested_1_args_func fun, const test_map_t& test_map);
test_map_t run_tests(tested_2_args_func fun, const test_map_t& test_map);

} // namespace checker
