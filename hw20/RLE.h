#pragma once

#include <vector>
#include <cstdint>

class RLE
{
public:
    typedef enum {
        V1 = 0, // наивная (базовая) реализация
        V2      // улучшенная версия с поиском литеральной последовательности и маркерами
    } version_t;

public:
    RLE() : RLE(version_t::V2) {};
    explicit RLE(version_t v)
    :   ver_(v)
    {}

    std::vector<uint8_t> compress(const std::vector<uint8_t>& data) const {
        switch (ver_) {
        case version_t::V2:
            return _compress(data);
        case version_t::V1:
        default:
            return _compress_naive(data);
        }
    }

    std::vector<uint8_t> decompress(const std::vector<uint8_t>& data) const {
        switch (ver_) {
        case version_t::V2:
            return _decompress(data);
        case version_t::V1:
        default:
            return _decompress_naive(data);
        }
    }

private:
    const version_t ver_{V2};

    static const uint8_t REPEATED_MARKER = 0x80; // маркер для повторяющихся данных
    static const uint8_t LITERAL_MARKER  = 0x00; // маркер для литералов
    static const size_t  MAX_REPEATED    = 127;  // максимальная длина повтора
    static const size_t  MAX_LITERAL     = 127;  // максимальная длина литералов

    // упаковка (примитивная).
    // формат:
    // - и для повторяющихся и для неповторяющихся символов: [байт][счетчик]
    std::vector<uint8_t> _compress_naive(const std::vector<uint8_t>& data) const
    {
        std::vector<uint8_t> output;
        if (data.empty()) return output;

        const size_t n = data.size();

        size_t i = 0;
        while (i < n) {
            // начинаем кодировать новую группу байт.
            // считаем сколько раз повторяется этот байт и помещаем в выход
            uint8_t current_byte = data[i];

            size_t count = 1;
            while ((i + count < n)
            &&     (count < 255)
            &&     (data[i + count] == current_byte)) {
                count++;
            }

            output.push_back(current_byte);
            output.push_back(static_cast<uint8_t>(count));

            i += count;
        }

        return output;
    }

    // распаковка (примитивная).
    // формат:
    // - и для повторяющихся и для неповторяющихся символов: [байт][счетчик]
    std::vector<uint8_t> _decompress_naive(const std::vector<uint8_t>& data) const
    {
        std::vector<uint8_t> output;

        const size_t n = data.size();

        size_t i = 0;
        while (i + 1 < n) {
            uint8_t byte = data[i];
            uint8_t count = data[i + 1];

            // восстанавливаем последовательность, столько символов, сколько указано
            for (size_t j = 0; j < count; j++) {
                output.push_back(byte);
            }

            i += 2;
        }

        return output;
    }

    // упаковка (улучшенная версия).
    // формат:
    // - для повторяющихся последовательностей  : [специальный_маркер | длина][байт]
    // - для неповторяющихся последовательностей: [специальный_маркер | длина][<литералы>]
    // под специальным маркером повторяющейся последовательности подразумевается
    // в данном случае старший бит (0x80), соответственно неповторяющейся - 0x00 (бит сброшен).
    // а длина - либо количество повторяющихся байт, либо длина неповторяющейся
    // (литеральной) последовательности. в связи с этим, длина ограничена 127 байтами
    std::vector<uint8_t> _compress(const std::vector<uint8_t>& data) const
    {
        std::vector<uint8_t> output;
        if (data.empty()) return output;

        const size_t n = data.size();
        size_t i = 0;
        while (i < n) {
            // поиск повторяющейся последовательности в некотором окне входных данных.
            // если есть повторяющиеся участки больше чем пару байт, то их выгоднее
            // сжимать.
            // а размер окна входных данных - не более 127, потому что uint8_t и используем бит маркера
            size_t  rep_start = i;
            uint8_t rep_byte  = data[i];
            while ((i < n)
            &&     ((i - rep_start) < MAX_REPEATED)
            &&     (data[i] == rep_byte)) {
                i++;
            }
            size_t rep_len = i - rep_start;

            if (rep_len >= 3) {
                // последовательность длинная (3+ байта), выгоднее сжать повторяющиеся данные
                // [REPEATED_MARKER | rep_len][rep_byte]
                output.push_back(REPEATED_MARKER | static_cast<uint8_t>(rep_len));
                output.push_back(rep_byte);
            } else {
                // это не повторения, или блок повторений не достаточно длинный
                size_t literal_start = rep_start;
                i = rep_start;
                while (i < n) {
                    // проверяем, не начинается ли длинный повтор (3+ одинаковых байта).
                    // если движемся по неповторяющейся последовательности, то условие
                    // равенства байт (data[i + next_rep] == data[i]) не выполняется
                    //   "ABCXXXXDEF"
                    // literal_start=0
                    // i=0, next_rep=1 -> B != A -> (1 меньше 3) -> i++
                    // i=1, next_rep=1 -> C != B -> (1 меньше 3) -> i++
                    // i=2, next_rep=1 -> X != C -> (1 меньше 3) -> i++
                    // i=3, next_rep=1 -> X == X
                    //                     -> в цикле считаем повторы, их 4 подряд (next_rep=4)
                    //                     -> (4 >= 3) -> break
                    // итого, блок литералов
                    //   literal_len = i - literal_start = 3 - 0 = 3  : "ABC"
                    if ((i + 2 < n)
                    &&  (data[i] == data[i+1])
                    &&  (data[i] == data[i+2])) {
                        break;
                    }
                    i++;
                    if (i - literal_start >= MAX_LITERAL) break;
                }
                size_t literal_len = i - literal_start;

                size_t sz   = 0;
                size_t from = 0;
                if (literal_len > 0) {
                    sz   = literal_len;
                    from = literal_start;
                } else {
                    // обработка коротких повторов (до 2 байт) - просто копируем их как литералы
                    i = rep_start + rep_len;
                    sz   = rep_len;
                    from = rep_start;
                }

                // [LITERAL_MARKER | sz][<литералы>]
                output.push_back(LITERAL_MARKER | static_cast<uint8_t>(sz));
                for (size_t j = 0; j < sz; j++) {
                    output.push_back(data[from + j]);
                }
            }
        }

        return output;
    }

    // распаковка (улучшенная версия).
    // формат:
    // - для повторяющихся последовательностей  : [специальный_маркер | длина][байт]
    // - для неповторяющихся последовательностей: [специальный_маркер | длина][<литералы>]
    // под специальным маркером повторяющейся последовательности подразумевается
    // в данном случае старший бит (0x80), соответственно неповторяющейся - 0x00 (бит сброшен).
    // а длина - либо количество повторяющихся байт, либо длина неповторяющейся
    // (литеральной) последовательности. в связи с этим, длина ограничена 127 байтами
    std::vector<uint8_t> _decompress(const std::vector<uint8_t>& data) const
    {
        std::vector<uint8_t> output;

        const size_t n = data.size();
        size_t i = 0;
        while (i < n) {
            uint8_t header = data[i++];

            if (header & REPEATED_MARKER) {
                // начало повторяющихся данных, восстанавливаем символ указанное количество раз
                // [REPEATED_MARKER | rep_len][байт]
                size_t rep_len = header & ~REPEATED_MARKER;
                if (i >= n) break;
                uint8_t rep_byte = data[i++];
                for (size_t j = 0; j < rep_len; j++) {
                    output.push_back(rep_byte);
                }
            } else {
                // [LITERAL_MARKER | rep_len][байт]
                // начало блока литералов (неповторяющейся последовательности)
                uint8_t literal_len = header & ~LITERAL_MARKER;
                for (size_t j = 0; j < literal_len && i < n; j++) {
                    output.push_back(data[i++]);
                }
            }
        }

        return output;
    }
};
