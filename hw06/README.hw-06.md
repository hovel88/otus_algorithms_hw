# ДЗ 05: Алгоритмы сортировки (BubbleSort, InsertionSort, ShellSort)

## Цель

Создание, тестирование и сравнение алгоритмов сортировки:

* сортировка обменом или "метод пузырька" (BubbleSort)
* сортировка вставками (InsertionSort)
* сортировка включниями с убывающими приращениями или "метод Шелла" (ShellSort)

## Подготовка

Основные характеристики алгоритмов сортировки:

1. Стабильность - когда алгоритм не меняет порядок элементов с одинаковыми ключами (устойчивость). Обычно имеет смысл при сортировке объектов/записей, а не отдельных чисел.
2. Адаптивность - когда алгоритм сортировки предварительного отсортированного массива выполняется значительно быстрее, т.е. адаптируется к данным.
3. On-line - когда алгоритм анализирует данные последовательно получая элементы, а не на изначально полном объеме данных, т.е. для каждого нового элемента определяется его положение в уже отсортированном массиве.

| | BubbleSort | InsertionSort | ShellSort |
| -- | -- | -- | -- |
| Сложность | O(N^2) | O(N^2) | зависит от выбора шага <br> может быть от O(N^(3/2)) до O(N^2) |
| Стабильность | + | + | - |
| Адаптивность | - | + | - |
| On-line | - | + | - |

Для сборки исходников запустить `make`:

```bash
dmitry@lachugin:~/lachugin_algorithms_hw/hw06$ make
g++ -lstdc++fs -std=c++17 -O2 -Wall -I. -o hw06_sort main_sort.cpp BubbleSort.cpp InsertionSort.cpp ShellSort.cpp
g++ -lstdc++fs -std=c++17 -O2 -Wall -I. -o hw06_sort_adv main_sort_adv.cpp BubbleSort.cpp InsertionSort.cpp ShellSort.cpp
g++ -lstdc++fs -std=c++17 -O2 -Wall -I. -o hw06_checker checker.cpp checker_internals.cpp BubbleSort.cpp InsertionSort.cpp ShellSort.cpp
```

Основными исследуемыми характеристиками алгоритмов сортировки являются:

* время выполнения сортировки массива размером N (**Duration (ns)**)
* количество операций сравнений (**Comparisons**)
* количество операций перестановки элементов (**Swaps**)

Все алгоритмы организованы в классы, унаследованные от базового интерфейсного класса **ISort**, это позволит легче производить тестирование, а также спрятать в базовом классе некоторые общие вещи.

## Решение

### Уровень JUNIOR

#### BUB1

Реализация алгоритма в файле `hw06/BubbleSort.cpp` в методе **sort_v1**.

Это базовая реализация. Начинаем с первого (самого левого) элемента, и ПОСЛЕДОВАТЕЛЬНО сравниваем его с последующим элементом и меняем их местами при необходимости, проходя до самого конца массива. Затем переходим к следующему элементу, берем его и начинаем ПОСЛЕДОВАТЕЛЬНО прикладывать его ко всем оставшимся до конца массива элементам, сравнивать и менять местами, если нужно.

#### INS1

Реализация алгоритма в файле `hw06/InsertionSort.cpp` в методе **sort_v1**.

Это базовая реализация, состоящая в проталкивании нового элемента в нужную позицию в итоговом отсортированном массиве.

Т.е. считаем, что в начале массива у нас отсортированная часть, и начинаем со второго элемента массива (и до конца) проталкивать элемент влево по одному. ПОСЛЕДОВАТЕЛЬНО сравнивая и меняя местами соседние элементы справа налево.

#### SHS1

Реализация алгоритма в файле `hw06/ShellSort.cpp` в методе **sort_v1**.

Это базовая реализация, в основе своей содержащая алгоритм вставками (который INS2, с дополнительной ячейкой памяти но без бинарного поиска). Только в сортировке вставками отсортированной последовательностью считались все элементы левее позиции нового элемента, а в данном случае не все, а каждый gap-й элемент левее образуют готовую последовательность.

Т.е. сортируем элементы блоками, постепенно уменьшая шаг (gap). И в итоге доходим до шага gap=1, что соответствует обычному проходу алгоритмом вставками. В данной реализации за первоначальный шаг берется половина от исходного размера массива, т.е. на первом этапе сортируются только 2 элемента (0-й и N/2-й). Затем шаг уменьшается вдвое (gap=gap/2), алгоритм повторяется и т.д.

#### Сравнительный анализ базовых реализаций

Тестировались алгоритмы: **BubbleSort_v1**, **InsertionSort_v1**, **ShellSort_v1**.

Тестирование проводилось на наборах массивов нескольких размеров: **100**, **1000**, **10000**, **100000**.  
Не проводил тестирование на размере массива 1000000, т.к. уже были получены показательные результаты и на меньшем объеме данных.

Для теста создавался один массив максимального размера, заполнялся произвольными числами от 0 до 99.  
Затем, все варианты массивов меньшего размера формировались путём создания копии первых N элементов из массива максимального размера.  
А также для каждого подопытного алгоритма сортировки формировалась своя копия исходного массива произвольных чисел.  
Таким образом старался привести эксперимент ближе к одинаковым условиям.

После запуска программы, она сформирует таблицу-отчет:

```bash
dmitry@lachugin:~/otus/lachugin_algorithms_hw/hw06$ ./hw06_sort
программа оценки работы алгоритмов сортировки (BubbleSort, InsertionSort, ShellSort) в базовых версиях
Первые и последние 20 элементов массива из 100000 элементов: 
   99 21 73 51 21 4 37 2 28 28 27 97 21 53 6 73 38 12 57 12 ... 19 69 43 17 2 86 72 45 13 19 91 77 27 46 9 43 57 58 49 48
Name                     Array size    Comparisons          Swaps       Duration (ns)
BubbleSort_v1                   100           4950           2166               46521
InsertionSort_v1                100           2262           2166               17699
ShellSort_v1                    100            900            441               17831
BubbleSort_v1                  1000         499500         250425             3130762
InsertionSort_v1               1000         251419         250425             1618523
ShellSort_v1                   1000          13079           5590              248192
BubbleSort_v1                 10000       49995000       24919876           117143574
InsertionSort_v1              10000       24929870       24919876            32725170
ShellSort_v1                  10000         203775          88802              666377
BubbleSort_v1                100000     4999950000     2469361244         12357021860
InsertionSort_v1             100000     2469461238     2469361244          3173344084
ShellSort_v1                 100000        2528706        1077480             7492667
```

Видим, что для алгоритма сортировки обменом (**BubbleSort_v2**) с увеличением N порядки сложностей по времени, по количеству операций сравнений и по операциям обменов - растут квадратично. В целом это и ожидаемо.

Если посмотреть на группы алгоритмов **BubbleSort**, **InsertionSort** и **ShellSort**, то сразу бросается в глаза, как заметно уменьшается порядок сложностей по операциям сравнений и обмена. Сложность по времени выполнения на малых N (до 100) не такая однозначная, тут возможно даже выиграет алгоритмы сортировки вставками **InsertionSort_v1**, а вот при увеличении N быстро отрываются алгоритмы сортировки Шелла. Связано это с тем, что в алгоритмах Шелла слегка более сложная логика подготовки и вычисления шага. Эта сложность оказывает влияние на малых размерах, но при увеличении размера массива влияние становится ничтожным.

### Уровень MIDDLE

#### BUB2

Реализация алгоритма в файле `hw06/BubbleSort.cpp` в методе **sort_v2**.

Эла оптимизация отличается от базовой реализации только дополнительным флагом для раннего выхода. Если на предыдущей итерации прохода по правой части массива мы выявили, что операций обмена не потребовалось, значит оставшаяся часть массива отсортирована, и перемалывать еще на новых итерациях не нужно.

Этот алгоритм особенно положительно сказывается только на почти отсортированных исходных массивах.

#### INS2

Реализация алгоритма в файле `hw06/InsertionSort.cpp` в методе **sort_v2**.

Реализация отличается о базовой тем, что используется дополнительная ячейка памяти, чтобы запомнить исследуемый элемент. Затем перемещаем вправо все необходимые элементы итогового отсортированного массива, пока не найдем подходящее место для вставки, ПОСЛЕДОВАТЕЛЬНО перебирая сравнивая один за другим. И затем помещаем в нужное место элемент из ячейки памяти.

Если быть точным, то реально операций swap тут не требуется. Т.е. все шаги является простыми присваиваниями, нет необходимости временно сохранять элемент и менять местами два соседних, нам достаточно сдвигать элементы, переопределяя их, т.е. достаточно делать `arr[t + 1] = arr[t]`, но для соблюдения формальности будем работать через swap.

В остальном же, этот оптимизированный вариант алгоритма мало отличается от базового варианта алгоритма с проталкиванием элемента.

#### INS3

Реализация алгоритма в файле `hw06/InsertionSort.cpp` в методе **sort_v3**.

Реализация отличается о базовой тем, что используется дополнительная ячейка памяти, чтобы запомнить исследуемый элемент. Затем применяем БИНАРНЫЙ ПОИСК, чтобы быстро найти подходящее место для вставки нового элемента. После этого перемещаем все необходимые элементы итогового отсортированного массива вправо. И затем помещаем в нужное место элемент из ячейки памяти.

Если быть точным, то реально операций swap тут не требуется. Т.е. все шаги является простыми присваиваниями, нет необходимости временно сохранять элемент и менять местами два соседних, нам достаточно сдвигать элементы, переопределяя их, т.е. достаточно делать `arr[t + 1] = arr[t]`, но для соблюдения формальности будем работать через swap.

В остальном же, этот оптимизированный вариант алгоритма мало отличается от базового варианта алгоритма с проталкиванием элемента. Т.е. основное отличие - быстро найти место для вставки, выполнив меньше операций сравнений. Т.е. количество сравнений уменьшается с O(N) до O(log N), хотя количество сдвигов остается тем же.

#### SHS2

Реализация алгоритма в файле `hw06/ShellSort.cpp` в методе **sort_v2**.

Эта оптимизация отличается об базового алгоритма тем, как выбирается шаг (gap). В данной реализации используется алгоритм, который в Интернетах называется алгоритмом Хиббарда. Для gap используется последовательность: 2^k - 1 (..., 31, 15, 7, 3, 1), что в худшем случая дает сложность O(N^(3/2)).

Т.е. утверждается, что когда шаги взаимно простые (не имеют общих делителей), то это обычно дает лучшую производительность на больших массивах. Не происходит сортировки тех элементов, которые уже были отсортированы на шагах ранее, т.к. нет общих делителей.  
Для v1: N/2, N/4, N/8, ..., 1
Для v2: 2^k - 1 (..., 31, 15, 7, 3, 1)

Вот как раз и проверим данное утверждение.

#### SHS3

Реализация алгоритма в файле `hw06/ShellSort.cpp` в методе **sort_v3**.

Эта оптимизация отличается об базового алгоритма тем, как выбирается шаг (gap). В данной реализации используется алгоритм, который в Интернетах называется алгоритмом Седжвика. Хотя почему-то в разных ресурсах указаны под этим названием совершенно разные формулы зависимостей, хотя идея одна и та же.

В моей реализации, для gap используется две последовательности:

* `9*4^k - 9*2^k + 1`   (..., 2197, 505, 109, 19, 1) и
* `4^k + 3*2^k + 1`   (..., 1121, 305, 89, 29, 11, 5)

Указывается, что в худшем случая дает сложность O(N^(4/3)). И утверждается, что это дает лучшую производительность, т.к. тут шаги не только взаимно простые (не имеют общих делителей), но и перекрывают друг друга (дает лучшее перемешивание), а также шаги более крупные в конце алгоритма, ведь с мелкими шагами в v1 (..., 4, 2 ,1) и v2 (..., 5, 3 ,1) сортировка вставками "перемалывает" одно и тоже.

Вот как раз и проверим данное утверждение.

#### Сравнительный анализ реализаций с оптимизациями

Тестировались алгоритмы: **BubbleSort_v2**, **InsertionSort_v2**, **InsertionSort_v3**, **ShellSort_v2**, **ShellSort_v3**.

Тестирование проводилось на наборах массивов нескольких размеров: **100**, **1000**, **10000**, **100000**.  
Не проводил тестирование на размере массива 1000000, т.к. уже были получены показательные результаты и на меньшем объеме данных.

Для теста создавался один массив максимального размера, заполнялся произвольными числами от 0 до 99.  
Затем, все варианты массивов меньшего размера формировались путём создания копии первых N элементов из массива максимального размера.  
А также для каждого подопытного алгоритма сортировки формировалась своя копия исходного массива произвольных чисел.  
Таким образом старался привести эксперимент ближе к одинаковым условиям.

После запуска программы, она сформирует таблицу-отчет:

```bash
dmitry@lachugin:~/lachugin_algorithms_hw/hw06$ ./hw06_sort_adv 
программа оценки работы алгоритмов сортировки (BubbleSort, InsertionSort, ShellSort) в оптимизированных версиях
Первые и последние 20 элементов массива из 100000 элементов: 
   99 21 73 51 21 4 37 2 28 28 27 97 21 53 6 73 38 12 57 12 ... 19 69 43 17 2 86 72 45 13 19 91 77 27 46 9 43 57 58 49 48
Name                     Array size    Comparisons          Swaps       Duration (ns)
BubbleSort_v2                   100           5049           2166               48321
InsertionSort_v2                100           2262           2166               15332
InsertionSort_v3                100           1151           2166               21115
ShellSort_v2                    100            703            267               13449
ShellSort_v3                    100            734            321               19612
BubbleSort_v2                  1000         500464         250425             3152867
InsertionSort_v2               1000         251419         250425             1695566
InsertionSort_v3               1000          18165         250425              810246
ShellSort_v2                   1000          12741           5301              215763
ShellSort_v3                   1000          11930           4563              245497
BubbleSort_v2                 10000       50000814       24919876           123470416
InsertionSort_v2              10000       24929870       24919876            35384465
InsertionSort_v3              10000         248145       24919876            13451195
ShellSort_v2                  10000         175708          67149              531528
ShellSort_v3                  10000         157908          50483              590667
BubbleSort_v2                100000     4999540455     2469361244         12977863491
InsertionSort_v2             100000     2469461238     2469361244          3442325943
InsertionSort_v3             100000        3146603     2469361244          1343310161
ShellSort_v2                 100000        2241354         822458             6385898
ShellSort_v3                 100000        1909362         496033             6390574
```

Видим, что даже для оптимизированного алгоритма сортировки обменом (**BubbleSort_v2**) с увеличением N порядки сложностей по времени, по количеству операций сравнений и по операциям обменов - растут квадратично. В целом это и ожидаемо.

В общем-то, если посмотреть на группы алгоритмов **BubbleSort**, **InsertionSort** и **ShellSort**, то сразу бросается в глаза, как заметно уменьшается порядок сложностей по операциям сравнений и обмена. Сложность по времени выполнения на малых N (до 100) не такая однозначная, тут возможно даже выиграет алгоритмы сортировки вставками **InsertionSort_v2**, а вот при увеличении N быстро отрываются алгоритмы сортировки Шелла. Связано это с тем, что в алгоритмах Шелла слегка более сложная логика подготовки и вычисления шага, в зависимости от размера массива. Эта сложность оказывает влияние на малых размерах, но при увеличении размера массива влияние становится ничтожным.

Если обратить внимание на алгоритмы сортировки вставками **InsertionSort_v2** и **InsertionSort_v3**, то можно видеть, что операций обменов у них одинаково, а вот для **InsertionSort_v3** операций сравнений ощутимо меньше. И чем больше N, тем заметно сильнее. Связано это с применением бинарного поиска места вставки, вместо последовательного перебора. Из-за этого вариант **InsertionSort_v3** алгоритма выполняется по времени быстрее.

С алгоритмами Шелла (**ShellSort_v2** и **ShellSort_v3**) тоже всё не однозначно. На малых значениях N более сложный алгоритм **ShellSort_v3** дает и результаты хуже, как по времени, так и по количеству операций сравнений и вставки. Это связано с более сложным алгоритмом выбора gap, и с бОльшим перекрытием блоков на каждой итерации. Однако, на больших N становится заметно что по операциям сравнения и обменов алгоритм **ShellSort_v3** начинает выигрывать (опять же из-за особенностей формирования gap с перекрытиями), а по временной сложности алгоритмы сравнялись. Можно экстраполировать на еще бОльшие N, и ожидать что **ShellSort_v3** начнет выигрывать у **ShellSort_v2** по всем показателям.

### Уровень SENIOR

#### Комплексное тестирование алгоритмов

Для тестов используются наборы данных с несколькими различными характеристиками:

* random — массив из случайных чисел (`hw06/sorting-tests/0.random`)
* digits — массив из случайных цифр (`hw06/sorting-tests/1.digits`)
* sorted — на 99% отсортированный массив (`hw06/sorting-tests/2.sorted`)
* revers — обратно-отсортированный массив (`hw06/sorting-tests/3.revers`)

Написана система тестирования.

Тестируются все алгоритмы с их оптимизациями (**BubbleSort_v1**, **BubbleSort_v2**, **InsertionSort_v1**, **InsertionSort_v2**, **InsertionSort_v3**, **ShellSort_v1**, **ShellSort_v2**, **ShellSort_v3**).

После запуска, программа выводит сводную таблицу с результатами:

```bash
dmitry@lachugin:~/otus/lachugin_algorithms_hw/hw06$ ./hw06_checker 
Подготовлено 8 тестов из каталога "/home/dmitry/otus/lachugin_algorithms_hw/hw06/sorting-tests/0.random"
Подготовлено 8 тестов из каталога "/home/dmitry/otus/lachugin_algorithms_hw/hw06/sorting-tests/1.digits"
Подготовлено 8 тестов из каталога "/home/dmitry/otus/lachugin_algorithms_hw/hw06/sorting-tests/2.sorted"
Подготовлено 8 тестов из каталога "/home/dmitry/otus/lachugin_algorithms_hw/hw06/sorting-tests/3.revers"
Запускаем тест группы: "0.random"
   исходный массив из 1 элементов: 
   0
  OK  --> BubbleSort_v1 (size=1): comps=0  swaps=0  duration=390
  OK  --> BubbleSort_v2 (size=1): comps=0  swaps=0  duration=90
  OK  --> InsertionSort_v1 (size=1): comps=0  swaps=0  duration=83
  OK  --> InsertionSort_v2 (size=1): comps=0  swaps=0  duration=93
  OK  --> InsertionSort_v3 (size=1): comps=0  swaps=0  duration=92
  OK  --> ShellSort_v1 (size=1): comps=0  swaps=0  duration=87
  OK  --> ShellSort_v2 (size=1): comps=0  swaps=0  duration=86
  OK  --> ShellSort_v3 (size=1): comps=0  swaps=0  duration=120
   первые и последние 5 элементов массива из 10 элементов: 
   7 0 6 1 3 ... 7 0 6 1 3
  OK  --> BubbleSort_v1 (size=10): comps=45  swaps=16  duration=430
  OK  --> BubbleSort_v2 (size=10): comps=52  swaps=16  duration=423
  OK  --> InsertionSort_v1 (size=10): comps=24  swaps=16  duration=249
  OK  --> InsertionSort_v2 (size=10): comps=24  swaps=16  duration=281
  OK  --> InsertionSort_v3 (size=10): comps=55  swaps=16  duration=452
  OK  --> ShellSort_v1 (size=10): comps=29  swaps=10  duration=421
  OK  --> ShellSort_v2 (size=10): comps=25  swaps=10  duration=343
  OK  --> ShellSort_v3 (size=10): comps=23  swaps=12  duration=812
   первые и последние 10 элементов массива из 100 элементов: 
   74 11 72 33 40 28 23 17 66 61 ... 74 11 72 33 40 28 23 17 66 61
  OK  --> BubbleSort_v1 (size=100): comps=4950  swaps=2157  duration=17183
  OK  --> BubbleSort_v2 (size=100): comps=4819  swaps=2157  duration=15335
  OK  --> InsertionSort_v1 (size=100): comps=2253  swaps=2157  duration=5109
  OK  --> InsertionSort_v2 (size=100): comps=2253  swaps=2157  duration=5064
  OK  --> InsertionSort_v3 (size=100): comps=1153  swaps=2157  duration=6604
  OK  --> ShellSort_v1 (size=100): comps=877  swaps=413  duration=6031
  OK  --> ShellSort_v2 (size=100): comps=834  swaps=403  duration=5178
  OK  --> ShellSort_v3 (size=100): comps=734  swaps=331  duration=6075
   первые и последние 10 элементов массива из 1000 элементов: 
   561 233 825 817 336 551 811 627 485 812 ... 561 233 825 817 336 551 811 627 485 812
  OK  --> BubbleSort_v1 (size=1000): comps=499500  swaps=247489  duration=1134920
  OK  --> BubbleSort_v2 (size=1000): comps=499372  swaps=247489  duration=1137172
  OK  --> InsertionSort_v1 (size=1000): comps=248479  swaps=247489  duration=569831
  OK  --> InsertionSort_v2 (size=1000): comps=248479  swaps=247489  duration=561262
  OK  --> InsertionSort_v3 (size=1000): comps=18169  swaps=247489  duration=271499
  OK  --> ShellSort_v1 (size=1000): comps=15296  swaps=7815  duration=99880
  OK  --> ShellSort_v2 (size=1000): comps=14379  swaps=6975  duration=80295
  OK  --> ShellSort_v3 (size=1000): comps=14113  swaps=6741  duration=95675
   первые и последние 10 элементов массива из 10000 элементов: 
   3839 1880 4323 1741 3390 3908 186 4357 9885 3111 ... 3839 1880 4323 1741 3390 3908 186 4357 9885 3111
  OK  --> BubbleSort_v1 (size=10000): comps=49995000  swaps=25288624  duration=119710144
  OK  --> BubbleSort_v2 (size=10000): comps=49997497  swaps=25288624  duration=114299144
  OK  --> InsertionSort_v1 (size=10000): comps=25298615  swaps=25288624  duration=38083206
  OK  --> InsertionSort_v2 (size=10000): comps=25298615  swaps=25288624  duration=37616849
  OK  --> InsertionSort_v3 (size=10000): comps=247903  swaps=25288624  duration=14605905
  OK  --> ShellSort_v1 (size=10000): comps=267306  swaps=152486  duration=961976
  OK  --> ShellSort_v2 (size=10000): comps=255750  swaps=147412  duration=794171
  OK  --> ShellSort_v3 (size=10000): comps=203477  swaps=96320  duration=840091
   первые и последние 10 элементов массива из 100000 элементов: 
   8167 4911 1309 46835 39659 7048 82594 57334 60184 49556 ... 8167 4911 1309 46835 39659 7048 82594 57334 60184 49556
  OK  --> BubbleSort_v1 (size=100000): comps=4999950000  swaps=2501737763  duration=13944874101
  OK  --> BubbleSort_v2 (size=100000): comps=5000040547  swaps=2501737763  duration=13228193306
  OK  --> InsertionSort_v1 (size=100000): comps=2501837753  swaps=2501737763  duration=3615766050
  OK  --> InsertionSort_v2 (size=100000): comps=2501837753  swaps=2501737763  duration=3556630061
  OK  --> InsertionSort_v3 (size=100000): comps=3145309  swaps=2501737763  duration=1401051870
  OK  --> ShellSort_v1 (size=100000): comps=4562610  swaps=3113279  duration=13522918
  OK  --> ShellSort_v2 (size=100000): comps=3920114  swaps=2503799  duration=11292352
  OK  --> ShellSort_v3 (size=100000): comps=2677097  swaps=1266093  duration=11825161
   первые и последние 10 элементов массива из 1000000 элементов: 
   106757 84762 359524 454600 539912 260065 748214 424144 828940 441559 ... 106757 84762 359524 454600 539912 260065 748214 424144 828940 441559
  FAIL  --> BubbleSort_v1 (size=1000000): comps=0  swaps=0  duration=0
  FAIL  --> BubbleSort_v2 (size=1000000): comps=0  swaps=0  duration=0
  FAIL  --> InsertionSort_v1 (size=1000000): comps=0  swaps=0  duration=0
  FAIL  --> InsertionSort_v2 (size=1000000): comps=0  swaps=0  duration=0
  FAIL  --> InsertionSort_v3 (size=1000000): comps=0  swaps=0  duration=0
  OK  --> ShellSort_v1 (size=1000000): comps=68027835  swaps=50530714  duration=184869860
  OK  --> ShellSort_v2 (size=1000000): comps=62283286  swaps=44889440  duration=163891887
  OK  --> ShellSort_v3 (size=1000000): comps=33226631  swaps=15880632  duration=147284701
   первые и последние 10 элементов массива из 10000000 элементов: 
   3014676 6210057 6922993 3173860 8803028 1921990 3755739 4739250 7609328 4600455 ... 3014676 6210057 6922993 3173860 8803028 1921990 3755739 4739250 7609328 4600455
  FAIL  --> BubbleSort_v1 (size=10000000): comps=0  swaps=0  duration=0
  FAIL  --> BubbleSort_v2 (size=10000000): comps=0  swaps=0  duration=0
  FAIL  --> InsertionSort_v1 (size=10000000): comps=0  swaps=0  duration=0
  FAIL  --> InsertionSort_v2 (size=10000000): comps=0  swaps=0  duration=0
  FAIL  --> InsertionSort_v3 (size=10000000): comps=0  swaps=0  duration=0
  OK  --> ShellSort_v1 (size=10000000): comps=1041605900  swaps=826620860  duration=2527025336
  OK  --> ShellSort_v2 (size=10000000): comps=1040073945  swaps=832098474  duration=2601588816
  OK  --> ShellSort_v3 (size=10000000): comps=398594255  swaps=191906810  duration=1754602365

Запускаем тест группы: "1.digits"
   исходный массив из 1 элементов: 
   0
  OK  --> BubbleSort_v1 (size=1): comps=0  swaps=0  duration=163
  OK  --> BubbleSort_v2 (size=1): comps=0  swaps=0  duration=69
  OK  --> InsertionSort_v1 (size=1): comps=0  swaps=0  duration=51
  OK  --> InsertionSort_v2 (size=1): comps=0  swaps=0  duration=54
  OK  --> InsertionSort_v3 (size=1): comps=0  swaps=0  duration=57
  OK  --> ShellSort_v1 (size=1): comps=0  swaps=0  duration=60
  OK  --> ShellSort_v2 (size=1): comps=0  swaps=0  duration=43
  OK  --> ShellSort_v3 (size=1): comps=0  swaps=0  duration=163
   первые и последние 5 элементов массива из 10 элементов: 
   6 1 7 3 2 ... 6 1 7 3 2
  OK  --> BubbleSort_v1 (size=10): comps=45  swaps=21  duration=292
  OK  --> BubbleSort_v2 (size=10): comps=49  swaps=21  duration=229
  OK  --> InsertionSort_v1 (size=10): comps=28  swaps=21  duration=149
  OK  --> InsertionSort_v2 (size=10): comps=28  swaps=21  duration=143
  OK  --> InsertionSort_v3 (size=10): comps=51  swaps=21  duration=286
  OK  --> ShellSort_v1 (size=10): comps=32  swaps=15  duration=286
  OK  --> ShellSort_v2 (size=10): comps=28  swaps=15  duration=270
  OK  --> ShellSort_v3 (size=10): comps=29  swaps=19  duration=458
   первые и последние 10 элементов массива из 100 элементов: 
   5 2 0 7 9 7 8 7 9 5 ... 5 2 0 7 9 7 8 7 9 5
  OK  --> BubbleSort_v1 (size=100): comps=4950  swaps=2401  duration=10167
  OK  --> BubbleSort_v2 (size=100): comps=4959  swaps=2401  duration=9287
  OK  --> InsertionSort_v1 (size=100): comps=2498  swaps=2401  duration=3458
  OK  --> InsertionSort_v2 (size=100): comps=2498  swaps=2401  duration=3467
  OK  --> InsertionSort_v3 (size=100): comps=1161  swaps=2401  duration=4221
  OK  --> ShellSort_v1 (size=100): comps=691  swaps=228  duration=2796
  OK  --> ShellSort_v2 (size=100): comps=644  swaps=208  duration=2277
  OK  --> ShellSort_v3 (size=100): comps=623  swaps=219  duration=3929
   первые и последние 10 элементов массива из 1000 элементов: 
   2 9 8 9 4 8 6 8 5 5 ... 2 9 8 9 4 8 6 8 5 5
  OK  --> BubbleSort_v1 (size=1000): comps=499500  swaps=228217  duration=615883
  OK  --> BubbleSort_v2 (size=1000): comps=495450  swaps=228217  duration=604832
  OK  --> InsertionSort_v1 (size=1000): comps=229214  swaps=228217  duration=337668
  OK  --> InsertionSort_v2 (size=1000): comps=229214  swaps=228217  duration=344793
  OK  --> InsertionSort_v3 (size=1000): comps=18261  swaps=228217  duration=206329
  OK  --> ShellSort_v1 (size=1000): comps=10449  swaps=2863  duration=33573
  OK  --> ShellSort_v2 (size=1000): comps=9991  swaps=2458  duration=26412
  OK  --> ShellSort_v3 (size=1000): comps=9726  swaps=2226  duration=30018
   первые и последние 10 элементов массива из 10000 элементов: 
   1 0 8 4 8 9 9 4 4 7 ... 1 0 8 4 8 9 9 4 4 7
  OK  --> BubbleSort_v1 (size=10000): comps=49995000  swaps=22471502  duration=110239960
  OK  --> BubbleSort_v2 (size=10000): comps=49500490  swaps=22471502  duration=109598480
  OK  --> InsertionSort_v1 (size=10000): comps=22481500  swaps=22471502  duration=33502416
  OK  --> InsertionSort_v2 (size=10000): comps=22481500  swaps=22471502  duration=33065058
  OK  --> InsertionSort_v3 (size=10000): comps=248871  swaps=22471502  duration=12815530
  OK  --> ShellSort_v1 (size=10000): comps=153568  swaps=37573  duration=434503
  OK  --> ShellSort_v2 (size=10000): comps=135590  swaps=25870  duration=354950
  OK  --> ShellSort_v3 (size=10000): comps=129952  swaps=21427  duration=358219
   первые и последние 10 элементов массива из 100000 элементов: 
   2 7 8 2 4 7 2 0 3 4 ... 2 7 8 2 4 7 2 0 3 4
  OK  --> BubbleSort_v1 (size=100000): comps=4999950000  swaps=2246197432  duration=13744362396
  OK  --> BubbleSort_v2 (size=100000): comps=4949954955  swaps=2246197432  duration=12887093188
  OK  --> InsertionSort_v1 (size=100000): comps=2246297430  swaps=2246197432  duration=3242960102
  OK  --> InsertionSort_v2 (size=100000): comps=2246297430  swaps=2246197432  duration=3190106466
  OK  --> InsertionSort_v3 (size=100000): comps=3154353  swaps=2246197432  duration=1250181744
  OK  --> ShellSort_v1 (size=100000): comps=1860949  swaps=401432  duration=4801024
  OK  --> ShellSort_v2 (size=100000): comps=1697481  swaps=267916  duration=3657786
  OK  --> ShellSort_v3 (size=100000): comps=1635712  swaps=211740  duration=3678313
   первые и последние 10 элементов массива из 1000000 элементов: 
   5 5 4 8 9 1 7 4 8 2 ... 5 5 4 8 9 1 7 4 8 2
  FAIL  --> BubbleSort_v1 (size=1000000): comps=0  swaps=0  duration=0
  FAIL  --> BubbleSort_v2 (size=1000000): comps=0  swaps=0  duration=0
  FAIL  --> InsertionSort_v1 (size=1000000): comps=0  swaps=0  duration=0
  FAIL  --> InsertionSort_v2 (size=1000000): comps=0  swaps=0  duration=0
  FAIL  --> InsertionSort_v3 (size=1000000): comps=0  swaps=0  duration=0
  OK  --> ShellSort_v1 (size=1000000): comps=22383596  swaps=4789821  duration=62561140
  OK  --> ShellSort_v2 (size=1000000): comps=20307786  swaps=2781582  duration=49810969
  OK  --> ShellSort_v3 (size=1000000): comps=19600952  swaps=2127650  duration=48877313
   первые и последние 10 элементов массива из 10000000 элементов: 
   3 4 5 5 4 7 4 9 5 3 ... 3 4 5 5 4 7 4 9 5 3
  FAIL  --> BubbleSort_v1 (size=10000000): comps=0  swaps=0  duration=0
  FAIL  --> BubbleSort_v2 (size=10000000): comps=0  swaps=0  duration=0
  FAIL  --> InsertionSort_v1 (size=10000000): comps=0  swaps=0  duration=0
  FAIL  --> InsertionSort_v2 (size=10000000): comps=0  swaps=0  duration=0
  FAIL  --> InsertionSort_v3 (size=10000000): comps=0  swaps=0  duration=0
  OK  --> ShellSort_v1 (size=10000000): comps=270196822  swaps=54253917  duration=672240333
  OK  --> ShellSort_v2 (size=10000000): comps=237536810  swaps=28239358  duration=500466428
  OK  --> ShellSort_v3 (size=10000000): comps=229224325  swaps=21272122  duration=509769897

Запускаем тест группы: "2.sorted"
   исходный массив из 1 элементов: 
   0
  OK  --> BubbleSort_v1 (size=1): comps=0  swaps=0  duration=264
  OK  --> BubbleSort_v2 (size=1): comps=0  swaps=0  duration=55
  OK  --> InsertionSort_v1 (size=1): comps=0  swaps=0  duration=53
  OK  --> InsertionSort_v2 (size=1): comps=0  swaps=0  duration=58
  OK  --> InsertionSort_v3 (size=1): comps=0  swaps=0  duration=60
  OK  --> ShellSort_v1 (size=1): comps=0  swaps=0  duration=53
  OK  --> ShellSort_v2 (size=1): comps=0  swaps=0  duration=42
  OK  --> ShellSort_v3 (size=1): comps=0  swaps=0  duration=183
   первые и последние 5 элементов массива из 10 элементов: 
   0 6 2 3 4 ... 0 6 2 3 4
  OK  --> BubbleSort_v1 (size=10): comps=45  swaps=9  duration=232
  OK  --> BubbleSort_v2 (size=10): comps=45  swaps=9  duration=221
  OK  --> InsertionSort_v1 (size=10): comps=18  swaps=9  duration=144
  OK  --> InsertionSort_v2 (size=10): comps=18  swaps=9  duration=151
  OK  --> InsertionSort_v3 (size=10): comps=59  swaps=9  duration=249
  OK  --> ShellSort_v1 (size=10): comps=22  swaps=1  duration=147
  OK  --> ShellSort_v2 (size=10): comps=27  swaps=9  duration=214
  OK  --> ShellSort_v3 (size=10): comps=14  swaps=1  duration=489
   первые и последние 10 элементов массива из 100 элементов: 
   0 1 2 3 4 5 6 7 8 9 ... 0 1 2 3 4 5 6 7 8 9
  OK  --> BubbleSort_v1 (size=100): comps=4950  swaps=87  duration=5656
  OK  --> BubbleSort_v2 (size=100): comps=3510  swaps=87  duration=2673
  OK  --> InsertionSort_v1 (size=100): comps=186  swaps=87  duration=286
  OK  --> InsertionSort_v2 (size=100): comps=186  swaps=87  duration=336
  OK  --> InsertionSort_v3 (size=100): comps=1245  swaps=87  duration=843
  OK  --> ShellSort_v1 (size=100): comps=590  swaps=87  duration=1303
  OK  --> ShellSort_v2 (size=100): comps=511  swaps=31  duration=843
  OK  --> ShellSort_v3 (size=100): comps=517  swaps=71  duration=1806
   первые и последние 10 элементов массива из 1000 элементов: 
   0 1 2 3 4 5 6 7 8 9 ... 0 1 2 3 4 5 6 7 8 9
  OK  --> BubbleSort_v1 (size=1000): comps=499500  swaps=6604  duration=503955
  OK  --> BubbleSort_v2 (size=1000): comps=496930  swaps=6604  duration=290925
  OK  --> InsertionSort_v1 (size=1000): comps=7603  swaps=6604  duration=9507
  OK  --> InsertionSort_v2 (size=1000): comps=7603  swaps=6604  duration=9302
  OK  --> InsertionSort_v3 (size=1000): comps=18375  swaps=6604  duration=11483
  OK  --> ShellSort_v1 (size=1000): comps=10710  swaps=2722  duration=28641
  OK  --> ShellSort_v2 (size=1000): comps=10413  swaps=2446  duration=24100
  OK  --> ShellSort_v3 (size=1000): comps=10357  swaps=2450  duration=26122
   первые и последние 10 элементов массива из 10000 элементов: 
   0 1 2 3 4 5 6 7 8 9 ... 0 1 2 3 4 5 6 7 8 9
  OK  --> BubbleSort_v1 (size=10000): comps=49995000  swaps=666392  duration=50675961
  OK  --> BubbleSort_v2 (size=10000): comps=49737454  swaps=666392  duration=29697015
  OK  --> InsertionSort_v1 (size=10000): comps=676391  swaps=666392  duration=1009690
  OK  --> InsertionSort_v2 (size=10000): comps=676391  swaps=666392  duration=954438
  OK  --> InsertionSort_v3 (size=10000): comps=248665  swaps=666392  duration=421072
  OK  --> ShellSort_v1 (size=10000): comps=176309  swaps=56474  duration=521403
  OK  --> ShellSort_v2 (size=10000): comps=171387  swaps=57928  duration=485865
  OK  --> ShellSort_v3 (size=10000): comps=163917  swaps=51640  duration=512816
   первые и последние 10 элементов массива из 100000 элементов: 
   0 1 2 3 4 5 6 7 8 9 ... 0 1 2 3 4 5 6 7 8 9
  OK  --> BubbleSort_v1 (size=100000): comps=4999950000  swaps=65248998  duration=4971341279
  OK  --> BubbleSort_v2 (size=100000): comps=4983441034  swaps=65248998  duration=2916768906
  OK  --> InsertionSort_v1 (size=100000): comps=65348997  swaps=65248998  duration=94076444
  OK  --> InsertionSort_v2 (size=100000): comps=65348997  swaps=65248998  duration=92557016
  OK  --> InsertionSort_v3 (size=100000): comps=3146427  swaps=65248998  duration=38940296
  OK  --> ShellSort_v1 (size=100000): comps=2833579  swaps=1334970  duration=8533296
  OK  --> ShellSort_v2 (size=100000): comps=2509526  swaps=1042150  duration=7855763
  OK  --> ShellSort_v3 (size=100000): comps=2258426  swaps=796742  duration=8068137
   первые и последние 10 элементов массива из 1000000 элементов: 
   0 1 2 3 4 5 6 7 8 9 ... 0 1 2 3 4 5 6 7 8 9
  FAIL  --> BubbleSort_v1 (size=1000000): comps=0  swaps=0  duration=0
  FAIL  --> BubbleSort_v2 (size=1000000): comps=0  swaps=0  duration=0
  FAIL  --> InsertionSort_v1 (size=1000000): comps=0  swaps=0  duration=0
  FAIL  --> InsertionSort_v2 (size=1000000): comps=0  swaps=0  duration=0
  FAIL  --> InsertionSort_v3 (size=1000000): comps=0  swaps=0  duration=0
  OK  --> ShellSort_v1 (size=1000000): comps=38939516  swaps=20954426  duration=118670168
  OK  --> ShellSort_v2 (size=1000000): comps=37313903  swaps=19378884  duration=109677206
  OK  --> ShellSort_v3 (size=1000000): comps=28889906  swaps=11038830  duration=104989634
   первые и последние 10 элементов массива из 10000000 элементов: 
   0 1 2 3 4 5 6 7 8 9 ... 0 1 2 3 4 5 6 7 8 9
  FAIL  --> BubbleSort_v1 (size=10000000): comps=0  swaps=0  duration=0
  FAIL  --> BubbleSort_v2 (size=10000000): comps=0  swaps=0  duration=0
  FAIL  --> InsertionSort_v1 (size=10000000): comps=0  swaps=0  duration=0
  FAIL  --> InsertionSort_v2 (size=10000000): comps=0  swaps=0  duration=0
  FAIL  --> InsertionSort_v3 (size=10000000): comps=0  swaps=0  duration=0
  OK  --> ShellSort_v1 (size=10000000): comps=624139826  swaps=404287028  duration=1675783072
  OK  --> ShellSort_v2 (size=10000000): comps=580178096  swaps=367110546  duration=1538115600
  OK  --> ShellSort_v3 (size=10000000): comps=353352946  swaps=141665472  duration=1350114378

Запускаем тест группы: "3.revers"
   исходный массив из 1 элементов: 
   0
  OK  --> BubbleSort_v1 (size=1): comps=0  swaps=0  duration=225
  OK  --> BubbleSort_v2 (size=1): comps=0  swaps=0  duration=66
  OK  --> InsertionSort_v1 (size=1): comps=0  swaps=0  duration=53
  OK  --> InsertionSort_v2 (size=1): comps=0  swaps=0  duration=55
  OK  --> InsertionSort_v3 (size=1): comps=0  swaps=0  duration=57
  OK  --> ShellSort_v1 (size=1): comps=0  swaps=0  duration=58
  OK  --> ShellSort_v2 (size=1): comps=0  swaps=0  duration=44
  OK  --> ShellSort_v3 (size=1): comps=0  swaps=0  duration=155
   первые и последние 5 элементов массива из 10 элементов: 
   9 8 7 6 5 ... 9 8 7 6 5
  OK  --> BubbleSort_v1 (size=10): comps=45  swaps=45  duration=180
  OK  --> BubbleSort_v2 (size=10): comps=54  swaps=45  duration=169
  OK  --> InsertionSort_v1 (size=10): comps=45  swaps=45  duration=156
  OK  --> InsertionSort_v2 (size=10): comps=45  swaps=45  duration=160
  OK  --> InsertionSort_v3 (size=10): comps=47  swaps=45  duration=239
  OK  --> ShellSort_v1 (size=10): comps=27  swaps=13  duration=226
  OK  --> ShellSort_v2 (size=10): comps=25  swaps=11  duration=227
  OK  --> ShellSort_v3 (size=10): comps=30  swaps=25  duration=518
   первые и последние 10 элементов массива из 100 элементов: 
   99 98 97 96 95 94 93 92 91 90 ... 99 98 97 96 95 94 93 92 91 90
  OK  --> BubbleSort_v1 (size=100): comps=4950  swaps=4950  duration=6715
  OK  --> BubbleSort_v2 (size=100): comps=5049  swaps=4950  duration=6666
  OK  --> InsertionSort_v1 (size=100): comps=4950  swaps=4950  duration=7049
  OK  --> InsertionSort_v2 (size=100): comps=4950  swaps=4950  duration=6792
  OK  --> InsertionSort_v3 (size=100): comps=1059  swaps=4950  duration=3146
  OK  --> ShellSort_v1 (size=100): comps=668  swaps=260  duration=1615
  OK  --> ShellSort_v2 (size=100): comps=614  swaps=192  duration=1475
  OK  --> ShellSort_v3 (size=100): comps=626  swaps=234  duration=1888
   первые и последние 10 элементов массива из 1000 элементов: 
   999 998 997 996 995 994 993 992 991 990 ... 999 998 997 996 995 994 993 992 991 990
  OK  --> BubbleSort_v1 (size=1000): comps=499500  swaps=499500  duration=697457
  OK  --> BubbleSort_v2 (size=1000): comps=500499  swaps=499500  duration=695833
  OK  --> InsertionSort_v1 (size=1000): comps=499500  swaps=499500  duration=728555
  OK  --> InsertionSort_v2 (size=1000): comps=499500  swaps=499500  duration=727954
  OK  --> InsertionSort_v3 (size=1000): comps=16973  swaps=499500  duration=271979
  OK  --> ShellSort_v1 (size=1000): comps=11716  swaps=4700  duration=17640
  OK  --> ShellSort_v2 (size=1000): comps=10511  swaps=3424  duration=13349
  OK  --> ShellSort_v3 (size=1000): comps=10788  swaps=3690  duration=17128
   первые и последние 10 элементов массива из 10000 элементов: 
   9999 9998 9997 9996 9995 9994 9993 9992 9991 9990 ... 9999 9998 9997 9996 9995 9994 9993 9992 9991 9990
  OK  --> BubbleSort_v1 (size=10000): comps=49995000  swaps=49995000  duration=70930830
  OK  --> BubbleSort_v2 (size=10000): comps=50004999  swaps=49995000  duration=71021125
  OK  --> InsertionSort_v1 (size=10000): comps=49995000  swaps=49995000  duration=72662527
  OK  --> InsertionSort_v2 (size=10000): comps=49995000  swaps=49995000  duration=71445614
  OK  --> InsertionSort_v3 (size=10000): comps=237261  swaps=49995000  duration=27412763
  OK  --> ShellSort_v1 (size=10000): comps=172578  swaps=62560  duration=249533
  OK  --> ShellSort_v2 (size=10000): comps=144824  swaps=36778  duration=171883
  OK  --> ShellSort_v3 (size=10000): comps=148103  swaps=41884  duration=203059
   первые и последние 10 элементов массива из 100000 элементов: 
   99999 99998 99997 99996 99995 99994 99993 99992 99991 99990 ... 99999 99998 99997 99996 99995 99994 99993 99992 99991 99990
  OK  --> BubbleSort_v1 (size=100000): comps=4999950000  swaps=4999950000  duration=6931270605
  OK  --> BubbleSort_v2 (size=100000): comps=5000049999  swaps=4999950000  duration=6909690507
  OK  --> InsertionSort_v1 (size=100000): comps=4999950000  swaps=4999950000  duration=7147750208
  OK  --> InsertionSort_v2 (size=100000): comps=4999950000  swaps=4999950000  duration=7035232864
  OK  --> InsertionSort_v3 (size=100000): comps=3037891  swaps=4999950000  duration=2802036564
  OK  --> ShellSort_v1 (size=100000): comps=2244585  swaps=844560  duration=3040089
  OK  --> ShellSort_v2 (size=100000): comps=1865861  swaps=458016  duration=2069233
  OK  --> ShellSort_v3 (size=100000): comps=1941773  swaps=542104  duration=2498738
   первые и последние 10 элементов массива из 1000000 элементов: 
   999999 999998 999997 999996 999995 999994 999993 999992 999991 999990 ... 999999 999998 999997 999996 999995 999994 999993 999992 999991 999990
  FAIL  --> BubbleSort_v1 (size=1000000): comps=0  swaps=0  duration=0
  FAIL  --> BubbleSort_v2 (size=1000000): comps=0  swaps=0  duration=0
  FAIL  --> InsertionSort_v1 (size=1000000): comps=0  swaps=0  duration=0
  FAIL  --> InsertionSort_v2 (size=1000000): comps=0  swaps=0  duration=0
  FAIL  --> InsertionSort_v3 (size=1000000): comps=0  swaps=0  duration=0
  OK  --> ShellSort_v1 (size=1000000): comps=26357530  swaps=9357504  duration=38942443
  OK  --> ShellSort_v2 (size=1000000): comps=23369384  swaps=6236456  duration=28684760
  OK  --> ShellSort_v3 (size=1000000): comps=22864392  swaps=5662150  duration=35029751
   первые и последние 10 элементов массива из 10000000 элементов: 
   9999999 9999998 9999997 9999996 9999995 9999994 9999993 9999992 9999991 9999990 ... 9999999 9999998 9999997 9999996 9999995 9999994 9999993 9999992 9999991 9999990
  FAIL  --> BubbleSort_v1 (size=10000000): comps=0  swaps=0  duration=0
  FAIL  --> BubbleSort_v2 (size=10000000): comps=0  swaps=0  duration=0
  FAIL  --> InsertionSort_v1 (size=10000000): comps=0  swaps=0  duration=0
  FAIL  --> InsertionSort_v2 (size=10000000): comps=0  swaps=0  duration=0
  FAIL  --> InsertionSort_v3 (size=10000000): comps=0  swaps=0  duration=0
  OK  --> ShellSort_v1 (size=10000000): comps=317626219  swaps=107626176  duration=454018510
  OK  --> ShellSort_v2 (size=10000000): comps=270522361  swaps=62945972  duration=332770287
  OK  --> ShellSort_v3 (size=10000000): comps=270520217  swaps=65109280  duration=369657789

----------- [Group name: "0.random"] -----------
Name                    Test #      Processed     Array size    Comparisons          Swaps       Duration (ns)
BubbleSort_v1           Test 0      processed              1              0              0                 390
BubbleSort_v2           Test 0      processed              1              0              0                  90
InsertionSort_v1        Test 0      processed              1              0              0                  83
InsertionSort_v2        Test 0      processed              1              0              0                  93
InsertionSort_v3        Test 0      processed              1              0              0                  92
ShellSort_v1            Test 0      processed              1              0              0                  87
ShellSort_v2            Test 0      processed              1              0              0                  86
ShellSort_v3            Test 0      processed              1              0              0                 120

BubbleSort_v1           Test 1      processed             10             45             16                 430
BubbleSort_v2           Test 1      processed             10             52             16                 423
InsertionSort_v1        Test 1      processed             10             24             16                 249
InsertionSort_v2        Test 1      processed             10             24             16                 281
InsertionSort_v3        Test 1      processed             10             55             16                 452
ShellSort_v1            Test 1      processed             10             29             10                 421
ShellSort_v2            Test 1      processed             10             25             10                 343
ShellSort_v3            Test 1      processed             10             23             12                 812

BubbleSort_v1           Test 2      processed            100           4950           2157               17183
BubbleSort_v2           Test 2      processed            100           4819           2157               15335
InsertionSort_v1        Test 2      processed            100           2253           2157                5109
InsertionSort_v2        Test 2      processed            100           2253           2157                5064
InsertionSort_v3        Test 2      processed            100           1153           2157                6604
ShellSort_v1            Test 2      processed            100            877            413                6031
ShellSort_v2            Test 2      processed            100            834            403                5178
ShellSort_v3            Test 2      processed            100            734            331                6075

BubbleSort_v1           Test 3      processed           1000         499500         247489             1134920
BubbleSort_v2           Test 3      processed           1000         499372         247489             1137172
InsertionSort_v1        Test 3      processed           1000         248479         247489              569831
InsertionSort_v2        Test 3      processed           1000         248479         247489              561262
InsertionSort_v3        Test 3      processed           1000          18169         247489              271499
ShellSort_v1            Test 3      processed           1000          15296           7815               99880
ShellSort_v2            Test 3      processed           1000          14379           6975               80295
ShellSort_v3            Test 3      processed           1000          14113           6741               95675

BubbleSort_v1           Test 4      processed          10000       49995000       25288624           119710144
BubbleSort_v2           Test 4      processed          10000       49997497       25288624           114299144
InsertionSort_v1        Test 4      processed          10000       25298615       25288624            38083206
InsertionSort_v2        Test 4      processed          10000       25298615       25288624            37616849
InsertionSort_v3        Test 4      processed          10000         247903       25288624            14605905
ShellSort_v1            Test 4      processed          10000         267306         152486              961976
ShellSort_v2            Test 4      processed          10000         255750         147412              794171
ShellSort_v3            Test 4      processed          10000         203477          96320              840091

BubbleSort_v1           Test 5      processed         100000     4999950000     2501737763         13944874101
BubbleSort_v2           Test 5      processed         100000     5000040547     2501737763         13228193306
InsertionSort_v1        Test 5      processed         100000     2501837753     2501737763          3615766050
InsertionSort_v2        Test 5      processed         100000     2501837753     2501737763          3556630061
InsertionSort_v3        Test 5      processed         100000        3145309     2501737763          1401051870
ShellSort_v1            Test 5      processed         100000        4562610        3113279            13522918
ShellSort_v2            Test 5      processed         100000        3920114        2503799            11292352
ShellSort_v3            Test 5      processed         100000        2677097        1266093            11825161

BubbleSort_v1           Test 6        skipped        1000000              0              0                   0
BubbleSort_v2           Test 6        skipped        1000000              0              0                   0
InsertionSort_v1        Test 6        skipped        1000000              0              0                   0
InsertionSort_v2        Test 6        skipped        1000000              0              0                   0
InsertionSort_v3        Test 6        skipped        1000000              0              0                   0
ShellSort_v1            Test 6      processed        1000000       68027835       50530714           184869860
ShellSort_v2            Test 6      processed        1000000       62283286       44889440           163891887
ShellSort_v3            Test 6      processed        1000000       33226631       15880632           147284701

BubbleSort_v1           Test 7        skipped       10000000              0              0                   0
BubbleSort_v2           Test 7        skipped       10000000              0              0                   0
InsertionSort_v1        Test 7        skipped       10000000              0              0                   0
InsertionSort_v2        Test 7        skipped       10000000              0              0                   0
InsertionSort_v3        Test 7        skipped       10000000              0              0                   0
ShellSort_v1            Test 7      processed       10000000     1041605900      826620860          2527025336
ShellSort_v2            Test 7      processed       10000000     1040073945      832098474          2601588816
ShellSort_v3            Test 7      processed       10000000      398594255      191906810          1754602365


----------- [Group name: "1.digits"] -----------
Name                    Test #      Processed     Array size    Comparisons          Swaps       Duration (ns)
BubbleSort_v1           Test 0      processed              1              0              0                 163
BubbleSort_v2           Test 0      processed              1              0              0                  69
InsertionSort_v1        Test 0      processed              1              0              0                  51
InsertionSort_v2        Test 0      processed              1              0              0                  54
InsertionSort_v3        Test 0      processed              1              0              0                  57
ShellSort_v1            Test 0      processed              1              0              0                  60
ShellSort_v2            Test 0      processed              1              0              0                  43
ShellSort_v3            Test 0      processed              1              0              0                 163

BubbleSort_v1           Test 1      processed             10             45             21                 292
BubbleSort_v2           Test 1      processed             10             49             21                 229
InsertionSort_v1        Test 1      processed             10             28             21                 149
InsertionSort_v2        Test 1      processed             10             28             21                 143
InsertionSort_v3        Test 1      processed             10             51             21                 286
ShellSort_v1            Test 1      processed             10             32             15                 286
ShellSort_v2            Test 1      processed             10             28             15                 270
ShellSort_v3            Test 1      processed             10             29             19                 458

BubbleSort_v1           Test 2      processed            100           4950           2401               10167
BubbleSort_v2           Test 2      processed            100           4959           2401                9287
InsertionSort_v1        Test 2      processed            100           2498           2401                3458
InsertionSort_v2        Test 2      processed            100           2498           2401                3467
InsertionSort_v3        Test 2      processed            100           1161           2401                4221
ShellSort_v1            Test 2      processed            100            691            228                2796
ShellSort_v2            Test 2      processed            100            644            208                2277
ShellSort_v3            Test 2      processed            100            623            219                3929

BubbleSort_v1           Test 3      processed           1000         499500         228217              615883
BubbleSort_v2           Test 3      processed           1000         495450         228217              604832
InsertionSort_v1        Test 3      processed           1000         229214         228217              337668
InsertionSort_v2        Test 3      processed           1000         229214         228217              344793
InsertionSort_v3        Test 3      processed           1000          18261         228217              206329
ShellSort_v1            Test 3      processed           1000          10449           2863               33573
ShellSort_v2            Test 3      processed           1000           9991           2458               26412
ShellSort_v3            Test 3      processed           1000           9726           2226               30018

BubbleSort_v1           Test 4      processed          10000       49995000       22471502           110239960
BubbleSort_v2           Test 4      processed          10000       49500490       22471502           109598480
InsertionSort_v1        Test 4      processed          10000       22481500       22471502            33502416
InsertionSort_v2        Test 4      processed          10000       22481500       22471502            33065058
InsertionSort_v3        Test 4      processed          10000         248871       22471502            12815530
ShellSort_v1            Test 4      processed          10000         153568          37573              434503
ShellSort_v2            Test 4      processed          10000         135590          25870              354950
ShellSort_v3            Test 4      processed          10000         129952          21427              358219

BubbleSort_v1           Test 5      processed         100000     4999950000     2246197432         13744362396
BubbleSort_v2           Test 5      processed         100000     4949954955     2246197432         12887093188
InsertionSort_v1        Test 5      processed         100000     2246297430     2246197432          3242960102
InsertionSort_v2        Test 5      processed         100000     2246297430     2246197432          3190106466
InsertionSort_v3        Test 5      processed         100000        3154353     2246197432          1250181744
ShellSort_v1            Test 5      processed         100000        1860949         401432             4801024
ShellSort_v2            Test 5      processed         100000        1697481         267916             3657786
ShellSort_v3            Test 5      processed         100000        1635712         211740             3678313

BubbleSort_v1           Test 6        skipped        1000000              0              0                   0
BubbleSort_v2           Test 6        skipped        1000000              0              0                   0
InsertionSort_v1        Test 6        skipped        1000000              0              0                   0
InsertionSort_v2        Test 6        skipped        1000000              0              0                   0
InsertionSort_v3        Test 6        skipped        1000000              0              0                   0
ShellSort_v1            Test 6      processed        1000000       22383596        4789821            62561140
ShellSort_v2            Test 6      processed        1000000       20307786        2781582            49810969
ShellSort_v3            Test 6      processed        1000000       19600952        2127650            48877313

BubbleSort_v1           Test 7        skipped       10000000              0              0                   0
BubbleSort_v2           Test 7        skipped       10000000              0              0                   0
InsertionSort_v1        Test 7        skipped       10000000              0              0                   0
InsertionSort_v2        Test 7        skipped       10000000              0              0                   0
InsertionSort_v3        Test 7        skipped       10000000              0              0                   0
ShellSort_v1            Test 7      processed       10000000      270196822       54253917           672240333
ShellSort_v2            Test 7      processed       10000000      237536810       28239358           500466428
ShellSort_v3            Test 7      processed       10000000      229224325       21272122           509769897


----------- [Group name: "2.sorted"] -----------
Name                    Test #      Processed     Array size    Comparisons          Swaps       Duration (ns)
BubbleSort_v1           Test 0      processed              1              0              0                 264
BubbleSort_v2           Test 0      processed              1              0              0                  55
InsertionSort_v1        Test 0      processed              1              0              0                  53
InsertionSort_v2        Test 0      processed              1              0              0                  58
InsertionSort_v3        Test 0      processed              1              0              0                  60
ShellSort_v1            Test 0      processed              1              0              0                  53
ShellSort_v2            Test 0      processed              1              0              0                  42
ShellSort_v3            Test 0      processed              1              0              0                 183

BubbleSort_v1           Test 1      processed             10             45              9                 232
BubbleSort_v2           Test 1      processed             10             45              9                 221
InsertionSort_v1        Test 1      processed             10             18              9                 144
InsertionSort_v2        Test 1      processed             10             18              9                 151
InsertionSort_v3        Test 1      processed             10             59              9                 249
ShellSort_v1            Test 1      processed             10             22              1                 147
ShellSort_v2            Test 1      processed             10             27              9                 214
ShellSort_v3            Test 1      processed             10             14              1                 489

BubbleSort_v1           Test 2      processed            100           4950             87                5656
BubbleSort_v2           Test 2      processed            100           3510             87                2673
InsertionSort_v1        Test 2      processed            100            186             87                 286
InsertionSort_v2        Test 2      processed            100            186             87                 336
InsertionSort_v3        Test 2      processed            100           1245             87                 843
ShellSort_v1            Test 2      processed            100            590             87                1303
ShellSort_v2            Test 2      processed            100            511             31                 843
ShellSort_v3            Test 2      processed            100            517             71                1806

BubbleSort_v1           Test 3      processed           1000         499500           6604              503955
BubbleSort_v2           Test 3      processed           1000         496930           6604              290925
InsertionSort_v1        Test 3      processed           1000           7603           6604                9507
InsertionSort_v2        Test 3      processed           1000           7603           6604                9302
InsertionSort_v3        Test 3      processed           1000          18375           6604               11483
ShellSort_v1            Test 3      processed           1000          10710           2722               28641
ShellSort_v2            Test 3      processed           1000          10413           2446               24100
ShellSort_v3            Test 3      processed           1000          10357           2450               26122

BubbleSort_v1           Test 4      processed          10000       49995000         666392            50675961
BubbleSort_v2           Test 4      processed          10000       49737454         666392            29697015
InsertionSort_v1        Test 4      processed          10000         676391         666392             1009690
InsertionSort_v2        Test 4      processed          10000         676391         666392              954438
InsertionSort_v3        Test 4      processed          10000         248665         666392              421072
ShellSort_v1            Test 4      processed          10000         176309          56474              521403
ShellSort_v2            Test 4      processed          10000         171387          57928              485865
ShellSort_v3            Test 4      processed          10000         163917          51640              512816

BubbleSort_v1           Test 5      processed         100000     4999950000       65248998          4971341279
BubbleSort_v2           Test 5      processed         100000     4983441034       65248998          2916768906
InsertionSort_v1        Test 5      processed         100000       65348997       65248998            94076444
InsertionSort_v2        Test 5      processed         100000       65348997       65248998            92557016
InsertionSort_v3        Test 5      processed         100000        3146427       65248998            38940296
ShellSort_v1            Test 5      processed         100000        2833579        1334970             8533296
ShellSort_v2            Test 5      processed         100000        2509526        1042150             7855763
ShellSort_v3            Test 5      processed         100000        2258426         796742             8068137

BubbleSort_v1           Test 6        skipped        1000000              0              0                   0
BubbleSort_v2           Test 6        skipped        1000000              0              0                   0
InsertionSort_v1        Test 6        skipped        1000000              0              0                   0
InsertionSort_v2        Test 6        skipped        1000000              0              0                   0
InsertionSort_v3        Test 6        skipped        1000000              0              0                   0
ShellSort_v1            Test 6      processed        1000000       38939516       20954426           118670168
ShellSort_v2            Test 6      processed        1000000       37313903       19378884           109677206
ShellSort_v3            Test 6      processed        1000000       28889906       11038830           104989634

BubbleSort_v1           Test 7        skipped       10000000              0              0                   0
BubbleSort_v2           Test 7        skipped       10000000              0              0                   0
InsertionSort_v1        Test 7        skipped       10000000              0              0                   0
InsertionSort_v2        Test 7        skipped       10000000              0              0                   0
InsertionSort_v3        Test 7        skipped       10000000              0              0                   0
ShellSort_v1            Test 7      processed       10000000      624139826      404287028          1675783072
ShellSort_v2            Test 7      processed       10000000      580178096      367110546          1538115600
ShellSort_v3            Test 7      processed       10000000      353352946      141665472          1350114378


----------- [Group name: "3.revers"] -----------
Name                    Test #      Processed     Array size    Comparisons          Swaps       Duration (ns)
BubbleSort_v1           Test 0      processed              1              0              0                 225
BubbleSort_v2           Test 0      processed              1              0              0                  66
InsertionSort_v1        Test 0      processed              1              0              0                  53
InsertionSort_v2        Test 0      processed              1              0              0                  55
InsertionSort_v3        Test 0      processed              1              0              0                  57
ShellSort_v1            Test 0      processed              1              0              0                  58
ShellSort_v2            Test 0      processed              1              0              0                  44
ShellSort_v3            Test 0      processed              1              0              0                 155

BubbleSort_v1           Test 1      processed             10             45             45                 180
BubbleSort_v2           Test 1      processed             10             54             45                 169
InsertionSort_v1        Test 1      processed             10             45             45                 156
InsertionSort_v2        Test 1      processed             10             45             45                 160
InsertionSort_v3        Test 1      processed             10             47             45                 239
ShellSort_v1            Test 1      processed             10             27             13                 226
ShellSort_v2            Test 1      processed             10             25             11                 227
ShellSort_v3            Test 1      processed             10             30             25                 518

BubbleSort_v1           Test 2      processed            100           4950           4950                6715
BubbleSort_v2           Test 2      processed            100           5049           4950                6666
InsertionSort_v1        Test 2      processed            100           4950           4950                7049
InsertionSort_v2        Test 2      processed            100           4950           4950                6792
InsertionSort_v3        Test 2      processed            100           1059           4950                3146
ShellSort_v1            Test 2      processed            100            668            260                1615
ShellSort_v2            Test 2      processed            100            614            192                1475
ShellSort_v3            Test 2      processed            100            626            234                1888

BubbleSort_v1           Test 3      processed           1000         499500         499500              697457
BubbleSort_v2           Test 3      processed           1000         500499         499500              695833
InsertionSort_v1        Test 3      processed           1000         499500         499500              728555
InsertionSort_v2        Test 3      processed           1000         499500         499500              727954
InsertionSort_v3        Test 3      processed           1000          16973         499500              271979
ShellSort_v1            Test 3      processed           1000          11716           4700               17640
ShellSort_v2            Test 3      processed           1000          10511           3424               13349
ShellSort_v3            Test 3      processed           1000          10788           3690               17128

BubbleSort_v1           Test 4      processed          10000       49995000       49995000            70930830
BubbleSort_v2           Test 4      processed          10000       50004999       49995000            71021125
InsertionSort_v1        Test 4      processed          10000       49995000       49995000            72662527
InsertionSort_v2        Test 4      processed          10000       49995000       49995000            71445614
InsertionSort_v3        Test 4      processed          10000         237261       49995000            27412763
ShellSort_v1            Test 4      processed          10000         172578          62560              249533
ShellSort_v2            Test 4      processed          10000         144824          36778              171883
ShellSort_v3            Test 4      processed          10000         148103          41884              203059

BubbleSort_v1           Test 5      processed         100000     4999950000     4999950000          6931270605
BubbleSort_v2           Test 5      processed         100000     5000049999     4999950000          6909690507
InsertionSort_v1        Test 5      processed         100000     4999950000     4999950000          7147750208
InsertionSort_v2        Test 5      processed         100000     4999950000     4999950000          7035232864
InsertionSort_v3        Test 5      processed         100000        3037891     4999950000          2802036564
ShellSort_v1            Test 5      processed         100000        2244585         844560             3040089
ShellSort_v2            Test 5      processed         100000        1865861         458016             2069233
ShellSort_v3            Test 5      processed         100000        1941773         542104             2498738

BubbleSort_v1           Test 6        skipped        1000000              0              0                   0
BubbleSort_v2           Test 6        skipped        1000000              0              0                   0
InsertionSort_v1        Test 6        skipped        1000000              0              0                   0
InsertionSort_v2        Test 6        skipped        1000000              0              0                   0
InsertionSort_v3        Test 6        skipped        1000000              0              0                   0
ShellSort_v1            Test 6      processed        1000000       26357530        9357504            38942443
ShellSort_v2            Test 6      processed        1000000       23369384        6236456            28684760
ShellSort_v3            Test 6      processed        1000000       22864392        5662150            35029751

BubbleSort_v1           Test 7        skipped       10000000              0              0                   0
BubbleSort_v2           Test 7        skipped       10000000              0              0                   0
InsertionSort_v1        Test 7        skipped       10000000              0              0                   0
InsertionSort_v2        Test 7        skipped       10000000              0              0                   0
InsertionSort_v3        Test 7        skipped       10000000              0              0                   0
ShellSort_v1            Test 7      processed       10000000      317626219      107626176           454018510
ShellSort_v2            Test 7      processed       10000000      270522361       62945972           332770287
ShellSort_v3            Test 7      processed       10000000      270520217       65109280           369657789

```

Т.к. BubbleSort и InsertionSort на больших показателях N начинают работать очень медленно, я ограничил для них N, от 1000000 и более они уже пропускают работу.

Сразу отметим и не будем говорить ниже о том, что на больших N за разумное время справляются только алгоритмы ShellSort, и о том, что с увеличением N алгоритмы ShellSort заметно выигрывают не только по времени, но и уходят в отрыв по меньшей сложности в сравнениях и перестановках.

Также сразу отметим, что BubbleSort чертовски медленный. Где это будет разумно, буду сравнивать только чуть более производительный InsertionSort.

##### Результаты теста с Random распределением

Заметна разница в скорости между InsertionSort_v3 и ShellSort_v3 почти в 110 раз. Потому что в ShellSort сложность O(n log n), в то время как InsertionSort имеет O(n^2).  
Итого, на полностью случайных данных **ShellSort** быстрее **InsertionSort** в 100+ раз уже на N=100000.

##### Результаты теста с Digits распределением (особенность в том, что очень мало случайных значений)

Тут интересное поведение прослеживается: на данных с повторениями (мало уникальных) ShellSort работает даже эффективнее, чем на случайных. Операций сравнений `1635712` (вместо `2677097`), а обменов `211740` (вместо `1266093`).  
Заметна разница в скорости между InsertionSort_v3 и ShellSort_v3 почти в 340 раз.  
Я это объясняю тем, что когда много одинаковых элементов, это означает, что при сортировке вставками (ShellSort у нас в реализации использует InsertionSort_v2 на каждом шаге) элементы часто уже стоят на своих местах или близко к ним.

##### Результаты теста с Sorted распределением (особенность в том, что уже почти отсортированный массив)

При N=100000, InsertionSort_v3 занял 0.039 сек, BubbleSort_v2 занял 2.92 сек, ShellSort_v3 занял 0.008 сек.
InsertionSort на почти отсортированных данных работает за O(n), если операций сравнений почти те же `3146427` (вместо `3145309`), то операций обменов `65248998` (вместо `2501737763`)
А вот BubbleSort на почти отсортированном притормозил. Видимо дело в том, что даже 1% неотсортированности на больших N - это очень много, и заставляет тащить числа через весь массив и что хуже - создают каскадный эффект перестановок.  
Итого, **InsertionSort** практически идеальный алгоритм для почти отсортированных массивов.

##### Результаты теста с Revers распределением

А вот тут большая разница. **InsertionSort** в худшем случае делает максимальное количество сравнений и обменов.  
**ShellSort** на обратных данных работает даже лучше, чем на случайных. Операций сравнений `1941773` (вместо `2677097`), а операций обменов `542104` (вместо `1266093`).  
Я это объясняю тем, что ShellSort на первом же большом шаге сразу ставит много элементов близко к их конечным позициям, т.о. для него обратный порядок не проблема.

##### Дополнительные моменты анализа

BubbleSort_v2 на отсортированных данных быстрее чем BubbleSort_v1 где-то в 1,7 раз. Однако на случайных данных разницы нет.

Сравнивая InsertionSort_v1, InsertionSort_v2 и InsertionSort_v3 можно заметить, что бинарный поиск дает ускорение больше чем в 2 раза, хотя операций обменов остается столько же, но операций обменов сильно сокращается. InsertionSort_v3 является лучше реализацией из этих трех.

Сравнивая ShellSort_v1, ShellSort_v2 и ShellSort_v3 интересно заметить, что ShellSort_v3 начинает раскрываться, дает ускорение почти в 1,5 раза, сокращает операции сравнений и обменов только на больших N. Хотя интересно, что при N=10000000 на обратных данных, ShellSort_v2 отработал за 0.333 сек, а ShellSort_v3 за 0.370 сек. Наверное дело в алгоритме формирования gap.

Выводы:

* если почти отсортированные данные: отлично подходит InsertionSort (у него O(n))
* если мало уникальных значений (0-9): хорошо показывает себя ShellSort, особенно v3 на больших N
* если очень маленькие массивы (N < 100): нет разницы, но лучше подойдет BubbleSort из-за максимальной простоты, или InsertionSort, т.к. и простой и чуть быстрее
* если случайные данные  большого размера (N > 1000): лучше ShellSort, а чем больше N, тем больше можно посмотреть на ShellSort_v3
* если обратный порядок данных: лучше ShellSort и с увеличением N неожиданно лучше именно ShellSort_v2
