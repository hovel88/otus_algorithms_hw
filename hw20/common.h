#pragma once

#include <filesystem>
#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <vector>
#include <map>

#include "RLE.h"

std::vector<uint8_t> _read_file(const std::string& filename)
{
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        throw std::runtime_error("can't open file: " + filename);
    }

    size_t size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<uint8_t> data(size);
    file.read(reinterpret_cast<char*>(data.data()), size);

    return data;
}


void _write_file(const std::string& filename, const std::vector<uint8_t>& data)
{
    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("can't create file: " + filename);
    }
    file.write(reinterpret_cast<const char*>(data.data()), data.size());
}


std::string _get_file_type(const std::string& filename)
{
    std::filesystem::path path(filename);
    std::string ext = path.extension().string();
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

    if (ext == ".txt" || ext == ".cpp" || ext == ".h" || ext == ".json" || ext == ".xml") {
        return "text";
    } else if (ext == ".jpg" || ext == ".jpeg" || ext == ".png" || ext == ".bmp") {
        return "image";
    } else if (ext == ".mp3" || ext == ".wav" || ext == ".flac" || ext == ".ogg") {
        return "audio";
    } else if (ext == ".zip" || ext == ".rar" || ext == ".7z" || ext == ".gz") {
        return "archive";
    } else if (ext == ".bin" || ext == ".exe" || ext == ".dll") {
        return "binary";
    }
    return "other";
}


struct BenchmarkResult {
    std::string filename{};
    std::string type{};
    bool        valid{false};
    size_t      original_size{0};
    size_t      compressed_size{0};
    double      compression_ratio{0.0};
    std::chrono::nanoseconds compression_time{};
    std::chrono::nanoseconds decompression_time{};
};

BenchmarkResult _benchmark_file(const std::string& input_file)
{
    BenchmarkResult result;
    result.filename = std::filesystem::path(input_file).filename().string();
    result.type     = _get_file_type(input_file);

    try {
        auto original = _read_file(input_file);
        RLE rle;

        auto start      = std::chrono::high_resolution_clock::now();
        auto compressed = rle.compress(original);
        auto stop       = std::chrono::high_resolution_clock::now();
        result.compression_time = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
        result.compressed_size  = compressed.size();

        start             = std::chrono::high_resolution_clock::now();
        auto decompressed = rle.decompress(compressed);
        stop              = std::chrono::high_resolution_clock::now();
        result.decompression_time = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);

        result.valid         = (original == decompressed);
        result.original_size = original.size();
        if (result.original_size > 0) {
            // коэффициента сжатия (меньше - лучше)
            result.compression_ratio = 100.0 * result.compressed_size / result.original_size;
        } else {
            result.compression_ratio = 100.0;
        }

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        result.valid             = false;
        result.original_size     = 0;
        result.compressed_size   = 0;
        result.compression_ratio = 100.0;
    }

    return result;
}


void _print_benchmark_table(const std::vector<BenchmarkResult>& results)
{
    std::cout << "\n" << std::string(100, '=') << std::endl;
    std::cout << std::left
              << std::setw(25) << "File"
              << std::setw(10) << "Type"
              << std::setw(8)  << "Status"
              << std::setw(15) << "Origin byte(s)"
              << std::setw(15) << "Compr. byte(s)"
              << std::setw(12) << "Ratio %"
              << std::setw(20) << "Compr. time (ns)"
              << std::setw(20) << "Decompr. time (ns)"
              << std::endl;
    std::cout << std::string(100, '-') << std::endl;

    std::map<std::string, std::pair<double, int>> type_stats;
    for (const auto& r : results) {
        std::cout << std::left
                  << std::setw(25) << r.filename.substr(0, 24)
                  << std::setw(10) << r.type
                  << std::setw(8)  << (r.valid ? "OK" : "FAIL")
                  << std::right << std::setw(15) << r.original_size
                  << std::right << std::setw(15) << r.compressed_size
                  << std::right << std::setw(12) << std::fixed << std::setprecision(2) << r.compression_ratio
                  << std::right << std::setw(20) << r.compression_time.count()
                  << std::right << std::setw(20) << r.decompression_time.count()
                  << std::endl;
        type_stats[r.type].first += r.compression_ratio;
        type_stats[r.type].second++;
    }

    if (!type_stats.empty()) {
        std::cout << std::string(100, '-') << std::endl;
        std::cout << "\nAverage compression ration:\n";
        std::cout << std::string(50, '-') << std::endl;

        std::vector<std::pair<std::string, double>> avg_list;
        for (const auto& [type, stats] : type_stats) {
            double avg = stats.first / stats.second;
            avg_list.push_back({type, avg});
        }

        std::sort(avg_list.begin(), avg_list.end(), 
                  [](const auto& a, const auto& b) { return a.second < b.second; });

        for (const auto& [type, avg] : avg_list) {
            std::cout << std::left  << std::setw(15) << type << " : "
                      << std::fixed << std::setprecision(2) << avg << "%";
            if (avg < 70) std::cout << " (+++)";
            else if (avg < 95) std::cout << " (++)";
            else if (avg < 105) std::cout << " (+)";
            else std::cout << " (!!!)";
            std::cout << std::endl;
        }
    }
}


void _compress_file(const std::string& input_file, const std::string& output_file)
{
    std::cout << "compressing: from '" << input_file << "' to '" << output_file << "'" << std::endl;

    try {
        RLE rle;
        auto origin_data = _read_file(input_file);
        auto compressed  = rle.compress(origin_data);
        _write_file(output_file, compressed);

        double ratio = 100.0 * compressed.size() / origin_data.size();
        std::cout << "  size: from " << origin_data.size() << " to " << compressed.size()
                  << " byte(s) (" << std::fixed << std::setprecision(2) << ratio << "%)" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "compressing ERROR: " << e.what() << std::endl;
        throw;
    }
}


void _decompress_file(const std::string& input_file, const std::string& output_file)
{
    std::cout << "decompressing: from '" << input_file << "' to '" << output_file << "'" << std::endl;

    try {
        RLE rle;
        auto compressed   = _read_file(input_file);
        auto decompressed = rle.decompress(compressed);
        _write_file(output_file, decompressed);

        std::cout << "  size: from " << compressed.size() << " to " << decompressed.size()
                  << " byte(s)" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "decompressing ERROR: " << e.what() << std::endl;
        throw;
    }
}
