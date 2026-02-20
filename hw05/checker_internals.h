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
    std::chrono::nanoseconds elapsed_time{0};
    std::string fail_details;
} test_result_t;
typedef struct {
    int num;
    std::string in_file;
    std::string out_file;
    test_result_t result;
} test_data_t;
using test_map_t = std::list<test_data_t>;

using tested_1in_1out_func = std::function<test_result_t(const std::string& out1, const std::string& in1)>;
using tested_1in_2out_func = std::function<test_result_t(const std::string& out1, const std::string& out2, const std::string& in1)>;
// using tested_2in_1out_func = std::function<test_result_t(const std::string& out1, const std::string& in1, const std::string& in2)>;

test_map_t prepare_tests(const std::string& dir);
void elapsed_time_table(const std::unordered_map<std::string, test_map_t>& tests);
test_map_t run_tests(tested_1in_1out_func fun, const test_map_t& test_map);
test_map_t run_tests(tested_1in_2out_func fun, const test_map_t& test_map);
// test_map_t run_tests(tested_2in_1out_func fun, const test_map_t& test_map);

} // namespace checker
