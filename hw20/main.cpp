#include <getopt.h>

#include "common.h"
#include "RLE.h"


void _run_demo(const RLE& rle, const std::string& desc)
{
    std::cout << "=== RLE demo ===" << std::endl;
    std::cout << desc << std::endl;

    {
        std::string test = "AAAAABBBCCCCCCDDD";
        std::vector<uint8_t> data(test.begin(), test.end());
        auto compressed   = rle.compress(data);
        auto decompressed = rle.decompress(compressed);

        std::cout << "\ntest 1: '" << test << "'" << std::endl;
        std::cout << "  original size    : " << data.size() << std::endl;
        std::cout << "  compressed size  : " << compressed.size() << std::endl;
        std::cout << "  compressing ratio: " << (100.0 * compressed.size() / data.size()) << "%" << std::endl;
        std::cout << "  valid            : " << (data == decompressed ? "OK" : "FAIL") << std::endl;
    }

    {
        std::string test =
            "****************\n"
            "****************\n"
            "****        ****\n"
            "****        ****\n"
            "****************\n";
        std::vector<uint8_t> data(test.begin(), test.end());
        auto compressed   = rle.compress(data);
        auto decompressed = rle.decompress(compressed);

        std::cout << "\ntest 2: ASCII-art" << std::endl;
        std::cout << "  original size    : " << data.size() << std::endl;
        std::cout << "  compressed size  : " << compressed.size() << std::endl;
        std::cout << "  compressing ratio: " << (100.0 * compressed.size() / data.size()) << "%" << std::endl;
        std::cout << "  valid            : " << (data == decompressed ? "OK" : "FAIL") << std::endl;
    }

    {
        std::vector<uint8_t> data(1000);
        for (size_t i = 0; i < data.size(); i++) {
            data[i] = rand() % 256;
        }
        auto compressed   = rle.compress(data);
        auto decompressed = rle.decompress(compressed);
        
        std::cout << "\ntest 3: random" << std::endl;
        std::cout << "  original size    : " << data.size() << std::endl;
        std::cout << "  compressed size  : " << compressed.size() << std::endl;
        std::cout << "  compressing ratio: " << (100.0 * compressed.size() / data.size()) << "%" << std::endl;
        std::cout << "  valid            : " << (data == decompressed ? "OK" : "FAIL") << std::endl;
    }

    std::cout << std::endl;
}


void _print_usage(const char* program_name)
{
    std::stringstream ss;

    ss << "usage: " << program_name << " [options] [files...]\n";
    ss << "\noptions:";
    ss << "\n  -c, --compress <file>    Сжать файл (создаст файл с расширением .rle)";
    ss << "\n  -d, --decompress <file>  Распаковать файл (удалить расширение .rle)";
    ss << "\n  -o, --output <file>      Указать выходной файл (для -c или -d)";
    ss << "\n  -b, --bench <files>      Запустить бенчмарк на указанных файлах";
    ss << "\n  -r, --bench-dir <dir>    Бенчмарк для всех файлов в папке";
    ss << "\n  --v1                     Использовать наивную версию алгоритма RLE";
    ss << "\n  --v2                     Использовать улучшенную версию алгоритма RLE (по умолчанию)";
    ss << "\n  --demo                   Запустить демонстрацию работы алгоритма";
    ss << "\n  -h, --help               Показать эту справку\n";

    std::cout << ss.str();
}


int main(int argc, char* argv[])
{
    std::cout << "Программа для демонстрации алгоритма компрессии RLE (Run-Length-Encoding)\n\n";
    if (argc == 1) {
        _print_usage(argv[0]);
        return 0;
    }

    static struct option long_options[] = {
        {"compress",   required_argument, 0, 'c'},
        {"decompress", required_argument, 0, 'd'},
        {"output",     required_argument, 0, 'o'},
        {"bench",      required_argument, 0, 'b'},
        {"bench-dir",  required_argument, 0, 'r'},
        {"v1",         no_argument,       0, 1001},
        {"v2",         no_argument,       0, 1002},
        {"demo",       no_argument,       0, 1000},
        {"help",       no_argument,       0, 'h'},
        {0, 0, 0, 0}
    };

    std::string compress_filepath;
    std::string decompress_filepath;
    std::string output_filepath;
    std::string benchmark_dir;
    std::vector<std::string> benchmark_files;
    bool use_rle_v1 = false;

    int opt = 0;
    int opt_index = 0;
    while ((opt = getopt_long(argc, argv, "c:d:o:b:r:h", long_options, &opt_index)) != -1) {
        switch (opt) {
            case 'c':
                compress_filepath = optarg;
                break;
            case 'd':
                decompress_filepath = optarg;
                break;
            case 'o':
                output_filepath = optarg;
                break;
            case 'r':
                benchmark_dir = optarg;
                break;
            case 'b':
                benchmark_files.push_back(optarg);
                break;
            case 1001:
                use_rle_v1 = true;
                break;
            case 1002:
                use_rle_v1 = false;
                break;
            case 1000:
                {
                    _run_demo(RLE(RLE::version_t::V1), "с использованием наивного алгоритма RLE (для сравнения)");
                    _run_demo(RLE(RLE::version_t::V2), "с использованием стандартного (улучшенного) алгоритма RLE");
                }
                return 0;
            case 'h':
            default:
                _print_usage(argv[0]);
                return 0;
        }
    }

    for (int i = optind; i < argc; ++i) {
        benchmark_files.push_back(argv[i]);
    }

    RLE::version_t ver = use_rle_v1 ? RLE::version_t::V1 : RLE::version_t::V2;

    // режим сжатия
    if (!compress_filepath.empty()) {
        std::string out_file = output_filepath.empty() ?
                               (compress_filepath + ".rle") : (output_filepath);

        std::cout << "compressing: from '" << compress_filepath
                  << "' to '" << out_file
                  << "' using RLE version "
                  << (ver == RLE::version_t::V1 ? "V1 (naive)"
                                                : "V2 (common)")
                  << std::endl;
        _compress_file(compress_filepath, out_file);
        return 0;
    }

    // режим распаковки
    if (!decompress_filepath.empty()) {
        std::string in_file = decompress_filepath;
        std::string out_file = output_filepath;

        if (out_file.empty()) {
            // если через флаги не указали выходной файл, то пытаемся определить его имя
            if (in_file.size() > 4 && in_file.substr(in_file.size() - 4) == ".rle") {
                out_file = in_file.substr(0, in_file.size() - 4);
            } else {
                out_file = in_file + ".decoded";
            }
        }

        std::cout << "decompressing: from '" << decompress_filepath
                  << "' to '" << out_file
                  << "' using RLE version "
                  << (ver == RLE::version_t::V1 ? "V1 (naive)"
                                                : "V2 (common)")
                  << std::endl;
        _decompress_file(in_file, out_file);
        return 0;
    }

    // режим бенчмарка
    if (!benchmark_files.empty() || !benchmark_dir.empty()) {
        std::vector<BenchmarkResult> results;

        // обработка всех файлов из аргументов
        for (const auto& file : benchmark_files) {
            if (std::filesystem::exists(file)
            &&  std::filesystem::is_regular_file(file)) {
                std::cout << "file: '" << file
                          << "' using RLE version "
                          << (ver == RLE::version_t::V1 ? "V1 (naive)"
                                                        : "V2 (common)")
                          << std::endl;
                results.push_back(_benchmark_file(file, ver));
            } else {
                std::cerr << "file '" << file << "' not found, skip" << std::endl;
            }
        }

        // обработка всех фалов в директории
        if (!benchmark_dir.empty()
        &&  std::filesystem::exists(benchmark_dir)
        &&  std::filesystem::is_directory(benchmark_dir)) {
            for (const auto& entry : std::filesystem::recursive_directory_iterator(benchmark_dir)) {
                if (entry.is_regular_file()) {
                    std::string filepath = entry.path().string();

                    std::cout << "file: '" << filepath
                              << "' using RLE version "
                              << (ver == RLE::version_t::V1 ? "V1 (naive)"
                                                            : "V2 (common)")
                              << std::endl;
                    results.push_back(_benchmark_file(filepath, ver));
                }
            }
        }

        if (!results.empty()) {
            _print_benchmark_table(results);
        } else {
            std::cerr << "there are no files to compress/decompress!" << std::endl;
            return 1;
        }

        return 0;
    }

    return 1;
}
