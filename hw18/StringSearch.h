#pragma once

#include <string>
#include <chrono>
#include <vector>
#include <array>
#include <algorithm>

struct Stats {
    size_t                   matches{0};  // количество найденных вхождений
    int64_t                  comps{0};    // количество сравнений символов
    std::chrono::nanoseconds duration{0}; // длительность выполнения
};


class FullScan
{
public:
    Stats search(const std::string& text, const std::string& pattern) {
        Stats stats;

        if (pattern.empty()) return stats;

        const size_t T = text.length();
        const size_t P = pattern.length();
        if (P > T) return stats;

        // прикладываем шаблон к тексту слева направо, последовательно смещая на символ, пока шаблон не упрётся в конец текста
        // и внутри шаблона проверяем символы, последовательно их перебирая
        for (size_t text_offset = 0; text_offset <= (T - P); ++text_offset) {
            size_t pattern_offset = 0;
            for (pattern_offset = 0; pattern_offset < P; ++pattern_offset) {
                stats.comps++;
                if (text[text_offset + pattern_offset] != pattern[pattern_offset]) {
                    break;
                }
            }
            if (pattern_offset == P) {
                stats.matches++;
            }
        }

        return stats;
    }
};


class PrefixFullScan
{
public:
    Stats search(const std::string& text, const std::string& pattern) {
        Stats stats;

        if (pattern.empty()) return stats;

        const size_t T = text.length();
        const size_t P = pattern.length();
        if (P > T) return stats;

        // оптимизация через префиксы.
        // при несовпадении сдвигаем шаблон так, чтобы уже совпавший префикс остался на месте

        size_t text_offset = 0;
        size_t pattern_offset = 0;
        while (text_offset <= (T - P)) {
            // cравниваем с текущей позиции
            for (; pattern_offset < P; ++pattern_offset) {
                stats.comps++;
                if (text[text_offset + pattern_offset] != pattern[pattern_offset]) {
                    break;
                }
            }

            if (pattern_offset == P) {
                // нашли вхождение
                stats.matches++;
            }

            if (pattern_offset == 0) {
                // не было никакого совпадения, просто двигаемся дальше
                ++text_offset;
                pattern_offset = 0;
            } else {
                // используем уже совпавший префикс.
                // находим длину наибольшего совпадающего префикса, который является суффиксом уже проверенной части шаблона.
                // идея такая:
                //   текст:    ABABABAC
                //   шаблон:   ABABAC
                //             |||||
                //   совпало 5 символов ("ABABA"), затем несовпадение в символе 'C'
                //   префикс "ABA" (первые 3 символа) совпадает с суффиксом "ABA" (последние 3 символа совпавшей части),
                //   и при этом у нас есть перехлёст на один символ 'A'. поэтому сдвигаем шаблон на 2 позиции (5 - 3),
                //   а не на 1, как было бы при обычном полном переборе
                size_t overlap = 0;
                for (size_t k = 1; k < pattern_offset; ++k) {
                    bool match = true;
                    for (size_t t = 0; t < k; ++t) {
                        if (pattern[t] != pattern[pattern_offset - k + t]) {
                            match = false;
                            break;
                        }
                    }
                    if (match) overlap = k;
                }

                // сдвигаем текст на (pattern_offset - overlap) позиций,
                // а указатель в шаблоне на overlap
                text_offset += (pattern_offset - overlap);
                pattern_offset = overlap;
            }
        }

        return stats;
    }
};


class SuffixFullScan
{
public:
    Stats search(const std::string& text, const std::string& pattern) {
        Stats stats;

        if (pattern.empty()) return stats;

        const int T = text.length();
        const int P = pattern.length();
        if (P > T) return stats;

        // оптимизация через суффиксы.
        // при несовпадении ищем последнее вхождение "проблемного" символа в шаблоне

        // для удобства предварительно строим таблицу последних вхождений для каждого символа
        std::array<int, 256> last_occurrence;
        last_occurrence.fill(-1);
        for (int i = 0; i < P; ++i) {
            last_occurrence[static_cast<unsigned char>(pattern[i])] = i;
        }

        // в процессе поиска шаблон накладывается на строку слева направо,
        // но само сравнение символов идет с конца шаблона к его началу
        int text_offset = 0;
        while (text_offset <= (T - P)) {
            int pattern_offset = P - 1;

            // сравниваем справа налево
            while (pattern_offset >= 0) {
                stats.comps++;
                if (pattern[pattern_offset] == text[text_offset + pattern_offset]) {
                    --pattern_offset;
                } else {
                    break;
                }
            }

            if (pattern_offset < 0) {
                // нашли вхождение
                stats.matches++;
                // используем таблицу для сдвига, как при несовпадении
                char next_char = text[text_offset + P]; // символ за найденным вхождением
                int shift = last_occurrence[static_cast<unsigned char>(next_char)];

                if (text_offset + P < T) {
                    if (shift < 0) {
                        text_offset += P;
                    } else {
                        text_offset += (P - shift);
                    }
                } else {
                    text_offset += 1; // если мы в конце
                }
            } else {
                // используем символ текста, на котором несовпало.
                // если такого символа нет в шаблоне, то сдвигаемся на число позиций, по длине шаблона
                // а если есть и последнее появление символа в шаблоне находится слева от текущей позиции,
                // то сдвигаемся так, чтобы совместить его с последним вхождением. если же есть но последнее
                // появление символа в шаблоне находится справа от текущшей позиции, то сдвигаем на 1 символ
                // (это не оптимально, но безопасно)
                char bad_char = text[text_offset + pattern_offset];
                int shift = last_occurrence[static_cast<unsigned char>(bad_char)];

                if (text_offset + pattern_offset < T) {
                    if (shift < 0) {
                        text_offset += P;
                    } else {
                        text_offset += (pattern_offset - shift);
                    }
                } else {
                    text_offset += 1; // если мы в конце
                }
            }
        }

        return stats;
    }
};


class BoyerMoore
{
private:
    std::array<int, 256> bad_characters_{};
    std::vector<int>     good_suffix_{};

public:
    ~BoyerMoore() = default;
    BoyerMoore() = delete;
    BoyerMoore(const BoyerMoore& other) = default;
    BoyerMoore(BoyerMoore&& other) noexcept = default;
    BoyerMoore& operator=(const BoyerMoore& other) = default;
    BoyerMoore& operator=(BoyerMoore&& other) noexcept = default;

    explicit BoyerMoore(const std::string& pattern) {
        // алгоритм использует две эвристики, выбирая из них тот сдвиг, который позволяет продвинуться дальше всего.
        // т.к. мы для усреднения будем запускать несколько раз поиск и собирать статистику, то чтобы генерация
        // этих экристик не накладывала оверхеда, я вынес процедуру в конструктор

        // эвристика плохого символа (Bad Character)
        // определяет, что делать, если символ в тексте не совпадает с символом в шаблоне
        // - если символ текста вообще не встречается в шаблоне, шаблон сдвигается вправо за этот один символ
        // - если символ текста есть в шаблоне, шаблон сдвигается так, чтобы совместить этот символ с его самым правым вхождением в шаблоне
        const int N = pattern.length();
        bad_characters_.fill(-1);
        for (int i = 0; i < N; ++i) {
            // для конкретного символа сохранится только его последнее местоположение в шаблоне
            // (т.е. самое правое) сколько бы раз он не встречался, чтобы не проскочить лишнего
            bad_characters_[static_cast<unsigned char>(pattern[i])] = i;
        }

        // эвристика хорошего суффикса (Good Suffix)
        // применяется, когда часть символов с конца шаблона уже успешно совпала с текстом, но затем произошел сбой
        // - ищется в шаблоне другая подстрока, идентичная уже совпавшей части (суффиксу), и сдвигается шаблон так, чтобы совместить их
        // - если такого суффикса больше нет в шаблоне, шаблон сдвигается вправо на длину совпавшей части
        // N+1 нужно, чтобы в начало поместить смещение длиной в строку шаблона
        good_suffix_.assign(N + 1, N);
        std::vector<int> border(N + 1, 0);
        int i = N;
        int j = N + 1;
        border[i] = j;
        while (i > 0) {
            while (j <= N && pattern[i - 1] != pattern[j - 1]) {
                if (good_suffix_[j] == N) {
                    good_suffix_[j] = j - i;
                }
                j = border[j];
            }
            --i;
            --j;
            border[i] = j;
        }
        j = border[0];
        for (i = 0; i <= N; ++i) {
            if (good_suffix_[i] == N) {
                good_suffix_[i] = j;
            }
            if (i == j) {
                j = border[j];
            }
        }
    }

    Stats search(const std::string& text, const std::string& pattern) {
        Stats stats;

        if (pattern.empty()) return stats;

        const int T = text.length();
        const int P = pattern.length();
        if (P > T) return stats;

        // в процессе поиска шаблон накладывается на строку слева направо,
        // но само сравнение символов идет с конца шаблона к его началу
        int text_offset = 0;
        while (text_offset <= (T - P)) {
            int pattern_offset = P - 1;

            // сравниваем справа налево
            while (pattern_offset >= 0) {
                stats.comps++;
                if (pattern[pattern_offset] == text[text_offset + pattern_offset]) {
                    --pattern_offset;
                } else {
                    break;
                }
            }

            if (pattern_offset < 0) {
                // нашли вхождение
                stats.matches++;
                text_offset += good_suffix_[0];
            } else {
                // нашли несовпадающий символ

                // сначала проверяем по таблице "плохих символов" и достаем смещение
                //   - если символ текста вообще не встречается в шаблоне, шаблон сдвигается вправо за этот один символ
                //   - если символ текста есть в шаблоне, шаблон сдвигается так, чтобы совместить этот символ с его самым правым местоположением в шаблоне
                int bad_characters_shift = pattern_offset - bad_characters_[static_cast<unsigned char>(text[text_offset + pattern_offset])];
                if (bad_characters_shift < 1) bad_characters_shift = 1; // проверяем тот случай, если самое правое вхождение символа уже прошагали

                // затем проверяем по таблице "хороших суффиксов" и достаем смещение
                //   - ищется в шаблоне другая подстрока, идентичная уже совпавшей части (суффиксу), и сдвигается шаблон так, чтобы совместить их
                //   - если такого суффикса больше нет в шаблоне, шаблон сдвигается вправо на длину совпавшей части
                int goodSuffixShift = good_suffix_[pattern_offset + 1];

                // выбираем по двум эвристикам максимальный сдвиг, чтобы эффективно двигать шаблон по тексту
                text_offset += std::max(bad_characters_shift, goodSuffixShift);
            }
        }

        return stats;
    }
};



class BoyerMooreHorspool {
private:
    std::array<int, 256> shift_table_{};

public:
    ~BoyerMooreHorspool() = default;
    BoyerMooreHorspool() = delete;
    BoyerMooreHorspool(const BoyerMooreHorspool& other) = default;
    BoyerMooreHorspool(BoyerMooreHorspool&& other) noexcept = default;
    BoyerMooreHorspool& operator=(const BoyerMooreHorspool& other) = default;
    BoyerMooreHorspool& operator=(BoyerMooreHorspool&& other) noexcept = default;

    explicit BoyerMooreHorspool(const std::string& pattern) {
        // это упрощённая версия Бойера-Мура, которая использует только эвристику стоп-символа.
        // в оригинальном Бойере-Муре стоп-символ - это символ текста, на котором произошло несовпадение,
        // а в этом алгоритме мы всегда смотрим на символ текста, который соответствует последнему символу шаблона.
        // т.к. мы для усреднения будем запускать несколько раз поиск и собирать статистику, то чтобы генерация
        // этой экристики не накладывала оверхеда, я вынес процедуру в конструктор

        const int N = pattern.length();

        // инициализируем сдвигом по умолчанию (длина шаблона)
        shift_table_.fill(N);

        // для каждого символа в шаблоне (кроме последнего) задаём сдвиг
        for (int i = 0; i < N - 1; ++i) {
            shift_table_[static_cast<unsigned char>(pattern[i])] = N - 1 - i;
        }
    }
    
    Stats search(const std::string& text, const std::string& pattern) {
        Stats stats;

        if (pattern.empty()) return stats;

        const int T = text.length();
        const int P = pattern.length();
        if (P > T) return stats;

        // в процессе поиска шаблон накладывается на строку слева направо,
        // но само сравнение символов идет с конца шаблона к его началу
        int text_offset = 0;
        while (text_offset <= (T - P)) {
            int pattern_offset = P - 1;

            // сравниваем справа налево
            while (pattern_offset >= 0) {
                stats.comps++;
                if (pattern[pattern_offset] == text[text_offset + pattern_offset]) {
                    --pattern_offset;
                } else {
                    break;
                }
            }

            if (pattern_offset < 0) {
                // нашли вхождение
                stats.matches++;
                text_offset += shift_table_[static_cast<unsigned char>(text[text_offset + P - 1])];
            } else {
                // всегда смотрим на последний символ окна
                text_offset += shift_table_[static_cast<unsigned char>(text[text_offset + P - 1])];
            }
        }

        return stats;
    }
};
