#include <unordered_map>
#include <set>
#include <filesystem>

#include "algo_hll.h"
#include "algo_cms.h"

int main() {
    std::cout << "Программа для демонстрации вероятностных алгоритмов HyperLogLog и Count-Min Sketch";

    // ============================================================

    std::cout << "\n\nзагрузка NASA HTTP Access Logs из файла...\n";
    std::cout << std::string(40, '-') << std::endl;
    const std::string filename = "access.log";
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "ошибка: не могу открыть файл '" << filename << "'\n";
        return 1;
    }

    print_memory_usage("используемая программой память ДО загрузки файла");

    std::vector<std::string> ips;
    ips.reserve(1000000);

    {
        Timer read_timer;
        std::string line;
        while (std::getline(file, line)) {
            if (!line.empty() && line[0] != '#') {
                std::string ip = extract_ip(line);
                if (!ip.empty()) {
                    ips.push_back(ip);
                }
            }
        }
        read_timer.print("  время чтения файла");
        std::cout << "  всего загружено IP-адресов: " << ips.size() << std::endl;
    }

    print_memory_usage("используемая программой память ПОСЛЕ загрузки файла");

    // ============================================================

    // HLL с разными размерами для сравнения
    HyperLogLog hll_small(10);  // m=1024,  теоретическая ошибка должна быть ~3.2%
    HyperLogLog hll_medium(12); // m=4096,  теоретическая ошибка должна быть ~1.6%
    HyperLogLog hll_large(14);  // m=16384, теоретическая ошибка должна быть ~0.8%

    // CMS с разными параметрами
    CountMinSketch cms_fast(3, 1024);     // быстрее, но менее точно
    CountMinSketch cms_accurate(4, 4096); // медленнее, но точнее

    // для небольшой точной выборки
    const size_t SAMPLE_SIZE = 10000000;
    size_t limit = std::min(SAMPLE_SIZE, ips.size());
    std::unordered_map<std::string, int> exact_freq_ips;
    std::set<std::string>                exact_unique_ips;
    size_t                               exact_unique_ips_avg_size = 0;
    size_t                               exact_freq_ips_avg_size = 0;

    // ============================================================

    std::cout << "\n\nзаполнение структур..." << std::endl;
    std::cout << std::string(40, '-') << std::endl;
    std::string descr_str = "  время заполнения ";
    {
        Timer fill_timer;
        for (const auto& ip : ips) {
            hll_small.add(ip);
        }
        fill_timer.print(descr_str + hll_small.name());
    }
    {
        Timer fill_timer;
        for (const auto& ip : ips) {
            hll_medium.add(ip);
        }
        fill_timer.print(descr_str + hll_medium.name());
    }
    {
        Timer fill_timer;
        for (const auto& ip : ips) {
            hll_large.add(ip);
        }
        fill_timer.print(descr_str + hll_large.name());
    }
    {
        Timer fill_timer;
        for (const auto& ip : ips) {
            cms_fast.add(ip);
        }
        fill_timer.print(descr_str + cms_fast.name());
    }
    {
        Timer fill_timer;
        for (const auto& ip : ips) {
            cms_accurate.add(ip);
        }
        fill_timer.print(descr_str + cms_accurate.name());
    }
    descr_str = "  время заполнения выборки из первых " + std::to_string(limit) + " элементов";
    {
        size_t ip_min_size = std::numeric_limits<int>::max();
        size_t ip_max_size = 0;
        size_t ip_avg_size = 0;
        Timer fill_timer;
        for (size_t i = 0; i < limit; i++) {
            exact_freq_ips[ips[i]]++;
            exact_unique_ips.insert(ips[i]);
            ip_min_size = std::min(ip_min_size, ips[i].length());
            ip_max_size = std::max(ip_max_size, ips[i].length());
        }
        fill_timer.print(descr_str);
        ip_avg_size = (ip_max_size > ip_min_size) ? ( (ip_max_size + ip_min_size) / 2 ) : 0;
        if (ip_avg_size > 0) {
            exact_unique_ips_avg_size = exact_unique_ips.size() * (ip_avg_size);
            exact_freq_ips_avg_size   = exact_freq_ips.size() * (ip_avg_size + sizeof(int));
        }
    }

    // ============================================================

    std::cout << "\n\nподсчет через точный словарь..." << std::endl;
    std::cout << std::string(40, '-') << std::endl;
    std::cout << "  в словаре выборка из первых N элементов: " << limit << std::endl;
    std::cout << "  всего уникальных IP-адресов: " << exact_unique_ips.size() << std::endl;
    std::cout << "  память для хранения уникальных IP-адресов: " << (exact_unique_ips_avg_size / 1024.0) << " KB" << std::endl;
    std::cout << "  память для хранения частот IP-адресов: " << (exact_freq_ips_avg_size / 1024.0) << " KB" << std::endl;

    // ============================================================

    std::cout << "\n\nтестирование HyperLogLog..." << std::endl;
    std::cout << std::string(40, '-') << std::endl;
    std::vector<HyperLogLog*> hlls = {
        &hll_small,
        &hll_medium,
        &hll_large
    };
    for (auto& hll : hlls) {
        double estimate = hll->estimate();
        double error = std::abs(estimate - exact_unique_ips.size()) / exact_unique_ips.size() * 100;

        // надо учесть, что оценка точности сама по себе не точна, ведь в нашей
        // выборке точных значений (точный словарь) не все элементы из файла
        std::cout << hll->name() << ":" << std::endl;
        std::cout << "  оценка уникальных IP-адресов: " << std::llround(estimate) << std::endl;
        std::cout << "  ошибка (относительно точного словаря): " << std::fixed << std::setprecision(2) << error << "%" << std::endl;
        std::cout << "  общая память: " << hll->get_memory_usage() / 1024.0 << " KB" << std::endl;
        std::cout << "  экономия памяти (относительно точного словаря): " << (100.0 - (hll->get_memory_usage() * 100.0) / (exact_unique_ips_avg_size)) << "%" << std::endl;
        std::cout << "  статистка:" << std::endl;
        hll->print_stats();
    }

    // ============================================================

    std::cout << "\n\nтестирование Count-Min Sketch..." << std::endl;
    std::cout << std::string(40, '-') << std::endl;
    // протестируем на случайной выборке IP
    std::vector<std::string> samples;
    size_t samples_size = std::min(1000UL, ips.size());
    std::sample(ips.begin(), ips.end(), std::back_inserter(samples), samples_size, std::mt19937{std::random_device{}()});

    std::vector<CountMinSketch*> cmses = {
        &cms_fast,
        &cms_accurate
    };
    for (auto& cms : cmses) {
        double total_error = 0.0;
        int max_error = 0;
        int valid_samples = 0;

        // будем надеться что побольше сэмплов окажется в выборке для точных значений,
        // именно с этими данными мы будем сравнивать и оценивать точность алгоритма.
        // но надо учесть, что оценка точности сама по себе не точна, ведь в нашей
        // выборке точных значений не все элементы из файла
        for (const auto& ip : samples) {
            auto it = exact_freq_ips.find(ip);
            if (it != exact_freq_ips.end()) {
                valid_samples++;
                int real_freq = it->second;
                int cms_freq  = cms->estimate_frequency(ip);
                int error     = cms_freq - real_freq;

                total_error += error;
                max_error = std::max(max_error, error);
            }
        }
        double avg_error = total_error / valid_samples;

        std::cout << cms->name() << ":" << std::endl;
        std::cout << "  средняя ошибка (относительно точного словаря): " << std::fixed << std::setprecision(2) << avg_error << std::endl;
        std::cout << "  максимальная ошибка (относительно точного словаря): " << max_error << std::endl;
        std::cout << "  общая память: " << cms->get_memory_usage() / 1024.0 << " KB" << std::endl;
        std::cout << "  экономия памяти (относительно точного словаря): " << (100.0 - (cms->get_memory_usage() * 100.0) / (exact_freq_ips_avg_size)) << "%" << std::endl;
        std::cout << "  статистка:" << std::endl;
        cms->print_stats();
    }

    // ============================================================

    std::cout << "\nTop-10 частых IP-адресов..." << std::endl;
    // используем более точную версию CMS
    auto top_ips = cms_accurate.get_top_n(ips, 10);
    std::cout << "ранг | IP-адрес ===> оценка частоты" << std::endl;
    std::cout << std::string(40, '-') << std::endl;
    int rank = 1;
    for (const auto& [ip, cms_freq] : top_ips) {
        std::cout << "  "  << std::setw(2) << rank++
                  << " | " << std::setw(25) << ip
                  << " ==> " << cms_freq << std::endl;
    }
    std::cout << "\nпроверка точности (если IP есть в выборке):" << std::endl;
    for (const auto& [ip, cms_freq] : top_ips) {
        auto it = exact_freq_ips.find(ip);
        if (it != exact_freq_ips.end()) {
            int real_freq = it->second;
            int error = cms_freq - real_freq;
            std::cout << "  " << std::setw(25) << ip
                      << ": CMS=" << cms_freq
                      << ", real=" << real_freq
                      << ", error=" << error << std::endl;
        } else {
            std::cout << "  " << std::setw(25) << ip
                      << ": CMS=" << cms_freq
                      << " (нет в выборке)" << std::endl;
        }
    }

    return 0;
}