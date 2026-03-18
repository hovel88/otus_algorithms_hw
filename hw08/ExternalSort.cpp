#include <map>

#include "ExternalSort.h"
#include "InsertionSort.h"

std::string ExternalSort::_make_temp_filepath(int number)
{
    std::string filename = "temp_" + std::to_string(number);
    switch (ver_) {
    case version_t::V1: filename.append("_v1"); break;
    case version_t::V2: filename.append("_v2"); break;
    default: break;
    }
    filename.append(".txt");
    auto temp_file = temp_dir_ / std::filesystem::path(filename);
    return temp_file.string();
}

bool ExternalSort::sort_v1(const std::string& in_file, const std::string& out_file)
{
    std::map<int, std::ofstream> temp_files;
    try {
        // нам потребуется много вспомогательных файлов, столько, сколько уникальных
        // ключей в исходных данных.
        // на ФАЗА 1 - читаем входной файл и распределяем числа по временным файлам.
        // в каждый временный файл будут попадать элементы с одним и тем же значением
        // и таким образом порядок их будет сохраняться
        // на ФАЗА 2 - объединяем временные файлы в один выходной.
        // поочереди открываем временные файлы и переписываем их содержимое в итоговый файл

        // ФАЗА 1: splitting
        {
            std::ifstream input(in_file);
            int number{0};
            while (input >> number) {
                if (temp_files.find(number) == temp_files.end()) {
                    temp_files.emplace(number, _make_temp_filepath(number));
                }
                temp_files[number] << number << "\n";
            }
            for (auto& [_, file] : temp_files) file.close();
            input.close();
        }

        // ФАЗА 2: merging
        // Примечание: std::map уже сортирует по возрастанию ключей
        {
            std::ofstream output(out_file);
            for (auto& [number, _] : temp_files) {
                const auto file = _make_temp_filepath(number);
                std::ifstream temp_file(file);
                std::string line;
                while (std::getline(temp_file, line)) {
                    output << line << "\n";
                }
                temp_file.close();
                std::filesystem::remove(file); // подчистим за собой
            }
            output.close();
        }
    }
    catch (const std::exception& e) {
        std::cerr << name() << " - исключение при обработке файла " << std::quoted(in_file) << ": " << e.what() << std::endl;
        return false;
    }

    return true;
}

bool ExternalSort::sort_v2(const std::string& in_file, const std::string& out_file, uint64_t segment_size)
{
    try {
        // нам потребуется два вспомогательных файла F1 и F2.
        // отрезок (сегмент), состоящий из одного элемента, упорядочен всегда.
        // на ФАЗА 0 (только один раз) - читаем исходный файл и распределяем
        // по одному элементу (сегмент из N=1) в каждый вспомогательный файл.
        // на ФАЗА 1 - читаем файл с данными (выходной файл, там данные)
        // и распределяем элементы по сегментами в каждый вспомогательный файл.
        // на ФАЗА 2 - в выходной файл F0 сливаем сегменты из вспомогательных
        // файлов F1 и F2, получим в файлах сегменты по N элементов.
        // далее будем повторять в цикле, при этом:
        // - файл F0 используем как входной для следующей итерации;
        // - задаем размер сегмента в 2 раза больше предыдущего шага;
        // - делим сегменты по вспомогательным файлам F1 и F2;
        // - затем снова сливаем сегментами из F1 и F2 в F0.
        // повторяем цикл пока не отсортируем весь входной массив, т.е. размер
        // сегмента должен стать больше количества элементов входных данных
        const auto temp_file1 = _make_temp_filepath(1);
        const auto temp_file2 = _make_temp_filepath(2);
        int number_of_segments = 0;
        uint64_t elements_total = 0;
        bool first_step = true;

        do {
            if (first_step) {
                first_step = false;
                // ФАЗА 0: init
                {
                    elements_total = _get_elements_total(in_file);

                    std::ifstream input(in_file);
                    std::ofstream output1(temp_file1);
                    std::ofstream output2(temp_file2);
                    number_of_segments = _split(segment_size, input, output1, output2);
                    output1.close();
                    output2.close();
                    input.close();
                }
            } else {
                // ФАЗА 1: splitting
                {
                    std::ifstream input(out_file);
                    std::ofstream output1(temp_file1);
                    std::ofstream output2(temp_file2);
                    number_of_segments = _split(segment_size, input, output1, output2);
                    output1.close();
                    output2.close();
                    input.close();
                }
            }
            // ФАЗА 2: merging
            {
                std::ifstream input1(temp_file1);
                std::ifstream input2(temp_file2);
                std::ofstream output(out_file);
                _merge(number_of_segments, segment_size, input1, input2, output);
                input1.close();
                input2.close();
                output.close();
            }
            // std::cout << "elements_total=" << elements_total << ",  segment_size=" << segment_size << ",  number_of_segments=" << number_of_segments << std::endl;
            segment_size <<= 1;
        } while (segment_size <= elements_total);

        std::filesystem::remove(temp_file1); // подчистим за собой
        std::filesystem::remove(temp_file2); // подчистим за собой
    }
    catch (const std::exception& e) {
        std::cerr << name() << " - исключение при обработке файла " << std::quoted(in_file) << ": " << e.what() << std::endl;
        return false;
    }

    return true;
}

bool ExternalSort::sort_v3(const std::string& in_file, const std::string& out_file)
{
    static const int BUFFER_SIZE = 100;

    // тут мы сначала предобрабатываем входной файл: при первом проходе
    // в память загружаются блоки по 100 чисел, сортируются любым другим алгоритмом
    // и отправляются на выход (в дополнительный файл), а потом действуем по алгоритму V2.
    const auto temp_file = _make_temp_filepath(0);
    try {
        {
            std::ifstream input(in_file);
            std::ofstream output(temp_file);

            // будем использовать InsertionSort, массив маленький
            InsertionSort sort_alg(InsertionSort::V3);
            std::vector<int> buffer(BUFFER_SIZE);
            while (input) {
                int count = 0;
                while (count < BUFFER_SIZE && input >> buffer[count]) ++count;
                if (count == 0) break;

                sort_alg.sort(buffer.data(), count);

                for (int i = 0; i < count; ++i) {
                    output << buffer[i] << "\n";
                }
            }

            input.close();
            output.close();
        }
    }
    catch (const std::exception& e) {
        std::cerr << name() << " - исключение при обработке файла " << std::quoted(in_file) << ": " << e.what() << std::endl;
        return false;
    }

    // теперь у нас есть предобработанный массив во временном файле.
    // фактически во временном файле мы уже сформировали серии по 100
    // элементов, правильно отсортированных.
    // этот файл и отправим на сортировку по алгоритму V2.
    // но инициализируем в алгоритме размер сегмента, чтобы не начинать с 1
    bool rv = sort_v2(temp_file, out_file, BUFFER_SIZE);
    std::filesystem::remove(temp_file); // подчистим за собой

    return rv;
}

uint64_t ExternalSort::_get_elements_total(const std::string& in_file)
{
    std::ifstream input(in_file);
    uint64_t total = 0;
    int number = 0;
    while (input >> number) {
        ++total;
    }
    input.close();
    return total;
}

int ExternalSort::_split(uint64_t segment_size, std::ifstream& input, std::ofstream& output1, std::ofstream& output2)
{
    uint64_t number_of_segments = 0;
    std::vector<int> buffer(segment_size);
    bool write_to_output1 = true;
    while (input) {
        uint64_t count = 0;
        while (count < segment_size && input >> buffer[count]) ++count;
        if (count == 0) break;

        // записываем серию в один из двух файлов
        // затем переключаем файл для следующей серии
        auto& output = write_to_output1 ? output1 : output2;
        write_to_output1 = !write_to_output1;
        for (uint64_t i = 0; i < count; ++i) {
            output << buffer[i] << "\n";
        }
        ++number_of_segments;
    }
    return number_of_segments;
}

void ExternalSort::_merge(uint64_t number_of_segments, uint64_t segment_size, std::ifstream& input1, std::ifstream& input2, std::ofstream& output)
{
    for (uint64_t i = 0; i < number_of_segments; ++i) {
        _merge_segment(segment_size, input1, input2, output);
    }
}

void ExternalSort::_merge_segment(uint64_t segment_size, std::ifstream& input1, std::ifstream& input2, std::ofstream& output)
{
    if (segment_size == 0) return;

    uint64_t counter_value_1 = 0;
    uint64_t counter_value_2 = 0;
    int value_1 = 0;
    int value_2 = 0;
    bool has_value_1 = false;
    bool has_value_2 = false;
    auto read_value_1 = [&segment_size, &input = input1, &value = value_1, &has = has_value_1, &counter = counter_value_1]() -> void {
        if (counter < segment_size) {
            if (input >> value) {
                ++counter;
                has = true;
                return;
            }
        }
        value = std::numeric_limits<int>::max();
        has   = false;
    };
    auto read_value_2 = [&segment_size, &input = input2, &value = value_2, &has = has_value_2, &counter = counter_value_2]() -> void {
        if (counter < segment_size) {
            if (input >> value) {
                ++counter;
                has = true;
                return;
            }
        }
        value = std::numeric_limits<int>::max();
        has   = false;
    };

    // извлекаем данные из файлов но не больше размера сегмента
    read_value_1();
    read_value_2();
    while (has_value_1 && has_value_2) {
        if (value_1 < value_2) {
            output << value_1 << "\n";
            read_value_1();
        } else {
            output << value_2 << "\n";
            read_value_2();
        }
    }
    // дозапись оставшихся чисел
    while (has_value_1) {
        output << value_1 << "\n";
        read_value_1();
    }
    while (has_value_2) {
        output << value_2 << "\n";
        read_value_2();
    }
}
