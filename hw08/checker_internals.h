#include <chrono>
#include <filesystem>
#include <functional>
#include <memory>
#include <sstream>
#include <iomanip>
#include <vector>
#include <list>
#include <map>
#include <utility>
#include <string.h>

#include "ISort.h"

namespace checker {

using obj_under_test_t = std::shared_ptr<ISort>; // OBJ->sort(int arr[], int n);
typedef struct {
    obj_under_test_t obj_under_test{};
    bool             success{false};
    ISort::SortStats stats{};
} test_output_t;
typedef struct {
    int num;
    std::string in_file;
    std::string out_file;
} test_input_t;
typedef struct {
    test_input_t             data{};
    std::list<test_output_t> result{};
} test_data_t;
using test_list_t = std::list<test_data_t>;
using test_map_t = std::map<std::string, test_list_t>; // group_name, test_list

using common_test_func_t = std::function<void(const std::vector<std::string>& in, const std::vector<std::string>& out, std::list<test_output_t>& items)>; // CTF: common test function

test_list_t prepare_tests(const std::string& dir, std::list<obj_under_test_t> objects);
test_map_t run_tests(const test_map_t& prepared_test_map, common_test_func_t ctf);
void show_table(const test_map_t& result_test_map);

} // namespace checker
