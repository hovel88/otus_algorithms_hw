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
    // - для повторяющихся последовательностей  : [специальный_маркер | байт][счетчик]
    // - для неповторяющихся последовательностей: [специальный_маркер][длина][<литералы>]
    std::vector<uint8_t> _compress(const std::vector<uint8_t>& data) const
    {
        std::vector<uint8_t> output;
        if (data.empty()) return output;

        const size_t n = data.size();
        const uint8_t REPEATED_MARKER = 0x80; // маркер для повторяющихся данных
        const uint8_t LITERAL_MARKER  = 0x00; // маркер для литералов

        size_t i = 0;
        while (i < n) {
            // поиск повторяющейся последовательности в некотором окне входных данных.
            // если есть повторяющиеся участки больше чем пару байт, то их выгоднее
            // сжимать.
            // придется хранить 2 байта <символ,число_повторений>, поэтому выгодно
            // сжимать последовательность 3+ байта.
            // а размер окна входных данных - не более 255, потому что uint8_t
            size_t  rep_start = i;
            uint8_t rep_byte  = data[i];
            while ((i < n)
            &&     ((i - rep_start) < 255)
            &&     (data[i] == rep_byte)) {
                i++;
            }
            size_t rep_len = i - rep_start;

            if (rep_len >= 3) {
                // последовательность длинная, выгоднее сжать повторяющиеся данные
                output.push_back(REPEATED_MARKER | rep_byte);
                output.push_back(static_cast<uint8_t>(rep_len));
            } else {
                // это не повторения, или блок повторений не достаточно длинный,
                // ищем блок неповторяющихся данных (длинный повтор, 3+ одинаковых байта)
                size_t literal_start = rep_start;
                i = rep_start;
                while (i < n) {
                    size_t next_rep_len = 1;
                    while ((i + next_rep_len < n)
                    &&     (next_rep_len < 255)
                    &&     (data[i + next_rep_len] == data[i])) {
                        // если движемся по неповторяющейся последовательности, то условие
                        // равенства байт (data[i + next_rep_len] == data[i]) не выполняется
                        //   "ABCXXXXDEF"
                        // literal_start=0
                        // i=0, next_rep_len=1 -> B != A -> (1 меньше 3) -> i++
                        // i=1, next_rep_len=1 -> C != B -> (1 меньше 3) -> i++
                        // i=2, next_rep_len=1 -> X != C -> (1 меньше 3) -> i++
                        // i=3, next_rep_len=1 -> X == X
                        //                     -> в цикле считаем повторы, их 4 подряд (next_rep_len=4)
                        //                     -> (4 >= 3) -> break
                        // итого, блок литералов
                        //   literal_len = i - literal_start = 3 - 0 = 3  : "ABC"
                        next_rep_len++;
                    }

                    if (next_rep_len >= 3) break; // если нашли повтор 3 и более байт - останавливаемся
                    i++;
                    if (i - literal_start >= 255) break;
                }
                size_t literal_len = i - literal_start;

                // особенный случай получается, когда литерал может быть всего одним символом,
                // чтобы для его кодирования него не пришлось выделять 3 байта.
                // тогда закодируем по типу последовательности (аналогично наивному алгоритму).
                // а в общем случае - для кодирования потребуется 4+ байт (2 служебных + 2+ данные)
                if (literal_len == 1
                &&  data[literal_start] != LITERAL_MARKER
                &&  data[literal_start] != REPEATED_MARKER) {
                    output.push_back(REPEATED_MARKER | data[literal_start]);
                    output.push_back(1);
                } else
                if (literal_len > 0) {
                    output.push_back(LITERAL_MARKER);
                    output.push_back(static_cast<uint8_t>(literal_len));
                    for (size_t j = 0; j < literal_len; j++) {
                        output.push_back(data[literal_start + j]);
                    }
                }
            }
        }

        return output;
    }

    // распаковка (улучшенная версия).
    // формат:
    // - для повторяющихся последовательностей  : [специальный_маркер | байт][счетчик]
    // - для неповторяющихся последовательностей: [специальный_маркер][длина][<литералы>]
    std::vector<uint8_t> _decompress(const std::vector<uint8_t>& data) const
    {
        std::vector<uint8_t> output;

        const size_t n = data.size();
        const uint8_t REPEATED_MARKER = 0x80; // маркер для повторяющихся данных
        const uint8_t LITERAL_MARKER  = 0x00; // маркер для литералов

        size_t i = 0;
        while (i < n) {
            uint8_t marker = data[i++];

            if ((marker & REPEATED_MARKER) == REPEATED_MARKER) {
                // начало повторяющихся данных, восстанавливаем символ указанное количество раз
                if (i >= n) break;
                uint8_t rep_byte = marker & ~REPEATED_MARKER;
                uint8_t rep_len  = data[i++];

                for (size_t j = 0; j < rep_len; j++) {
                    output.push_back(rep_byte);
                }
            } else
            if (marker == LITERAL_MARKER) {
                // начало блока литералов (неповторяющейся последовательности)
                if (i >= n) break;
                uint8_t literal_len = data[i++];

                for (size_t j = 0; j < literal_len && i < n; j++) {
                    output.push_back(data[i++]);
                }
            } else {
                output.push_back(marker);
            }
        }

        return output;
    }
};
