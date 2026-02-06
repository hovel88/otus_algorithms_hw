#include <filesystem>
#include <functional>
#include <vector>
#include <map>
#include <utility>

namespace checker {

using filter_func = std::function<bool(const std::filesystem::path&)>;
using tested_func = std::function<std::string(const std::string&)>;
using test_map    = std::map<int, std::pair<std::string, std::string>>; // number, <in, out>

test_map prepare_tests(const std::string& dir);
void run_tests(tested_func fun, const test_map& tm);

} // namespace checker
