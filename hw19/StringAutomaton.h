#pragma once

#include "StringSearch.h"


class PatternAutomaton {
private:
    std::vector<std::array<int, 256>> moves_table_{}; // [state][char] = next_state

    static int next_state_(const std::string& pattern, int state, char ch) {
        // механизм поиска следующего состояния
        // если символ совпадает со следующим ожидаемым в шаблоне - переходим в следующее состояние
        // иначе ищем самую длинную границу (префикс) для state + ch при несовпадении очередного символа

        if (state < static_cast<int>(pattern.length())
        &&  ch == pattern[state]) {
            return state + 1;
        }
        if (state == 0) return 0;

        // для поиска самой длинной границы пытаемся использовать суффикс от текущего префикса
        // и движемся в сторону уменьшения длины суффикса
        std::string current = pattern.substr(0, state) + ch;
        for (int len = state; len > 0; --len) {
            if (current.substr(current.length() - len) == pattern.substr(0, len)) {
                return len;
            }
        }
        return 0;
    }

public:
    ~PatternAutomaton() = default;
    PatternAutomaton() = delete;
    PatternAutomaton(const PatternAutomaton& other) = default;
    PatternAutomaton(PatternAutomaton&& other) noexcept = default;
    PatternAutomaton& operator=(const PatternAutomaton& other) = default;
    PatternAutomaton& operator=(PatternAutomaton&& other) noexcept = default;

    explicit PatternAutomaton(const std::string& pattern) {
        // т.к. мы для усреднения будем запускать несколько раз поиск и собирать статистику, то чтобы генерация
        // префикской таблицы не накладывала оверхеда, я вынес процедуру в конструктор

        const int P = pattern.length();

        // состояния: 0..P, где P - терминальное (нашли шаблон), а 0 - начальное состоние
        moves_table_.resize(P + 1);
        for (int state = 0; state <= P; ++state) {
            for (int c = 0; c < 256; ++c) {
                char ch = static_cast<char>(c);
                moves_table_[state][c] = next_state_(pattern, state, ch);
            }
        }
    }

    Stats search(const std::string& text, const std::string& pattern) {
        Stats stats;

        if (pattern.empty()) return stats;

        const int P = pattern.length();

        // у нас есть полная посчитанная таблица перевохов (матрица) из символа в символ, под конкретный шаблон.
        // в матрице столбцы - это все возможные символы из доступного алфавита.
        // в матрице строки - это состояния, соответствующие префиксу (его длине). соотвественно строк у нас
        // столько, какова длина искомого шаблона (+1, для исходного (нулевого) состояния).
        // задача - отслеживать текущее состояние, символ за символом запихивая текст в таблицу переходов (матрицу).
        // как только мы дошли до максимальной строки, значит мы получили полное совпадеине.
        // алгоритм поиска максимально простой, однако сложность алгоиртма в том, что надо поддерживать слишком
        // большую матрицу переходов в памяти (все состояния нами не нужны), и её еще нужно посчитать

        int state = 0;
        for (char ch : text) {
            stats.comps++;

            state = moves_table_[state][static_cast<unsigned char>(ch)];
            if (state == P) {
                stats.matches++;
                // сбрасывать состояние не надо, т.к. у нас полная матрица переходов на руках.
                // и мы долждны отслуживать перекрытия шаблонов
            }
        }

        return stats;
    }
};



std::vector<int> make_prefix_table_slow(const std::string& pattern)
{
    const int P = pattern.length();

    // префикс-функция pi[i] - длина наибольшего собственного префикса строки pattern[0..i],
    // который также является и её суффиксом.
    // таблица считается по факту только при длине шаблона от 2-х символов и больше.
    // медленный алгоритм просто проходится по позициям каждого символа в шаблоне
    // и пробует все возможные варианты длин префиксов. медлденный алгоритм, потому что
    // у него практически кубическая сложность O(P^3)
    // пример заполнения табьлицы:
    // pattern = "ABABAC"
    // pos=0: "A"           (нет префикса)  pi[0]=0
    // pos=1: "AB"          (нет общего)    pi[1]=0
    // pos=2: "ABA"         "A"             pi[2]=1
    // pos=3: "ABAB"        "AB"            pi[3]=2
    // pos=4: "ABABA"       "ABA" (overlap) pi[4]=3
    // pos=5: "ABABAC"      (нет общего)    pi[5]=0

    std::vector<int> pi(P, 0);
    for (int pos = 1; pos < P; ++pos) {
        for (int len = pos; len > 0; --len) {
            int offset = pos - len + 1;
            bool match = true;
            // cравниваем префикс длины len с суффиксом длины len
            // префикс: pattern[0..len-1]
            // суффикс: pattern[pos-len+1..i]
            for (int k = 0; k < len; ++k) {
                if (pattern[k] != pattern[offset + k]) {
                    match = false;
                    break;
                }
            }

            if (match) {
                pi[pos] = len;
                break;
            }
        }
        // если не нашли pos то остается 0
    }
    return pi;
}

std::vector<int> make_prefix_table_fast(const std::string& pattern)
{
    const int P = pattern.length();

    // основная идея ускорения в том, что не нужно повторно перебирать все варианты префиксов,
    // мы можем использовать уже вычисленные значения на предыдущих этапах, ведь строка
    // не могла поменяться, только добавился новый символ, а лучшие варианты префиксов-суффиксов
    // на прошлых этапах мы уже знаем.
    // этот варинат значительно быстрее, его внутренний цикл не проходит все P значений, длина префикса
    // уменьшается очень быстро. поэтому сложность алгоритма скорее чуть хуже линейной O(k * P)
    // пример заполнения табьлицы:
    // pattern = "ABABAC"
    // pos=0: (нет префикса)        "A"                                             pi[0]=0
    // pos=1: pref=pi[0]=0          pattern[1]='B' == pattern[pref]='A'    нет      pi[1]=0
    // pos=2: pref=pi[1]=1          pattern[2]='A' == pattern[pref]='A'    да       pi[2]=1
    // pos=3: pref=pi[2]=1          pattern[3]='B' == pattern[pref]='B'    да       pi[3]=2
    // pos=4: pref=pi[3]=2          pattern[4]='A' == pattern[pref]='A'    да       pi[4]=3
    // pos=5: pref=pi[4]=3          pattern[5]='C' == pattern[pref]='B'    нет
    //        pref=pi[pref-1]=1     pattern[5]='C' == pattern[pref]='B'    нет
    //        pref=pi[pref-1]=0     pattern[5]='C' == pattern[pref]='A'    нет      pi[5]=0

    std::vector<int> pi(P, 0);
    for (int pos = 1; pos < P; ++pos) {
        int pref = pi[pos - 1]; // берём предыдущее значение префикса

        // идём по таблице предвычесленных значений на прошлых этапах,
        // пока не совпало и не дошли до самого начала - откатываемся к предыдущему
        // возможному префиксу
        while (pref > 0 && pattern[pos] != pattern[pref]) {
            pref = pi[pref - 1];
        }

        // если символы совпали, увеличиваем длину префикса
        if (pattern[pos] == pattern[pref]) {
            ++pref;
        }

        pi[pos] = pref;
    }
    return pi;
}



class KnuthMorrisPratt
{
private:
    std::vector<int> prefix_table_{};

public:
    ~KnuthMorrisPratt() = default;
    KnuthMorrisPratt() = delete;
    KnuthMorrisPratt(const KnuthMorrisPratt& other) = default;
    KnuthMorrisPratt(KnuthMorrisPratt&& other) noexcept = default;
    KnuthMorrisPratt& operator=(const KnuthMorrisPratt& other) = default;
    KnuthMorrisPratt& operator=(KnuthMorrisPratt&& other) noexcept = default;

    explicit KnuthMorrisPratt(const std::string& pattern) {
        // т.к. мы для усреднения будем запускать несколько раз поиск и собирать статистику, то чтобы генерация
        // префикской таблицы не накладывала оверхеда, я вынес процедуру в конструктор

        prefix_table_ = make_prefix_table_fast(pattern);
    }

    Stats search(const std::string& text, const std::string& pattern) {
        Stats stats;

        if (pattern.empty()) return stats;

        const int P = pattern.length();

        // в конечном автомате самая сложная часть, это посчитать изначально таблицу.
        // сам поиск простой, мы начинаем проверять по одному символу из исходного текста,
        // проверяя по таблице, в какое следующее состояние из текущего состояния мы перейдём.
        // каждый новый символ нас либо приближает к полному совпадению префикса, либо откатывает
        // на одно из предыдущих состояний автомата.
        // как только нашли полное совпадение, откатываем состояние на шаг назад и продолжаем
        // подавать следующие символы текста, чтобы не пропустить ситуации с перекрытием шаблона

        int pref_len = 0; // текущая длина совпавшего префикса
        for (char ch : text) {
            stats.comps++;

            // пока символ не совпадает и не дошли до начала
            // откатываемся к предыдущему возможному префиксу (состоянию в конечном автомате)
            while (pref_len > 0 && ch != pattern[pref_len]) {
                pref_len = prefix_table_[pref_len - 1];
            }

            // символы совпали, увеличиваем длину префикса
            if (ch == pattern[pref_len]) {
                ++pref_len;
            }

            if (pref_len == P) {
                stats.matches++;
                pref_len = prefix_table_[pref_len - 1];
            }
        }

        return stats;
    }
};
