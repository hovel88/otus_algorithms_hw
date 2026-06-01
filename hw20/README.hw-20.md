# ДЗ 20: Создать программу сжатия файлов на основе алгоритма RLE

## Цель

* написать функцию сжатия массива по алгоритму RLE
* написать программу для сжатия файлов
* написать программу для распаковки файлов
* при запуске программы без параметров она должна выводить краткую инструкцию, как её запускать для сжатия/распаковки файлов
* реализовать улучшенный алгоритм RLE: сжатие и распаковку
* сравнить работу программы с разными типами файлов: текст, фото, аудио, zip-архив

## Подготовка

Для сборки исходников запустить `make`:

```bash
lachugin@pop-os:~/otus/otus_algorithms_hw/hw20$ (main) ▶ make
g++ -lstdc++fs -std=c++17 -O2 -Wall -I. -o hw20_rle main.cpp
```

## Решение

В программе, демонстрирующей работу RLE реализован как упаковщик, так и распаковщик файлов. Также можно запустить с возможностью показа демо (заранее зашитые в коде несколько тестов), или же в режиме бенчмарка на указанных файлах.

Для того чтобы выбрать необходимый режим работы, используется обработка параметров командной строки:

```bash
dmitry@lachugin:~/otus/lachugin_algorithms_hw/hw20$ ./hw20_rle -h
Программа для демонстрации алгоритма компрессии RLE (Run-Length-Encoding)

usage: ./hw20_rle [options] [files...]

options:
  -c, --compress <file>    Сжать файл (создаст файл с расширением .rle)
  -d, --decompress <file>  Распаковать файл (удалить расширение .rle)
  -o, --output <file>      Указать выходной файл (для -c или -d)
  -b, --bench <files>      Запустить бенчмарк на указанных файлах
  -r, --bench-dir <dir>    Бенчмарк для всех файлов в папке
  --v1                     Использовать наивную версию алгоритма RLE
  --v2                     Использовать улучшенную версию алгоритма RLE (по умолчанию)
  --demo                   Запустить демонстрацию работы алгоритма
  -h, --help               Показать эту справку
```

Реализована небольшая система тестирования (для работы режима бенчмарка). Исходные файлы (и некие общие не относящиеся непосредственно к RLE вещи) находятся в `hw20/common.h`. Для прохождения бенчмарка был подготовлен набор файлов разного типа каталоге **test/**:

```bash
-rw-rw-r-- 1 dmitry dmitry  307453 июн  1 10:11 sample-15s.mp3
-rw-rw-r-- 1 dmitry dmitry 3382316 июн  1 10:12 sample-15s.wav
-rw-rw-r-- 1 dmitry dmitry     970 июн  1 10:19 sample-arch.7z
-rw-rw-r-- 1 dmitry dmitry   12414 июн  1 10:17 sample-arch.zip
-rw-rw-r-- 1 dmitry dmitry  263224 июн  1 09:58 sample-barbara.bmp
-rw-rw-r-- 1 dmitry dmitry   81721 июн  1 10:03 sample-barbara.jpg
-rwxrwxr-x 1 dmitry dmitry   84696 июн  1 10:05 sample-binary.bin
-rw-rw-r-- 1 dmitry dmitry     386 июн  1 10:15 sample-file.csv
-rw-rw-r-- 1 dmitry dmitry  263224 июн  1 09:58 sample-lena.bmp
-rw-rw-r-- 1 dmitry dmitry   32205 июн  1 10:02 sample-lena.jpg
-rw-rw-r-- 1 dmitry dmitry     446 июн  1 10:13 sample-text.txt
```

Сам алгоритм RLE реализован в классе **RLE** а файле `hw20/RLE.h`.

Согласно постановки домашнего задания, реализованы две версии RLE: наивная (максимально простой алгоритм) и улучшенная (стандартный алгоритм).

### Наивная версия RLE

Реализация в методах **_compress_naive()** и **_decompress_naive()**.

Идея очень простая, надо искать последовательности байтов и упаковывать их в пары `<байт><счетчик>`. Обратная распаковка тоже очень простая, мы считываем байт и затем считываем сколько раз он должен повторяться. В очень особенных случаях - это даст хорошую степень компрессии. Однако это скорее вырожденные случаи, и на практике данных, где нет повторений байт, каждый неповторяющийся байт будет кодироваться двумя байтами, т.е. исходные данные увеличатся в размере.

### Улучшенная версия RLE

Основная проблема наивного подхода в том как кодируются неповторяющиеся символы. Нам нужно понимать разницу между повторяющейся и неповторяющейся последовательностью. Идея улучшения - использовать концепцию контейнера TLV. Где T - это тип поля, L - длина данных, V - сами данные как есть. Только в данном случае T вырождено в один бит (выставлен (0x80) если повторяющаяся последовательность, и сброшен (0x00) если неповторяющаяся).

Для компактности. поля T и L можно уместить в один байт, соответственно на размер кодируемого окна остается всего 7 бит (т.е. максимум 127 байт). Однако это даст возможность на литеральных последовательностях (когда символы не повторяются) длиной до 127 байт получить всего лишь +1 байт для служебных нужд, вместо +127 байт как в наивном варианте алгоритма.

### Тесты на демо

Можно запустить быстрый тест

```bash
dmitry@lachugin:~/otus/lachugin_algorithms_hw/hw20$ ./hw20_rle --demo
Программа для демонстрации алгоритма компрессии RLE (Run-Length-Encoding)

=== RLE demo ===
с использованием наивного алгоритма RLE (для сравнения)

test 1: 'AAAAABBBCCCCCCDDD'
  original size    : 17
  compressed size  : 8
  compressing ratio: 47.0588%
  valid            : OK

test 2: ASCII-art
  original size    : 85
  compressed size  : 28
  compressing ratio: 32.9412%
  valid            : OK

test 3: random
  original size    : 1000
  compressed size  : 2000
  compressing ratio: 200%
  valid            : OK

=== RLE demo ===
с использованием стандартного (улучшенного) алгоритма RLE

test 1: 'AAAAABBBCCCCCCDDD'
  original size    : 17
  compressed size  : 8
  compressing ratio: 47.0588%
  valid            : OK

test 2: ASCII-art
  original size    : 85
  compressed size  : 28
  compressing ratio: 32.9412%
  valid            : OK

test 3: random
  original size    : 1000
  compressed size  : 1008
  compressing ratio: 100.8%
  valid            : OK

```

Из него видно, что улучшенная версия RLE работает НЕ ХУЖЕ, чем наивная версия. Однако на рандомизированных данные работает ЗНАЧИТЕЛЬНО ЛУЧШЕ! Наивная версия увеличила размер исходных данных в 2 раза (`compressing ratio: 200%`) за счет служебной информации. А вот улучшенная версия совсем незначительно (`compressing ratio: 100.8%`).

### Тест бенчмарка

Далее можно запустить программу на всех видах данных, которые были подготовлены. Запустим 2 раза, сначала укажем использовать наивную версию алгоритма, а затем - улучшенную версию алгоритма RLE.

Программа в каждом случае выдаст сводную табличку.

Наивная версия RLE:

```bash
dmitry@lachugin:~/otus/lachugin_algorithms_hw/hw20$ ./hw20_rle --v1 -r ./test/
Программа для демонстрации алгоритма компрессии RLE (Run-Length-Encoding)

file: './test/sample-file.csv' using RLE version V1 (naive)
file: './test/sample-15s.mp3' using RLE version V1 (naive)
file: './test/sample-binary.bin' using RLE version V1 (naive)
file: './test/sample-text.txt' using RLE version V1 (naive)
file: './test/sample-lena.jpg' using RLE version V1 (naive)
file: './test/sample-arch.7z' using RLE version V1 (naive)
file: './test/sample-barbara.bmp' using RLE version V1 (naive)
file: './test/sample-barbara.jpg' using RLE version V1 (naive)
file: './test/sample-15s.wav' using RLE version V1 (naive)
file: './test/sample-arch.zip' using RLE version V1 (naive)
file: './test/sample-lena.bmp' using RLE version V1 (naive)

=================================================================================================================
File                     Type      Status     Origin bytes   Compr. bytes   Ratio %    Compr. time  Decompr. time
-----------------------------------------------------------------------------------------------------------------
sample-file.csv          other     OK                  386            724    187.56       13183 ns        4799 ns
sample-15s.mp3           audio     OK               307453         575646    187.23     5197183 ns     2208441 ns
sample-binary.bin        binary    OK                84696         124120    146.55      732782 ns      485310 ns
sample-text.txt          text      OK                  446            876    196.41        4910 ns        2682 ns
sample-lena.jpg          image     OK                32205          63854    198.27      221040 ns      134737 ns
sample-arch.7z           archive   OK                  970           1898    195.67        7935 ns        4662 ns
sample-barbara.bmp       image     OK               263224         481162    182.80     2461148 ns     1571286 ns
sample-barbara.jpg       image     OK                81721         162696    199.09      519935 ns      304023 ns
sample-15s.wav           audio     OK              3382316        6737478    199.20    22337080 ns    11463774 ns
sample-arch.zip          archive   OK                12414          19700    158.69       54327 ns       26575 ns
sample-lena.bmp          image     OK               263224         475470    180.63     1988596 ns     1356873 ns
-----------------------------------------------------------------------------------------------------------------

Average compression ration:
---------------------------
binary          : 146.55% (!!!)
archive         : 177.18% (!!!)
other           : 187.56% (!!!)
image           : 190.20% (!!!)
audio           : 193.21% (!!!)
text            : 196.41% (!!!)
```

Улучшенная версия RLE:

```bash
dmitry@lachugin:~/otus/lachugin_algorithms_hw/hw20$ ./hw20_rle --v2 -r ./test/
Программа для демонстрации алгоритма компрессии RLE (Run-Length-Encoding)

file: './test/sample-file.csv' using RLE version V2 (common)
file: './test/sample-15s.mp3' using RLE version V2 (common)
file: './test/sample-binary.bin' using RLE version V2 (common)
file: './test/sample-text.txt' using RLE version V2 (common)
file: './test/sample-lena.jpg' using RLE version V2 (common)
file: './test/sample-arch.7z' using RLE version V2 (common)
file: './test/sample-barbara.bmp' using RLE version V2 (common)
file: './test/sample-barbara.jpg' using RLE version V2 (common)
file: './test/sample-15s.wav' using RLE version V2 (common)
file: './test/sample-arch.zip' using RLE version V2 (common)
file: './test/sample-lena.bmp' using RLE version V2 (common)

=================================================================================================================
File                     Type      Status     Origin bytes   Compr. bytes   Ratio %    Compr. time  Decompr. time
-----------------------------------------------------------------------------------------------------------------
sample-file.csv          other     OK                  386            387    100.26        9600 ns        9383 ns
sample-15s.mp3           audio     OK               307453         296895     96.57     3013415 ns     2185144 ns
sample-binary.bin        binary    OK                84696          71562     84.49      664393 ns      458532 ns
sample-text.txt          text      OK                  446            450    100.90        3901 ns        2896 ns
sample-lena.jpg          image     OK                32205          32351    100.45      168529 ns      125045 ns
sample-arch.7z           archive   OK                  970            971    100.10        6443 ns        5131 ns
sample-barbara.bmp       image     OK               263224         262971     99.90     2448366 ns     1568153 ns
sample-barbara.jpg       image     OK                81721          82353    100.77      438451 ns      353733 ns
sample-15s.wav           audio     OK              3382316        3402777    100.60    18201200 ns    10138843 ns
sample-arch.zip          archive   OK                12414          11611     93.53       50105 ns       30513 ns
sample-lena.bmp          image     OK               263224         263667    100.17     1762031 ns      906772 ns
-----------------------------------------------------------------------------------------------------------------

Average compression ration:
---------------------------
binary          : 84.49% (++)
archive         : 96.82% (+)
audio           : 98.59% (+)
other           : 100.26% (+)
image           : 100.32% (+)
text            : 100.90% (+)
```

Видно, что в реальных тестах (а не на искусственных примерах), наивная версия RLE всегда показывает увеличение размера данных.

## Выводы

* RLE эффективен для данных с длинными повторяющимися последовательностями
* Лучше всего сжимает: текстовые файлы с пробелами, BMP изображения, простые графики
* Худшие результаты: уже сжатые данные (ZIP, JPG, MP3), случайные данные
* Для некоторых файлов размер может даже увеличиться (коэффициент > 100%)
