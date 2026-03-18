# ДЗ 08: Алгоритмы сортировки (QuickSort, MergeSort, ExternalSort)

## Цель

Создание, тестирование и сравнение алгоритмов сортировки:

* сортировка быстрая (QuickSort)
* сортировка слиянием (MergeSort)
* сортировка внешняя (ExternalSort)

## Подготовка

Основные характеристики алгоритмов сортировки:

1. Стабильность - когда алгоритм не меняет порядок элементов с одинаковыми ключами (устойчивость). Обычно имеет смысл при сортировке объектов/записей, а не отдельных чисел.
2. Адаптивность - когда алгоритм сортировки предварительного отсортированного массива выполняется значительно быстрее, т.е. адаптируется к данным.
3. On-line - когда алгоритм анализирует данные последовательно получая элементы, а не на изначально полном объеме данных, т.е. для каждого нового элемента определяется его положение в уже отсортированном массиве.

| | QuickSort | MergeSort |
| -- | -- | -- |
| Сложность | в лучшем O(N log N), в худшем O(N^2) | O(N log N) |
| Стабильность | - | + |
| Адаптивность | - | - |
| On-line | - | - |

Для сборки исходников запустить `make`:

```bash
dmitry@lachugin:~/otus/lachugin_algorithms_hw/hw08$ make
g++ -lstdc++fs -std=c++17 -O2 -Wall -I. -o hw08_sort main_sort.cpp QuickSort.cpp MergeSort.cpp
g++ -lstdc++fs -std=c++17 -O2 -Wall -I. -o hw08_external_sort main_external_sort.cpp ExternalSort.cpp InsertionSort.cpp
g++ -lstdc++fs -std=c++17 -O2 -Wall -I. -o hw08_checker checker.cpp checker_internals.cpp BubbleSort.cpp InsertionSort.cpp ShellSort.cpp SelectionSort.cpp HeapSort.cpp QuickSort.cpp MergeSort.cpp
```

Основными исследуемыми характеристиками алгоритмов сортировки являются:

* время выполнения сортировки массива размером N (**Duration (ns)**)
* количество операций сравнений (**Comparisons**)
* количество операций перестановки элементов (**Swaps**)

Алгоритмы организованы в классы, унаследованные от базового интерфейсного класса **ISort**, это позволит легче производить тестирование, а также спрятать в базовом классе некоторые общие вещи.

**TODO:** Нужно распаковать архив `sorting_tests.zip` с тестами в каталог `hw08/sorting-tests`.

## Решение

### Уровень JUNIOR

#### QS1

Реализация алгоритма в файле `hw08/QuickSort.cpp` в методе **sort_v1** (продолжается в **sort_recursive** и **split**).

Определяем опорный элемент в массиве между границами low и high, т.о. делим массив на две части и рекурсивно уходим обрабатывать каждую из них. В итоге массив рекурсивно делится на части с элементами меньше опорного и больше опорного, в каждом подмассиве элементы сортируются.

На самом деле тут всё очень сильно зависит от выбора опорного элемента. Его можно выбрать как левый (первый) элемент в диапазоне от low до high, его можно выбрать как правый (последний) элемент в диапазоне от low до high,  его можно выбрать как средний элемент в диапазоне от low до high, еще какие-то варианты (например, медианный из этих трех выше). От этого будет зависеть результативность алгоритма для конкретного массива. В нашей реализации за опорный элемент выбирается правый в диапазоне.

#### MS2

Реализация алгоритма в файле `hw08/MergeSort.cpp` в методе **sort_v1** (продолжается в **sort_recursive** и **merge**).

Идем рекурсивно сверху вниз, делим массив пополам. рекурсия уходит вглубь, пока в половинке не останется по 1 элементу. После дробления массива на половинки, рекурсия начинает разворачиваться и эти пары половинок (начиная с маленьких и всё больше и больше) - начинают объединяться через дополнительную память, выстраивая элементы в правильной восходящей последовательности.

#### Сравнительный анализ базовых реализаций

Тестировались алгоритмы: **QuickSort_v1**, **MergeSort_v1**.

Тестирование проводилось на наборах массивов нескольких размеров: **100**, **1000**, **10000**, **100000**.  
Не проводил тестирование на размере массива 1000000, т.к. уже были получены показательные результаты и на меньшем объеме данных.

Для теста создавался один массив максимального размера, заполнялся произвольными числами от 0 до 99.  
Затем, все варианты массивов меньшего размера формировались путём создания копии первых N элементов из массива максимального размера.  
А также для каждого подопытного алгоритма сортировки формировалась своя копия исходного массива произвольных чисел.  
Таким образом старался привести эксперимент ближе к одинаковым условиям.

После запуска программы, она сформирует таблицу-отчет:

```bash
dmitry@lachugin:~/otus/lachugin_algorithms_hw/hw08$ ./hw08_sort 
программа оценки работы алгоритмов сортировки (QuickSort, MergeSort) в базовых версиях
Первые и последние 20 элементов массива из 100000 элементов: 
   99 21 73 51 21 4 37 2 28 28 27 97 21 53 6 73 38 12 57 12 ... 19 69 43 17 2 86 72 45 13 19 91 77 27 46 9 43 57 58 49 48
Name                     Array size    Comparisons          Swaps       Duration (ns)
QuickSort_v1                    100            804            356                4537
MergeSort_v1                    100            549            672                7653
QuickSort_v1                   1000          13418           3945               40745
MergeSort_v1                   1000           8721           9976               67429
QuickSort_v1                  10000         586601          33895              862455
MergeSort_v1                  10000         120329         133616              768773
QuickSort_v1                 100000       50888579         393285            50454688
MergeSort_v1                 100000        1532808        1668928             6174977
```

Видим, что **MergeSort** подтверждает сложность O(N log N) с увеличением N. В тоже время **QuickSort** ведет себя по разному с увеличением N. Т.к. для этого алгоритма всё зависит от конкретного массива и выбора опорного элемента. В лучших случаях он мог выдавать O(N log N), а в худших O(N^2).

Также наблюдаем, что **MergeSort** с увеличением N работает быстрее, даже при учете, что ему требуется выделение дополнительной памяти, перекопирование элементов туда и обратно. Получается что эта дополнительная накладная сложность оказывает заметное влияние на малых N, но ля больших N уже оказывает не такое влияние.

### Уровень SENIOR

#### Комплексное тестирование алгоритмов

Для тестов используются наборы данных с несколькими различными характеристиками:

* random — массив из случайных чисел (`hw08/sorting-tests/0.random`)
* digits — массив из случайных цифр (`hw08/sorting-tests/1.digits`)
* sorted — на 99% отсортированный массив (`hw08/sorting-tests/2.sorted`)
* revers — обратно-отсортированный массив (`hw08/sorting-tests/3.revers`)

Написана система тестирования.

Тестируются все алгоритмы (включая алгоритмы из прошлых ДЗ с их оптимизациями): **BubbleSort_v1**, **BubbleSort_v2**, **InsertionSort_v1**, **InsertionSort_v2**, **InsertionSort_v3**, **ShellSort_v1**, **ShellSort_v2**, **ShellSort_v3**, **SelectionSort_v1**, **HeapSort_v1**, **QuickSort_v1**, **MergeSort_v1**.

Алгоритм **QuickSort** я ограничил, при N больше 100000 он пропускает сортировку, т.к. начинает требовать много времени для не рандомизированных массивов.

После запуска, программа выводит сводную таблицу с результатами:

```bash
dmitry@lachugin:~/otus/lachugin_algorithms_hw/hw08$ ./hw08_checker 
Подготовлено 8 тестов из каталога "/home/dmitry/otus/lachugin_algorithms_hw/hw08/sorting-tests/0.random"
Подготовлено 8 тестов из каталога "/home/dmitry/otus/lachugin_algorithms_hw/hw08/sorting-tests/1.digits"
Подготовлено 8 тестов из каталога "/home/dmitry/otus/lachugin_algorithms_hw/hw08/sorting-tests/2.sorted"
Подготовлено 8 тестов из каталога "/home/dmitry/otus/lachugin_algorithms_hw/hw08/sorting-tests/3.revers"
Запускаем тест группы: "0.random"
   исходный массив из 1 элементов: 
   0
  OK  --> BubbleSort_v1 (size=1): comps=0  swaps=0  duration=491
  OK  --> BubbleSort_v2 (size=1): comps=0  swaps=0  duration=93
  OK  --> InsertionSort_v1 (size=1): comps=0  swaps=0  duration=188
  OK  --> InsertionSort_v2 (size=1): comps=0  swaps=0  duration=53
  OK  --> InsertionSort_v3 (size=1): comps=0  swaps=0  duration=54
  OK  --> ShellSort_v1 (size=1): comps=0  swaps=0  duration=166
  OK  --> ShellSort_v2 (size=1): comps=0  swaps=0  duration=52
  OK  --> ShellSort_v3 (size=1): comps=0  swaps=0  duration=72
  OK  --> SelectionSort_v1 (size=1): comps=0  swaps=0  duration=372
  OK  --> HeapSort_v1 (size=1): comps=0  swaps=0  duration=58
  OK  --> QuickSort_v1 (size=1): comps=0  swaps=0  duration=60
  OK  --> MergeSort_v1 (size=1): comps=0  swaps=0  duration=162
   первые и последние 5 элементов массива из 10 элементов: 
   7 0 6 1 3 ... 7 0 6 1 3
  OK  --> BubbleSort_v1 (size=10): comps=45  swaps=16  duration=293
  OK  --> BubbleSort_v2 (size=10): comps=52  swaps=16  duration=304
  OK  --> InsertionSort_v1 (size=10): comps=24  swaps=16  duration=187
  OK  --> InsertionSort_v2 (size=10): comps=24  swaps=16  duration=186
  OK  --> InsertionSort_v3 (size=10): comps=55  swaps=16  duration=331
  OK  --> ShellSort_v1 (size=10): comps=29  swaps=10  duration=213
  OK  --> ShellSort_v2 (size=10): comps=25  swaps=10  duration=219
  OK  --> ShellSort_v3 (size=10): comps=23  swaps=12  duration=499
  OK  --> SelectionSort_v1 (size=10): comps=45  swaps=8  duration=204
  OK  --> HeapSort_v1 (size=10): comps=39  swaps=28  duration=363
  OK  --> QuickSort_v1 (size=10): comps=31  swaps=8  duration=339
  OK  --> MergeSort_v1 (size=10): comps=23  swaps=34  duration=668
   первые и последние 10 элементов массива из 100 элементов: 
   74 11 72 33 40 28 23 17 66 61 ... 74 11 72 33 40 28 23 17 66 61
  OK  --> BubbleSort_v1 (size=100): comps=4950  swaps=2157  duration=9849
  OK  --> BubbleSort_v2 (size=100): comps=4819  swaps=2157  duration=11431
  OK  --> InsertionSort_v1 (size=100): comps=2253  swaps=2157  duration=3655
  OK  --> InsertionSort_v2 (size=100): comps=2253  swaps=2157  duration=3127
  OK  --> InsertionSort_v3 (size=100): comps=1153  swaps=2157  duration=4304
  OK  --> ShellSort_v1 (size=100): comps=877  swaps=413  duration=3146
  OK  --> ShellSort_v2 (size=100): comps=834  swaps=403  duration=3430
  OK  --> ShellSort_v3 (size=100): comps=734  swaps=331  duration=3748
  OK  --> SelectionSort_v1 (size=100): comps=4950  swaps=95  duration=10055
  OK  --> HeapSort_v1 (size=100): comps=1034  swaps=591  duration=3312
  OK  --> QuickSort_v1 (size=100): comps=690  swaps=287  duration=3720
  OK  --> MergeSort_v1 (size=100): comps=543  swaps=672  duration=4923
   первые и последние 10 элементов массива из 1000 элементов: 
   561 233 825 817 336 551 811 627 485 812 ... 561 233 825 817 336 551 811 627 485 812
  OK  --> BubbleSort_v1 (size=1000): comps=499500  swaps=247489  duration=694666
  OK  --> BubbleSort_v2 (size=1000): comps=499372  swaps=247489  duration=1198185
  OK  --> InsertionSort_v1 (size=1000): comps=248479  swaps=247489  duration=334231
  OK  --> InsertionSort_v2 (size=1000): comps=248479  swaps=247489  duration=341051
  OK  --> InsertionSort_v3 (size=1000): comps=18169  swaps=247489  duration=185139
  OK  --> ShellSort_v1 (size=1000): comps=15296  swaps=7815  duration=52080
  OK  --> ShellSort_v2 (size=1000): comps=14379  swaps=6975  duration=56449
  OK  --> ShellSort_v3 (size=1000): comps=14113  swaps=6741  duration=71027
  OK  --> SelectionSort_v1 (size=1000): comps=499500  swaps=993  duration=1026751
  OK  --> HeapSort_v1 (size=1000): comps=16850  swaps=9065  duration=40191
  OK  --> QuickSort_v1 (size=1000): comps=11404  swaps=4343  duration=47010
  OK  --> MergeSort_v1 (size=1000): comps=8721  swaps=9976  duration=60862
   первые и последние 10 элементов массива из 10000 элементов: 
   3839 1880 4323 1741 3390 3908 186 4357 9885 3111 ... 3839 1880 4323 1741 3390 3908 186 4357 9885 3111
  OK  --> BubbleSort_v1 (size=10000): comps=49995000  swaps=25288624  duration=116077892
  OK  --> BubbleSort_v2 (size=10000): comps=49997497  swaps=25288624  duration=142014003
  OK  --> InsertionSort_v1 (size=10000): comps=25298615  swaps=25288624  duration=38869048
  OK  --> InsertionSort_v2 (size=10000): comps=25298615  swaps=25288624  duration=38914791
  OK  --> InsertionSort_v3 (size=10000): comps=247903  swaps=25288624  duration=16882915
  OK  --> ShellSort_v1 (size=10000): comps=267306  swaps=152486  duration=861080
  OK  --> ShellSort_v2 (size=10000): comps=255750  swaps=147412  duration=961239
  OK  --> ShellSort_v3 (size=10000): comps=203477  swaps=96320  duration=883085
  OK  --> SelectionSort_v1 (size=10000): comps=49995000  swaps=9992  duration=111909479
  OK  --> HeapSort_v1 (size=10000): comps=235395  swaps=124222  duration=516641
  OK  --> QuickSort_v1 (size=10000): comps=155089  swaps=72224  duration=625077
  OK  --> MergeSort_v1 (size=10000): comps=120481  swaps=133616  duration=683367
   первые и последние 10 элементов массива из 100000 элементов: 
   8167 4911 1309 46835 39659 7048 82594 57334 60184 49556 ... 8167 4911 1309 46835 39659 7048 82594 57334 60184 49556
  OK  --> BubbleSort_v1 (size=100000): comps=4999950000  swaps=2501737763  duration=13190496838
  OK  --> BubbleSort_v2 (size=100000): comps=5000040547  swaps=2501737763  duration=13583163880
  OK  --> InsertionSort_v1 (size=100000): comps=2501837753  swaps=2501737763  duration=3417213074
  OK  --> InsertionSort_v2 (size=100000): comps=2501837753  swaps=2501737763  duration=3704149504
  OK  --> InsertionSort_v3 (size=100000): comps=3145309  swaps=2501737763  duration=1560830355
  OK  --> ShellSort_v1 (size=100000): comps=4562610  swaps=3113279  duration=11525372
  OK  --> ShellSort_v2 (size=100000): comps=3920114  swaps=2503799  duration=13119732
  OK  --> ShellSort_v3 (size=100000): comps=2677097  swaps=1266093  duration=12027367
  OK  --> SelectionSort_v1 (size=100000): comps=4999950000  swaps=99987  duration=10771855647
  OK  --> HeapSort_v1 (size=100000): comps=3020053  swaps=1575003  duration=7936077
  OK  --> QuickSort_v1 (size=100000): comps=2065627  swaps=938783  duration=7819632
  OK  --> MergeSort_v1 (size=100000): comps=1536652  swaps=1668928  duration=8648046
   первые и последние 10 элементов массива из 1000000 элементов: 
   106757 84762 359524 454600 539912 260065 748214 424144 828940 441559 ... 106757 84762 359524 454600 539912 260065 748214 424144 828940 441559
  FAIL  --> BubbleSort_v1 (size=1000000): comps=0  swaps=0  duration=0
  FAIL  --> BubbleSort_v2 (size=1000000): comps=0  swaps=0  duration=0
  FAIL  --> InsertionSort_v1 (size=1000000): comps=0  swaps=0  duration=0
  FAIL  --> InsertionSort_v2 (size=1000000): comps=0  swaps=0  duration=0
  FAIL  --> InsertionSort_v3 (size=1000000): comps=0  swaps=0  duration=0
  OK  --> ShellSort_v1 (size=1000000): comps=68027835  swaps=50530714  duration=168073437
  OK  --> ShellSort_v2 (size=1000000): comps=62283286  swaps=44889440  duration=185020322
  OK  --> ShellSort_v3 (size=1000000): comps=33226631  swaps=15880632  duration=156281015
  FAIL  --> SelectionSort_v1 (size=1000000): comps=0  swaps=0  duration=0
  OK  --> HeapSort_v1 (size=1000000): comps=36792142  swaps=19046520  duration=310188734
  FAIL  --> QuickSort_v1 (size=1000000): comps=0  swaps=0  duration=0
  OK  --> MergeSort_v1 (size=1000000): comps=18674508  swaps=19951424  duration=101071034
   первые и последние 10 элементов массива из 10000000 элементов: 
   3014676 6210057 6922993 3173860 8803028 1921990 3755739 4739250 7609328 4600455 ... 3014676 6210057 6922993 3173860 8803028 1921990 3755739 4739250 7609328 4600455
  FAIL  --> BubbleSort_v1 (size=10000000): comps=0  swaps=0  duration=0
  FAIL  --> BubbleSort_v2 (size=10000000): comps=0  swaps=0  duration=0
  FAIL  --> InsertionSort_v1 (size=10000000): comps=0  swaps=0  duration=0
  FAIL  --> InsertionSort_v2 (size=10000000): comps=0  swaps=0  duration=0
  FAIL  --> InsertionSort_v3 (size=10000000): comps=0  swaps=0  duration=0
  OK  --> ShellSort_v1 (size=10000000): comps=1041605900  swaps=826620860  duration=2271406653
  OK  --> ShellSort_v2 (size=10000000): comps=1040073945  swaps=832098474  duration=2778838852
  OK  --> ShellSort_v3 (size=10000000): comps=398594255  swaps=191906810  duration=1818186986
  FAIL  --> SelectionSort_v1 (size=10000000): comps=0  swaps=0  duration=0
  OK  --> HeapSort_v1 (size=10000000): comps=434640382  swaps=223831712  duration=6936685825
  FAIL  --> QuickSort_v1 (size=10000000): comps=0  swaps=0  duration=0
  OK  --> MergeSort_v1 (size=10000000): comps=220101952  swaps=233222784  duration=1229245380

Запускаем тест группы: "1.digits"
   исходный массив из 1 элементов: 
   0
  OK  --> BubbleSort_v1 (size=1): comps=0  swaps=0  duration=775
  OK  --> BubbleSort_v2 (size=1): comps=0  swaps=0  duration=52
  OK  --> InsertionSort_v1 (size=1): comps=0  swaps=0  duration=221
  OK  --> InsertionSort_v2 (size=1): comps=0  swaps=0  duration=53
  OK  --> InsertionSort_v3 (size=1): comps=0  swaps=0  duration=52
  OK  --> ShellSort_v1 (size=1): comps=0  swaps=0  duration=289
  OK  --> ShellSort_v2 (size=1): comps=0  swaps=0  duration=49
  OK  --> ShellSort_v3 (size=1): comps=0  swaps=0  duration=78
  OK  --> SelectionSort_v1 (size=1): comps=0  swaps=0  duration=493
  OK  --> HeapSort_v1 (size=1): comps=0  swaps=0  duration=59
  OK  --> QuickSort_v1 (size=1): comps=0  swaps=0  duration=61
  OK  --> MergeSort_v1 (size=1): comps=0  swaps=0  duration=805
   первые и последние 5 элементов массива из 10 элементов: 
   6 1 7 3 2 ... 6 1 7 3 2
  OK  --> BubbleSort_v1 (size=10): comps=45  swaps=21  duration=461
  OK  --> BubbleSort_v2 (size=10): comps=49  swaps=21  duration=236
  OK  --> InsertionSort_v1 (size=10): comps=28  swaps=21  duration=205
  OK  --> InsertionSort_v2 (size=10): comps=28  swaps=21  duration=200
  OK  --> InsertionSort_v3 (size=10): comps=51  swaps=21  duration=281
  OK  --> ShellSort_v1 (size=10): comps=32  swaps=15  duration=261
  OK  --> ShellSort_v2 (size=10): comps=28  swaps=15  duration=297
  OK  --> ShellSort_v3 (size=10): comps=29  swaps=19  duration=524
  OK  --> SelectionSort_v1 (size=10): comps=45  swaps=7  duration=174
  OK  --> HeapSort_v1 (size=10): comps=37  swaps=27  duration=290
  OK  --> QuickSort_v1 (size=10): comps=31  swaps=11  duration=420
  OK  --> MergeSort_v1 (size=10): comps=22  swaps=34  duration=1165
   первые и последние 10 элементов массива из 100 элементов: 
   5 2 0 7 9 7 8 7 9 5 ... 5 2 0 7 9 7 8 7 9 5
  OK  --> BubbleSort_v1 (size=100): comps=4950  swaps=2401  duration=10132
  OK  --> BubbleSort_v2 (size=100): comps=4959  swaps=2401  duration=11558
  OK  --> InsertionSort_v1 (size=100): comps=2498  swaps=2401  duration=3942
  OK  --> InsertionSort_v2 (size=100): comps=2498  swaps=2401  duration=3504
  OK  --> InsertionSort_v3 (size=100): comps=1161  swaps=2401  duration=4738
  OK  --> ShellSort_v1 (size=100): comps=691  swaps=228  duration=2372
  OK  --> ShellSort_v2 (size=100): comps=644  swaps=208  duration=2325
  OK  --> ShellSort_v3 (size=100): comps=623  swaps=219  duration=3252
  OK  --> SelectionSort_v1 (size=100): comps=4950  swaps=88  duration=10030
  OK  --> HeapSort_v1 (size=100): comps=980  swaps=524  duration=3449
  OK  --> QuickSort_v1 (size=100): comps=880  swaps=221  duration=2728
  OK  --> MergeSort_v1 (size=100): comps=547  swaps=672  duration=4570
   первые и последние 10 элементов массива из 1000 элементов: 
   2 9 8 9 4 8 6 8 5 5 ... 2 9 8 9 4 8 6 8 5 5
  OK  --> BubbleSort_v1 (size=1000): comps=499500  swaps=228217  duration=573486
  OK  --> BubbleSort_v2 (size=1000): comps=495450  swaps=228217  duration=1162970
  OK  --> InsertionSort_v1 (size=1000): comps=229214  swaps=228217  duration=310787
  OK  --> InsertionSort_v2 (size=1000): comps=229214  swaps=228217  duration=313919
  OK  --> InsertionSort_v3 (size=1000): comps=18261  swaps=228217  duration=164143
  OK  --> ShellSort_v1 (size=1000): comps=10449  swaps=2863  duration=29105
  OK  --> ShellSort_v2 (size=1000): comps=9991  swaps=2458  duration=29614
  OK  --> ShellSort_v3 (size=1000): comps=9726  swaps=2226  duration=36071
  OK  --> SelectionSort_v1 (size=1000): comps=499500  swaps=894  duration=1021820
  OK  --> HeapSort_v1 (size=1000): comps=15801  swaps=8271  duration=41127
  OK  --> QuickSort_v1 (size=1000): comps=54477  swaps=1292  duration=83624
  OK  --> MergeSort_v1 (size=1000): comps=8488  swaps=9976  duration=44804
   первые и последние 10 элементов массива из 10000 элементов: 
   1 0 8 4 8 9 9 4 4 7 ... 1 0 8 4 8 9 9 4 4 7
  OK  --> BubbleSort_v1 (size=10000): comps=49995000  swaps=22471502  duration=115245250
  OK  --> BubbleSort_v2 (size=10000): comps=49500490  swaps=22471502  duration=138936119
  OK  --> InsertionSort_v1 (size=10000): comps=22481500  swaps=22471502  duration=31264898
  OK  --> InsertionSort_v2 (size=10000): comps=22481500  swaps=22471502  duration=35152044
  OK  --> InsertionSort_v3 (size=10000): comps=248871  swaps=22471502  duration=13517163
  OK  --> ShellSort_v1 (size=10000): comps=153568  swaps=37573  duration=356829
  OK  --> ShellSort_v2 (size=10000): comps=135590  swaps=25870  duration=343193
  OK  --> ShellSort_v3 (size=10000): comps=129952  swaps=21427  duration=394834
  OK  --> SelectionSort_v1 (size=10000): comps=49995000  swaps=8999  duration=114839623
  OK  --> HeapSort_v1 (size=10000): comps=217737  swaps=112694  duration=523963
  OK  --> QuickSort_v1 (size=10000): comps=5038978  swaps=22987  duration=5550888
  OK  --> MergeSort_v1 (size=10000): comps=116872  swaps=133616  duration=441055
   первые и последние 10 элементов массива из 100000 элементов: 
   2 7 8 2 4 7 2 0 3 4 ... 2 7 8 2 4 7 2 0 3 4
  OK  --> BubbleSort_v1 (size=100000): comps=4999950000  swaps=2246197432  duration=13026374145
  OK  --> BubbleSort_v2 (size=100000): comps=4949954955  swaps=2246197432  duration=14138960877
  OK  --> InsertionSort_v1 (size=100000): comps=2246297430  swaps=2246197432  duration=3454265443
  OK  --> InsertionSort_v2 (size=100000): comps=2246297430  swaps=2246197432  duration=3469311510
  OK  --> InsertionSort_v3 (size=100000): comps=3154353  swaps=2246197432  duration=1384727389
  OK  --> ShellSort_v1 (size=100000): comps=1860949  swaps=401432  duration=4257007
  OK  --> ShellSort_v2 (size=100000): comps=1697481  swaps=267916  duration=4110081
  OK  --> ShellSort_v3 (size=100000): comps=1635712  swaps=211740  duration=4925729
  OK  --> SelectionSort_v1 (size=100000): comps=4999950000  swaps=90000  duration=10632456683
  OK  --> HeapSort_v1 (size=100000): comps=2766452  swaps=1420513  duration=6262320
  OK  --> QuickSort_v1 (size=100000): comps=500359982  swaps=239983  duration=569527932
  OK  --> MergeSort_v1 (size=100000): comps=1483484  swaps=1668928  duration=4792023
   первые и последние 10 элементов массива из 1000000 элементов: 
   5 5 4 8 9 1 7 4 8 2 ... 5 5 4 8 9 1 7 4 8 2
  FAIL  --> BubbleSort_v1 (size=1000000): comps=0  swaps=0  duration=0
  FAIL  --> BubbleSort_v2 (size=1000000): comps=0  swaps=0  duration=0
  FAIL  --> InsertionSort_v1 (size=1000000): comps=0  swaps=0  duration=0
  FAIL  --> InsertionSort_v2 (size=1000000): comps=0  swaps=0  duration=0
  FAIL  --> InsertionSort_v3 (size=1000000): comps=0  swaps=0  duration=0
  OK  --> ShellSort_v1 (size=1000000): comps=22383596  swaps=4789821  duration=72119500
  OK  --> ShellSort_v2 (size=1000000): comps=20307786  swaps=2781582  duration=75751754
  OK  --> ShellSort_v3 (size=1000000): comps=19600952  swaps=2127650  duration=77592792
  FAIL  --> SelectionSort_v1 (size=1000000): comps=0  swaps=0  duration=0
  OK  --> HeapSort_v1 (size=1000000): comps=33483726  swaps=17110562  duration=81399383
  FAIL  --> QuickSort_v1 (size=1000000): comps=0  swaps=0  duration=0
  OK  --> MergeSort_v1 (size=1000000): comps=17970393  swaps=19951424  duration=60010693
   первые и последние 10 элементов массива из 10000000 элементов: 
   3 4 5 5 4 7 4 9 5 3 ... 3 4 5 5 4 7 4 9 5 3
  FAIL  --> BubbleSort_v1 (size=10000000): comps=0  swaps=0  duration=0
  FAIL  --> BubbleSort_v2 (size=10000000): comps=0  swaps=0  duration=0
  FAIL  --> InsertionSort_v1 (size=10000000): comps=0  swaps=0  duration=0
  FAIL  --> InsertionSort_v2 (size=10000000): comps=0  swaps=0  duration=0
  FAIL  --> InsertionSort_v3 (size=10000000): comps=0  swaps=0  duration=0
  OK  --> ShellSort_v1 (size=10000000): comps=270196822  swaps=54253917  duration=740078577
  OK  --> ShellSort_v2 (size=10000000): comps=237536810  swaps=28239358  duration=633334710
  OK  --> ShellSort_v3 (size=10000000): comps=229224325  swaps=21272122  duration=713581935
  FAIL  --> SelectionSort_v1 (size=10000000): comps=0  swaps=0  duration=0
  OK  --> HeapSort_v1 (size=10000000): comps=394975504  swaps=201210998  duration=845606254
  FAIL  --> QuickSort_v1 (size=10000000): comps=0  swaps=0  duration=0
  OK  --> MergeSort_v1 (size=10000000): comps=211522718  swaps=233222784  duration=658882045

Запускаем тест группы: "2.sorted"
   исходный массив из 1 элементов: 
   0
  OK  --> BubbleSort_v1 (size=1): comps=0  swaps=0  duration=292
  OK  --> BubbleSort_v2 (size=1): comps=0  swaps=0  duration=59
  OK  --> InsertionSort_v1 (size=1): comps=0  swaps=0  duration=133
  OK  --> InsertionSort_v2 (size=1): comps=0  swaps=0  duration=52
  OK  --> InsertionSort_v3 (size=1): comps=0  swaps=0  duration=54
  OK  --> ShellSort_v1 (size=1): comps=0  swaps=0  duration=148
  OK  --> ShellSort_v2 (size=1): comps=0  swaps=0  duration=48
  OK  --> ShellSort_v3 (size=1): comps=0  swaps=0  duration=73
  OK  --> SelectionSort_v1 (size=1): comps=0  swaps=0  duration=280
  OK  --> HeapSort_v1 (size=1): comps=0  swaps=0  duration=53
  OK  --> QuickSort_v1 (size=1): comps=0  swaps=0  duration=65
  OK  --> MergeSort_v1 (size=1): comps=0  swaps=0  duration=206
   первые и последние 5 элементов массива из 10 элементов: 
   0 6 2 3 4 ... 0 6 2 3 4
  OK  --> BubbleSort_v1 (size=10): comps=45  swaps=9  duration=255
  OK  --> BubbleSort_v2 (size=10): comps=45  swaps=9  duration=193
  OK  --> InsertionSort_v1 (size=10): comps=18  swaps=9  duration=131
  OK  --> InsertionSort_v2 (size=10): comps=18  swaps=9  duration=161
  OK  --> InsertionSort_v3 (size=10): comps=59  swaps=9  duration=237
  OK  --> ShellSort_v1 (size=10): comps=22  swaps=1  duration=122
  OK  --> ShellSort_v2 (size=10): comps=27  swaps=9  duration=193
  OK  --> ShellSort_v3 (size=10): comps=14  swaps=1  duration=385
  OK  --> SelectionSort_v1 (size=10): comps=45  swaps=1  duration=164
  OK  --> HeapSort_v1 (size=10): comps=41  swaps=29  duration=334
  OK  --> QuickSort_v1 (size=10): comps=48  swaps=1  duration=298
  OK  --> MergeSort_v1 (size=10): comps=22  swaps=34  duration=865
   первые и последние 10 элементов массива из 100 элементов: 
   0 1 2 3 4 5 6 7 8 9 ... 0 1 2 3 4 5 6 7 8 9
  OK  --> BubbleSort_v1 (size=100): comps=4950  swaps=87  duration=4047
  OK  --> BubbleSort_v2 (size=100): comps=3510  swaps=87  duration=2624
  OK  --> InsertionSort_v1 (size=100): comps=186  swaps=87  duration=290
  OK  --> InsertionSort_v2 (size=100): comps=186  swaps=87  duration=346
  OK  --> InsertionSort_v3 (size=100): comps=1245  swaps=87  duration=972
  OK  --> ShellSort_v1 (size=100): comps=590  swaps=87  duration=984
  OK  --> ShellSort_v2 (size=100): comps=511  swaps=31  duration=959
  OK  --> ShellSort_v3 (size=100): comps=517  swaps=71  duration=1887
  OK  --> SelectionSort_v1 (size=100): comps=4950  swaps=1  duration=7797
  OK  --> HeapSort_v1 (size=100): comps=1081  swaps=645  duration=2813
  OK  --> QuickSort_v1 (size=100): comps=2760  swaps=1  duration=3795
  OK  --> MergeSort_v1 (size=100): comps=397  swaps=672  duration=2338
   первые и последние 10 элементов массива из 1000 элементов: 
   0 1 2 3 4 5 6 7 8 9 ... 0 1 2 3 4 5 6 7 8 9
  OK  --> BubbleSort_v1 (size=1000): comps=499500  swaps=6604  duration=291169
  OK  --> BubbleSort_v2 (size=1000): comps=496930  swaps=6604  duration=294769
  OK  --> InsertionSort_v1 (size=1000): comps=7603  swaps=6604  duration=9581
  OK  --> InsertionSort_v2 (size=1000): comps=7603  swaps=6604  duration=9946
  OK  --> InsertionSort_v3 (size=1000): comps=18375  swaps=6604  duration=11661
  OK  --> ShellSort_v1 (size=1000): comps=10710  swaps=2722  duration=29915
  OK  --> ShellSort_v2 (size=1000): comps=10413  swaps=2446  duration=27763
  OK  --> ShellSort_v3 (size=1000): comps=10357  swaps=2450  duration=32232
  OK  --> SelectionSort_v1 (size=1000): comps=499500  swaps=10  duration=1009840
  OK  --> HeapSort_v1 (size=1000): comps=17560  swaps=9680  duration=34554
  OK  --> QuickSort_v1 (size=1000): comps=175339  swaps=752  duration=183035
  OK  --> MergeSort_v1 (size=1000): comps=6645  swaps=9976  duration=21514
   первые и последние 10 элементов массива из 10000 элементов: 
   0 1 2 3 4 5 6 7 8 9 ... 0 1 2 3 4 5 6 7 8 9
  OK  --> BubbleSort_v1 (size=10000): comps=49995000  swaps=666392  duration=32258501
  OK  --> BubbleSort_v2 (size=10000): comps=49737454  swaps=666392  duration=33472528
  OK  --> InsertionSort_v1 (size=10000): comps=676391  swaps=666392  duration=968906
  OK  --> InsertionSort_v2 (size=10000): comps=676391  swaps=666392  duration=968869
  OK  --> InsertionSort_v3 (size=10000): comps=248665  swaps=666392  duration=458941
  OK  --> ShellSort_v1 (size=10000): comps=176309  swaps=56474  duration=425115
  OK  --> ShellSort_v2 (size=10000): comps=171387  swaps=57928  duration=525504
  OK  --> ShellSort_v3 (size=10000): comps=163917  swaps=51640  duration=581476
  OK  --> SelectionSort_v1 (size=10000): comps=49995000  swaps=100  duration=111430313
  OK  --> HeapSort_v1 (size=10000): comps=244307  swaps=131480  duration=452759
  OK  --> QuickSort_v1 (size=10000): comps=802285  swaps=26100  duration=630504
  OK  --> MergeSort_v1 (size=10000): comps=101839  swaps=133616  duration=251156
   первые и последние 10 элементов массива из 100000 элементов: 
   0 1 2 3 4 5 6 7 8 9 ... 0 1 2 3 4 5 6 7 8 9
  OK  --> BubbleSort_v1 (size=100000): comps=4999950000  swaps=65248998  duration=3135119886
  OK  --> BubbleSort_v2 (size=100000): comps=4983441034  swaps=65248998  duration=3125558863
  OK  --> InsertionSort_v1 (size=100000): comps=65348997  swaps=65248998  duration=96763909
  OK  --> InsertionSort_v2 (size=100000): comps=65348997  swaps=65248998  duration=99249896
  OK  --> InsertionSort_v3 (size=100000): comps=3146427  swaps=65248998  duration=49706349
  OK  --> ShellSort_v1 (size=100000): comps=2833579  swaps=1334970  duration=7362997
  OK  --> ShellSort_v2 (size=100000): comps=2509526  swaps=1042150  duration=8307672
  OK  --> ShellSort_v3 (size=100000): comps=2258426  swaps=796742  duration=8679817
  OK  --> SelectionSort_v1 (size=100000): comps=4999950000  swaps=1000  duration=11047477492
  OK  --> HeapSort_v1 (size=100000): comps=3110584  swaps=1648620  duration=5948476
  OK  --> QuickSort_v1 (size=100000): comps=17718857  swaps=469866  duration=17100606
  OK  --> MergeSort_v1 (size=100000): comps=1341875  swaps=1668928  duration=2761101
   первые и последние 10 элементов массива из 1000000 элементов: 
   0 1 2 3 4 5 6 7 8 9 ... 0 1 2 3 4 5 6 7 8 9
  FAIL  --> BubbleSort_v1 (size=1000000): comps=0  swaps=0  duration=0
  FAIL  --> BubbleSort_v2 (size=1000000): comps=0  swaps=0  duration=0
  FAIL  --> InsertionSort_v1 (size=1000000): comps=0  swaps=0  duration=0
  FAIL  --> InsertionSort_v2 (size=1000000): comps=0  swaps=0  duration=0
  FAIL  --> InsertionSort_v3 (size=1000000): comps=0  swaps=0  duration=0
  OK  --> ShellSort_v1 (size=1000000): comps=38939516  swaps=20954426  duration=104497351
  OK  --> ShellSort_v2 (size=1000000): comps=37313903  swaps=19378884  duration=126355985
  OK  --> ShellSort_v3 (size=1000000): comps=28889906  swaps=11038830  duration=117260146
  FAIL  --> SelectionSort_v1 (size=1000000): comps=0  swaps=0  duration=0
  OK  --> HeapSort_v1 (size=1000000): comps=37692154  swaps=19765196  duration=87386964
  FAIL  --> QuickSort_v1 (size=1000000): comps=0  swaps=0  duration=0
  OK  --> MergeSort_v1 (size=1000000): comps=16606871  swaps=19951424  duration=41766691
   первые и последние 10 элементов массива из 10000000 элементов: 
   0 1 2 3 4 5 6 7 8 9 ... 0 1 2 3 4 5 6 7 8 9
  FAIL  --> BubbleSort_v1 (size=10000000): comps=0  swaps=0  duration=0
  FAIL  --> BubbleSort_v2 (size=10000000): comps=0  swaps=0  duration=0
  FAIL  --> InsertionSort_v1 (size=10000000): comps=0  swaps=0  duration=0
  FAIL  --> InsertionSort_v2 (size=10000000): comps=0  swaps=0  duration=0
  FAIL  --> InsertionSort_v3 (size=10000000): comps=0  swaps=0  duration=0
  OK  --> ShellSort_v1 (size=10000000): comps=624139826  swaps=404287028  duration=1487268534
  OK  --> ShellSort_v2 (size=10000000): comps=580178096  swaps=367110546  duration=1718728229
  OK  --> ShellSort_v3 (size=10000000): comps=353352946  swaps=141665472  duration=1460486118
  FAIL  --> SelectionSort_v1 (size=10000000): comps=0  swaps=0  duration=0
  OK  --> HeapSort_v1 (size=10000000): comps=444849813  swaps=231605924  duration=1047595094
  FAIL  --> QuickSort_v1 (size=10000000): comps=0  swaps=0  duration=0
  OK  --> MergeSort_v1 (size=10000000): comps=200958059  swaps=233222784  duration=425703984

Запускаем тест группы: "3.revers"
   исходный массив из 1 элементов: 
   0
  OK  --> BubbleSort_v1 (size=1): comps=0  swaps=0  duration=231
  OK  --> BubbleSort_v2 (size=1): comps=0  swaps=0  duration=44
  OK  --> InsertionSort_v1 (size=1): comps=0  swaps=0  duration=114
  OK  --> InsertionSort_v2 (size=1): comps=0  swaps=0  duration=52
  OK  --> InsertionSort_v3 (size=1): comps=0  swaps=0  duration=54
  OK  --> ShellSort_v1 (size=1): comps=0  swaps=0  duration=153
  OK  --> ShellSort_v2 (size=1): comps=0  swaps=0  duration=49
  OK  --> ShellSort_v3 (size=1): comps=0  swaps=0  duration=75
  OK  --> SelectionSort_v1 (size=1): comps=0  swaps=0  duration=241
  OK  --> HeapSort_v1 (size=1): comps=0  swaps=0  duration=56
  OK  --> QuickSort_v1 (size=1): comps=0  swaps=0  duration=65
  OK  --> MergeSort_v1 (size=1): comps=0  swaps=0  duration=118
   первые и последние 5 элементов массива из 10 элементов: 
   9 8 7 6 5 ... 9 8 7 6 5
  OK  --> BubbleSort_v1 (size=10): comps=45  swaps=45  duration=181
  OK  --> BubbleSort_v2 (size=10): comps=54  swaps=45  duration=186
  OK  --> InsertionSort_v1 (size=10): comps=45  swaps=45  duration=206
  OK  --> InsertionSort_v2 (size=10): comps=45  swaps=45  duration=197
  OK  --> InsertionSort_v3 (size=10): comps=47  swaps=45  duration=250
  OK  --> ShellSort_v1 (size=10): comps=27  swaps=13  duration=183
  OK  --> ShellSort_v2 (size=10): comps=25  swaps=11  duration=271
  OK  --> ShellSort_v3 (size=10): comps=30  swaps=25  duration=428
  OK  --> SelectionSort_v1 (size=10): comps=45  swaps=5  duration=196
  OK  --> HeapSort_v1 (size=10): comps=35  swaps=21  duration=275
  OK  --> QuickSort_v1 (size=10): comps=54  swaps=5  duration=378
  OK  --> MergeSort_v1 (size=10): comps=15  swaps=34  duration=411
   первые и последние 10 элементов массива из 100 элементов: 
   99 98 97 96 95 94 93 92 91 90 ... 99 98 97 96 95 94 93 92 91 90
  OK  --> BubbleSort_v1 (size=100): comps=4950  swaps=4950  duration=6778
  OK  --> BubbleSort_v2 (size=100): comps=5049  swaps=4950  duration=6679
  OK  --> InsertionSort_v1 (size=100): comps=4950  swaps=4950  duration=6991
  OK  --> InsertionSort_v2 (size=100): comps=4950  swaps=4950  duration=6616
  OK  --> InsertionSort_v3 (size=100): comps=1059  swaps=4950  duration=3709
  OK  --> ShellSort_v1 (size=100): comps=668  swaps=260  duration=1291
  OK  --> ShellSort_v2 (size=100): comps=614  swaps=192  duration=1604
  OK  --> ShellSort_v3 (size=100): comps=626  swaps=234  duration=2394
  OK  --> SelectionSort_v1 (size=100): comps=4950  swaps=50  duration=8206
  OK  --> HeapSort_v1 (size=100): comps=944  swaps=516  duration=2907
  OK  --> QuickSort_v1 (size=100): comps=5049  swaps=50  duration=5164
  OK  --> MergeSort_v1 (size=100): comps=316  swaps=672  duration=2414
   первые и последние 10 элементов массива из 1000 элементов: 
   999 998 997 996 995 994 993 992 991 990 ... 999 998 997 996 995 994 993 992 991 990
  OK  --> BubbleSort_v1 (size=1000): comps=499500  swaps=499500  duration=698914
  OK  --> BubbleSort_v2 (size=1000): comps=500499  swaps=499500  duration=708978
  OK  --> InsertionSort_v1 (size=1000): comps=499500  swaps=499500  duration=659870
  OK  --> InsertionSort_v2 (size=1000): comps=499500  swaps=499500  duration=698478
  OK  --> InsertionSort_v3 (size=1000): comps=16973  swaps=499500  duration=280535
  OK  --> ShellSort_v1 (size=1000): comps=11716  swaps=4700  duration=14158
  OK  --> ShellSort_v2 (size=1000): comps=10511  swaps=3424  duration=18324
  OK  --> ShellSort_v3 (size=1000): comps=10788  swaps=3690  duration=23264
  OK  --> SelectionSort_v1 (size=1000): comps=499500  swaps=500  duration=1017458
  OK  --> HeapSort_v1 (size=1000): comps=15965  swaps=8316  duration=33516
  OK  --> QuickSort_v1 (size=1000): comps=500499  swaps=500  duration=511949
  OK  --> MergeSort_v1 (size=1000): comps=4932  swaps=9976  duration=20811
   первые и последние 10 элементов массива из 10000 элементов: 
   9999 9998 9997 9996 9995 9994 9993 9992 9991 9990 ... 9999 9998 9997 9996 9995 9994 9993 9992 9991 9990
  OK  --> BubbleSort_v1 (size=10000): comps=49995000  swaps=49995000  duration=76989191
  OK  --> BubbleSort_v2 (size=10000): comps=50004999  swaps=49995000  duration=74434999
  OK  --> InsertionSort_v1 (size=10000): comps=49995000  swaps=49995000  duration=70517252
  OK  --> InsertionSort_v2 (size=10000): comps=49995000  swaps=49995000  duration=74304599
  OK  --> InsertionSort_v3 (size=10000): comps=237261  swaps=49995000  duration=30344961
  OK  --> ShellSort_v1 (size=10000): comps=172578  swaps=62560  duration=196953
  OK  --> ShellSort_v2 (size=10000): comps=144824  swaps=36778  duration=234213
  OK  --> ShellSort_v3 (size=10000): comps=148103  swaps=41884  duration=306808
  OK  --> SelectionSort_v1 (size=10000): comps=49995000  swaps=5000  duration=110143385
  OK  --> HeapSort_v1 (size=10000): comps=226682  swaps=116696  duration=478250
  OK  --> QuickSort_v1 (size=10000): comps=50004999  swaps=5000  duration=42490482
  OK  --> MergeSort_v1 (size=10000): comps=64608  swaps=133616  duration=203274
   первые и последние 10 элементов массива из 100000 элементов: 
   99999 99998 99997 99996 99995 99994 99993 99992 99991 99990 ... 99999 99998 99997 99996 99995 99994 99993 99992 99991 99990
  OK  --> BubbleSort_v1 (size=100000): comps=4999950000  swaps=4999950000  duration=7200110603
  OK  --> BubbleSort_v2 (size=100000): comps=5000049999  swaps=4999950000  duration=7173353810
  OK  --> InsertionSort_v1 (size=100000): comps=4999950000  swaps=4999950000  duration=6790916549
  OK  --> InsertionSort_v2 (size=100000): comps=4999950000  swaps=4999950000  duration=7253777828
  OK  --> InsertionSort_v3 (size=100000): comps=3037891  swaps=4999950000  duration=3016640587
  OK  --> ShellSort_v1 (size=100000): comps=2244585  swaps=844560  duration=2447380
  OK  --> ShellSort_v2 (size=100000): comps=1865861  swaps=458016  duration=2613033
  OK  --> ShellSort_v3 (size=100000): comps=1941773  swaps=542104  duration=3748212
  OK  --> SelectionSort_v1 (size=100000): comps=4999950000  swaps=50000  duration=10645226445
  OK  --> HeapSort_v1 (size=100000): comps=2926640  swaps=1497434  duration=5600971
  OK  --> QuickSort_v1 (size=100000): comps=5000049999  swaps=50000  duration=4129812072
  OK  --> MergeSort_v1 (size=100000): comps=815024  swaps=1668928  duration=2344126
   первые и последние 10 элементов массива из 1000000 элементов: 
   999999 999998 999997 999996 999995 999994 999993 999992 999991 999990 ... 999999 999998 999997 999996 999995 999994 999993 999992 999991 999990
  FAIL  --> BubbleSort_v1 (size=1000000): comps=0  swaps=0  duration=0
  FAIL  --> BubbleSort_v2 (size=1000000): comps=0  swaps=0  duration=0
  FAIL  --> InsertionSort_v1 (size=1000000): comps=0  swaps=0  duration=0
  FAIL  --> InsertionSort_v2 (size=1000000): comps=0  swaps=0  duration=0
  FAIL  --> InsertionSort_v3 (size=1000000): comps=0  swaps=0  duration=0
  OK  --> ShellSort_v1 (size=1000000): comps=26357530  swaps=9357504  duration=36899133
  OK  --> ShellSort_v2 (size=1000000): comps=23369384  swaps=6236456  duration=47055849
  OK  --> ShellSort_v3 (size=1000000): comps=22864392  swaps=5662150  duration=57447384
  FAIL  --> SelectionSort_v1 (size=1000000): comps=0  swaps=0  duration=0
  OK  --> HeapSort_v1 (size=1000000): comps=36001436  swaps=18333408  duration=77400576
  FAIL  --> QuickSort_v1 (size=1000000): comps=0  swaps=0  duration=0
  OK  --> MergeSort_v1 (size=1000000): comps=9884992  swaps=19951424  duration=36953824
   первые и последние 10 элементов массива из 10000000 элементов: 
   9999999 9999998 9999997 9999996 9999995 9999994 9999993 9999992 9999991 9999990 ... 9999999 9999998 9999997 9999996 9999995 9999994 9999993 9999992 9999991 9999990
  FAIL  --> BubbleSort_v1 (size=10000000): comps=0  swaps=0  duration=0
  FAIL  --> BubbleSort_v2 (size=10000000): comps=0  swaps=0  duration=0
  FAIL  --> InsertionSort_v1 (size=10000000): comps=0  swaps=0  duration=0
  FAIL  --> InsertionSort_v2 (size=10000000): comps=0  swaps=0  duration=0
  FAIL  --> InsertionSort_v3 (size=10000000): comps=0  swaps=0  duration=0
  OK  --> ShellSort_v1 (size=10000000): comps=317626219  swaps=107626176  duration=413292346
  OK  --> ShellSort_v2 (size=10000000): comps=270522361  swaps=62945972  duration=411702475
  OK  --> ShellSort_v3 (size=10000000): comps=270520217  swaps=65109280  duration=568504704
  FAIL  --> SelectionSort_v1 (size=10000000): comps=0  swaps=0  duration=0
  OK  --> HeapSort_v1 (size=10000000): comps=426919609  swaps=216912428  duration=898086694
  FAIL  --> QuickSort_v1 (size=10000000): comps=0  swaps=0  duration=0
  OK  --> MergeSort_v1 (size=10000000): comps=114434624  swaps=233222784  duration=389480815

----------- [Group name: "0.random"] -----------
Name                    Test #      Processed     Array size    Comparisons          Swaps       Duration (ns)
BubbleSort_v1           Test 0      processed              1              0              0                 491
BubbleSort_v2           Test 0      processed              1              0              0                  93
InsertionSort_v1        Test 0      processed              1              0              0                 188
InsertionSort_v2        Test 0      processed              1              0              0                  53
InsertionSort_v3        Test 0      processed              1              0              0                  54
ShellSort_v1            Test 0      processed              1              0              0                 166
ShellSort_v2            Test 0      processed              1              0              0                  52
ShellSort_v3            Test 0      processed              1              0              0                  72
SelectionSort_v1        Test 0      processed              1              0              0                 372
HeapSort_v1             Test 0      processed              1              0              0                  58
QuickSort_v1            Test 0      processed              1              0              0                  60
MergeSort_v1            Test 0      processed              1              0              0                 162

BubbleSort_v1           Test 1      processed             10             45             16                 293
BubbleSort_v2           Test 1      processed             10             52             16                 304
InsertionSort_v1        Test 1      processed             10             24             16                 187
InsertionSort_v2        Test 1      processed             10             24             16                 186
InsertionSort_v3        Test 1      processed             10             55             16                 331
ShellSort_v1            Test 1      processed             10             29             10                 213
ShellSort_v2            Test 1      processed             10             25             10                 219
ShellSort_v3            Test 1      processed             10             23             12                 499
SelectionSort_v1        Test 1      processed             10             45              8                 204
HeapSort_v1             Test 1      processed             10             39             28                 363
QuickSort_v1            Test 1      processed             10             31              8                 339
MergeSort_v1            Test 1      processed             10             23             34                 668

BubbleSort_v1           Test 2      processed            100           4950           2157                9849
BubbleSort_v2           Test 2      processed            100           4819           2157               11431
InsertionSort_v1        Test 2      processed            100           2253           2157                3655
InsertionSort_v2        Test 2      processed            100           2253           2157                3127
InsertionSort_v3        Test 2      processed            100           1153           2157                4304
ShellSort_v1            Test 2      processed            100            877            413                3146
ShellSort_v2            Test 2      processed            100            834            403                3430
ShellSort_v3            Test 2      processed            100            734            331                3748
SelectionSort_v1        Test 2      processed            100           4950             95               10055
HeapSort_v1             Test 2      processed            100           1034            591                3312
QuickSort_v1            Test 2      processed            100            690            287                3720
MergeSort_v1            Test 2      processed            100            543            672                4923

BubbleSort_v1           Test 3      processed           1000         499500         247489              694666
BubbleSort_v2           Test 3      processed           1000         499372         247489             1198185
InsertionSort_v1        Test 3      processed           1000         248479         247489              334231
InsertionSort_v2        Test 3      processed           1000         248479         247489              341051
InsertionSort_v3        Test 3      processed           1000          18169         247489              185139
ShellSort_v1            Test 3      processed           1000          15296           7815               52080
ShellSort_v2            Test 3      processed           1000          14379           6975               56449
ShellSort_v3            Test 3      processed           1000          14113           6741               71027
SelectionSort_v1        Test 3      processed           1000         499500            993             1026751
HeapSort_v1             Test 3      processed           1000          16850           9065               40191
QuickSort_v1            Test 3      processed           1000          11404           4343               47010
MergeSort_v1            Test 3      processed           1000           8721           9976               60862

BubbleSort_v1           Test 4      processed          10000       49995000       25288624           116077892
BubbleSort_v2           Test 4      processed          10000       49997497       25288624           142014003
InsertionSort_v1        Test 4      processed          10000       25298615       25288624            38869048
InsertionSort_v2        Test 4      processed          10000       25298615       25288624            38914791
InsertionSort_v3        Test 4      processed          10000         247903       25288624            16882915
ShellSort_v1            Test 4      processed          10000         267306         152486              861080
ShellSort_v2            Test 4      processed          10000         255750         147412              961239
ShellSort_v3            Test 4      processed          10000         203477          96320              883085
SelectionSort_v1        Test 4      processed          10000       49995000           9992           111909479
HeapSort_v1             Test 4      processed          10000         235395         124222              516641
QuickSort_v1            Test 4      processed          10000         155089          72224              625077
MergeSort_v1            Test 4      processed          10000         120481         133616              683367

BubbleSort_v1           Test 5      processed         100000     4999950000     2501737763         13190496838
BubbleSort_v2           Test 5      processed         100000     5000040547     2501737763         13583163880
InsertionSort_v1        Test 5      processed         100000     2501837753     2501737763          3417213074
InsertionSort_v2        Test 5      processed         100000     2501837753     2501737763          3704149504
InsertionSort_v3        Test 5      processed         100000        3145309     2501737763          1560830355
ShellSort_v1            Test 5      processed         100000        4562610        3113279            11525372
ShellSort_v2            Test 5      processed         100000        3920114        2503799            13119732
ShellSort_v3            Test 5      processed         100000        2677097        1266093            12027367
SelectionSort_v1        Test 5      processed         100000     4999950000          99987         10771855647
HeapSort_v1             Test 5      processed         100000        3020053        1575003             7936077
QuickSort_v1            Test 5      processed         100000        2065627         938783             7819632
MergeSort_v1            Test 5      processed         100000        1536652        1668928             8648046

BubbleSort_v1           Test 6        skipped        1000000              0              0                   0
BubbleSort_v2           Test 6        skipped        1000000              0              0                   0
InsertionSort_v1        Test 6        skipped        1000000              0              0                   0
InsertionSort_v2        Test 6        skipped        1000000              0              0                   0
InsertionSort_v3        Test 6        skipped        1000000              0              0                   0
ShellSort_v1            Test 6      processed        1000000       68027835       50530714           168073437
ShellSort_v2            Test 6      processed        1000000       62283286       44889440           185020322
ShellSort_v3            Test 6      processed        1000000       33226631       15880632           156281015
SelectionSort_v1        Test 6        skipped        1000000              0              0                   0
HeapSort_v1             Test 6      processed        1000000       36792142       19046520           310188734
QuickSort_v1            Test 6        skipped        1000000              0              0                   0
MergeSort_v1            Test 6      processed        1000000       18674508       19951424           101071034

BubbleSort_v1           Test 7        skipped       10000000              0              0                   0
BubbleSort_v2           Test 7        skipped       10000000              0              0                   0
InsertionSort_v1        Test 7        skipped       10000000              0              0                   0
InsertionSort_v2        Test 7        skipped       10000000              0              0                   0
InsertionSort_v3        Test 7        skipped       10000000              0              0                   0
ShellSort_v1            Test 7      processed       10000000     1041605900      826620860          2271406653
ShellSort_v2            Test 7      processed       10000000     1040073945      832098474          2778838852
ShellSort_v3            Test 7      processed       10000000      398594255      191906810          1818186986
SelectionSort_v1        Test 7        skipped       10000000              0              0                   0
HeapSort_v1             Test 7      processed       10000000      434640382      223831712          6936685825
QuickSort_v1            Test 7        skipped       10000000              0              0                   0
MergeSort_v1            Test 7      processed       10000000      220101952      233222784          1229245380


----------- [Group name: "1.digits"] -----------
Name                    Test #      Processed     Array size    Comparisons          Swaps       Duration (ns)
BubbleSort_v1           Test 0      processed              1              0              0                 775
BubbleSort_v2           Test 0      processed              1              0              0                  52
InsertionSort_v1        Test 0      processed              1              0              0                 221
InsertionSort_v2        Test 0      processed              1              0              0                  53
InsertionSort_v3        Test 0      processed              1              0              0                  52
ShellSort_v1            Test 0      processed              1              0              0                 289
ShellSort_v2            Test 0      processed              1              0              0                  49
ShellSort_v3            Test 0      processed              1              0              0                  78
SelectionSort_v1        Test 0      processed              1              0              0                 493
HeapSort_v1             Test 0      processed              1              0              0                  59
QuickSort_v1            Test 0      processed              1              0              0                  61
MergeSort_v1            Test 0      processed              1              0              0                 805

BubbleSort_v1           Test 1      processed             10             45             21                 461
BubbleSort_v2           Test 1      processed             10             49             21                 236
InsertionSort_v1        Test 1      processed             10             28             21                 205
InsertionSort_v2        Test 1      processed             10             28             21                 200
InsertionSort_v3        Test 1      processed             10             51             21                 281
ShellSort_v1            Test 1      processed             10             32             15                 261
ShellSort_v2            Test 1      processed             10             28             15                 297
ShellSort_v3            Test 1      processed             10             29             19                 524
SelectionSort_v1        Test 1      processed             10             45              7                 174
HeapSort_v1             Test 1      processed             10             37             27                 290
QuickSort_v1            Test 1      processed             10             31             11                 420
MergeSort_v1            Test 1      processed             10             22             34                1165

BubbleSort_v1           Test 2      processed            100           4950           2401               10132
BubbleSort_v2           Test 2      processed            100           4959           2401               11558
InsertionSort_v1        Test 2      processed            100           2498           2401                3942
InsertionSort_v2        Test 2      processed            100           2498           2401                3504
InsertionSort_v3        Test 2      processed            100           1161           2401                4738
ShellSort_v1            Test 2      processed            100            691            228                2372
ShellSort_v2            Test 2      processed            100            644            208                2325
ShellSort_v3            Test 2      processed            100            623            219                3252
SelectionSort_v1        Test 2      processed            100           4950             88               10030
HeapSort_v1             Test 2      processed            100            980            524                3449
QuickSort_v1            Test 2      processed            100            880            221                2728
MergeSort_v1            Test 2      processed            100            547            672                4570

BubbleSort_v1           Test 3      processed           1000         499500         228217              573486
BubbleSort_v2           Test 3      processed           1000         495450         228217             1162970
InsertionSort_v1        Test 3      processed           1000         229214         228217              310787
InsertionSort_v2        Test 3      processed           1000         229214         228217              313919
InsertionSort_v3        Test 3      processed           1000          18261         228217              164143
ShellSort_v1            Test 3      processed           1000          10449           2863               29105
ShellSort_v2            Test 3      processed           1000           9991           2458               29614
ShellSort_v3            Test 3      processed           1000           9726           2226               36071
SelectionSort_v1        Test 3      processed           1000         499500            894             1021820
HeapSort_v1             Test 3      processed           1000          15801           8271               41127
QuickSort_v1            Test 3      processed           1000          54477           1292               83624
MergeSort_v1            Test 3      processed           1000           8488           9976               44804

BubbleSort_v1           Test 4      processed          10000       49995000       22471502           115245250
BubbleSort_v2           Test 4      processed          10000       49500490       22471502           138936119
InsertionSort_v1        Test 4      processed          10000       22481500       22471502            31264898
InsertionSort_v2        Test 4      processed          10000       22481500       22471502            35152044
InsertionSort_v3        Test 4      processed          10000         248871       22471502            13517163
ShellSort_v1            Test 4      processed          10000         153568          37573              356829
ShellSort_v2            Test 4      processed          10000         135590          25870              343193
ShellSort_v3            Test 4      processed          10000         129952          21427              394834
SelectionSort_v1        Test 4      processed          10000       49995000           8999           114839623
HeapSort_v1             Test 4      processed          10000         217737         112694              523963
QuickSort_v1            Test 4      processed          10000        5038978          22987             5550888
MergeSort_v1            Test 4      processed          10000         116872         133616              441055

BubbleSort_v1           Test 5      processed         100000     4999950000     2246197432         13026374145
BubbleSort_v2           Test 5      processed         100000     4949954955     2246197432         14138960877
InsertionSort_v1        Test 5      processed         100000     2246297430     2246197432          3454265443
InsertionSort_v2        Test 5      processed         100000     2246297430     2246197432          3469311510
InsertionSort_v3        Test 5      processed         100000        3154353     2246197432          1384727389
ShellSort_v1            Test 5      processed         100000        1860949         401432             4257007
ShellSort_v2            Test 5      processed         100000        1697481         267916             4110081
ShellSort_v3            Test 5      processed         100000        1635712         211740             4925729
SelectionSort_v1        Test 5      processed         100000     4999950000          90000         10632456683
HeapSort_v1             Test 5      processed         100000        2766452        1420513             6262320
QuickSort_v1            Test 5      processed         100000      500359982         239983           569527932
MergeSort_v1            Test 5      processed         100000        1483484        1668928             4792023

BubbleSort_v1           Test 6        skipped        1000000              0              0                   0
BubbleSort_v2           Test 6        skipped        1000000              0              0                   0
InsertionSort_v1        Test 6        skipped        1000000              0              0                   0
InsertionSort_v2        Test 6        skipped        1000000              0              0                   0
InsertionSort_v3        Test 6        skipped        1000000              0              0                   0
ShellSort_v1            Test 6      processed        1000000       22383596        4789821            72119500
ShellSort_v2            Test 6      processed        1000000       20307786        2781582            75751754
ShellSort_v3            Test 6      processed        1000000       19600952        2127650            77592792
SelectionSort_v1        Test 6        skipped        1000000              0              0                   0
HeapSort_v1             Test 6      processed        1000000       33483726       17110562            81399383
QuickSort_v1            Test 6        skipped        1000000              0              0                   0
MergeSort_v1            Test 6      processed        1000000       17970393       19951424            60010693

BubbleSort_v1           Test 7        skipped       10000000              0              0                   0
BubbleSort_v2           Test 7        skipped       10000000              0              0                   0
InsertionSort_v1        Test 7        skipped       10000000              0              0                   0
InsertionSort_v2        Test 7        skipped       10000000              0              0                   0
InsertionSort_v3        Test 7        skipped       10000000              0              0                   0
ShellSort_v1            Test 7      processed       10000000      270196822       54253917           740078577
ShellSort_v2            Test 7      processed       10000000      237536810       28239358           633334710
ShellSort_v3            Test 7      processed       10000000      229224325       21272122           713581935
SelectionSort_v1        Test 7        skipped       10000000              0              0                   0
HeapSort_v1             Test 7      processed       10000000      394975504      201210998           845606254
QuickSort_v1            Test 7        skipped       10000000              0              0                   0
MergeSort_v1            Test 7      processed       10000000      211522718      233222784           658882045


----------- [Group name: "2.sorted"] -----------
Name                    Test #      Processed     Array size    Comparisons          Swaps       Duration (ns)
BubbleSort_v1           Test 0      processed              1              0              0                 292
BubbleSort_v2           Test 0      processed              1              0              0                  59
InsertionSort_v1        Test 0      processed              1              0              0                 133
InsertionSort_v2        Test 0      processed              1              0              0                  52
InsertionSort_v3        Test 0      processed              1              0              0                  54
ShellSort_v1            Test 0      processed              1              0              0                 148
ShellSort_v2            Test 0      processed              1              0              0                  48
ShellSort_v3            Test 0      processed              1              0              0                  73
SelectionSort_v1        Test 0      processed              1              0              0                 280
HeapSort_v1             Test 0      processed              1              0              0                  53
QuickSort_v1            Test 0      processed              1              0              0                  65
MergeSort_v1            Test 0      processed              1              0              0                 206

BubbleSort_v1           Test 1      processed             10             45              9                 255
BubbleSort_v2           Test 1      processed             10             45              9                 193
InsertionSort_v1        Test 1      processed             10             18              9                 131
InsertionSort_v2        Test 1      processed             10             18              9                 161
InsertionSort_v3        Test 1      processed             10             59              9                 237
ShellSort_v1            Test 1      processed             10             22              1                 122
ShellSort_v2            Test 1      processed             10             27              9                 193
ShellSort_v3            Test 1      processed             10             14              1                 385
SelectionSort_v1        Test 1      processed             10             45              1                 164
HeapSort_v1             Test 1      processed             10             41             29                 334
QuickSort_v1            Test 1      processed             10             48              1                 298
MergeSort_v1            Test 1      processed             10             22             34                 865

BubbleSort_v1           Test 2      processed            100           4950             87                4047
BubbleSort_v2           Test 2      processed            100           3510             87                2624
InsertionSort_v1        Test 2      processed            100            186             87                 290
InsertionSort_v2        Test 2      processed            100            186             87                 346
InsertionSort_v3        Test 2      processed            100           1245             87                 972
ShellSort_v1            Test 2      processed            100            590             87                 984
ShellSort_v2            Test 2      processed            100            511             31                 959
ShellSort_v3            Test 2      processed            100            517             71                1887
SelectionSort_v1        Test 2      processed            100           4950              1                7797
HeapSort_v1             Test 2      processed            100           1081            645                2813
QuickSort_v1            Test 2      processed            100           2760              1                3795
MergeSort_v1            Test 2      processed            100            397            672                2338

BubbleSort_v1           Test 3      processed           1000         499500           6604              291169
BubbleSort_v2           Test 3      processed           1000         496930           6604              294769
InsertionSort_v1        Test 3      processed           1000           7603           6604                9581
InsertionSort_v2        Test 3      processed           1000           7603           6604                9946
InsertionSort_v3        Test 3      processed           1000          18375           6604               11661
ShellSort_v1            Test 3      processed           1000          10710           2722               29915
ShellSort_v2            Test 3      processed           1000          10413           2446               27763
ShellSort_v3            Test 3      processed           1000          10357           2450               32232
SelectionSort_v1        Test 3      processed           1000         499500             10             1009840
HeapSort_v1             Test 3      processed           1000          17560           9680               34554
QuickSort_v1            Test 3      processed           1000         175339            752              183035
MergeSort_v1            Test 3      processed           1000           6645           9976               21514

BubbleSort_v1           Test 4      processed          10000       49995000         666392            32258501
BubbleSort_v2           Test 4      processed          10000       49737454         666392            33472528
InsertionSort_v1        Test 4      processed          10000         676391         666392              968906
InsertionSort_v2        Test 4      processed          10000         676391         666392              968869
InsertionSort_v3        Test 4      processed          10000         248665         666392              458941
ShellSort_v1            Test 4      processed          10000         176309          56474              425115
ShellSort_v2            Test 4      processed          10000         171387          57928              525504
ShellSort_v3            Test 4      processed          10000         163917          51640              581476
SelectionSort_v1        Test 4      processed          10000       49995000            100           111430313
HeapSort_v1             Test 4      processed          10000         244307         131480              452759
QuickSort_v1            Test 4      processed          10000         802285          26100              630504
MergeSort_v1            Test 4      processed          10000         101839         133616              251156

BubbleSort_v1           Test 5      processed         100000     4999950000       65248998          3135119886
BubbleSort_v2           Test 5      processed         100000     4983441034       65248998          3125558863
InsertionSort_v1        Test 5      processed         100000       65348997       65248998            96763909
InsertionSort_v2        Test 5      processed         100000       65348997       65248998            99249896
InsertionSort_v3        Test 5      processed         100000        3146427       65248998            49706349
ShellSort_v1            Test 5      processed         100000        2833579        1334970             7362997
ShellSort_v2            Test 5      processed         100000        2509526        1042150             8307672
ShellSort_v3            Test 5      processed         100000        2258426         796742             8679817
SelectionSort_v1        Test 5      processed         100000     4999950000           1000         11047477492
HeapSort_v1             Test 5      processed         100000        3110584        1648620             5948476
QuickSort_v1            Test 5      processed         100000       17718857         469866            17100606
MergeSort_v1            Test 5      processed         100000        1341875        1668928             2761101

BubbleSort_v1           Test 6        skipped        1000000              0              0                   0
BubbleSort_v2           Test 6        skipped        1000000              0              0                   0
InsertionSort_v1        Test 6        skipped        1000000              0              0                   0
InsertionSort_v2        Test 6        skipped        1000000              0              0                   0
InsertionSort_v3        Test 6        skipped        1000000              0              0                   0
ShellSort_v1            Test 6      processed        1000000       38939516       20954426           104497351
ShellSort_v2            Test 6      processed        1000000       37313903       19378884           126355985
ShellSort_v3            Test 6      processed        1000000       28889906       11038830           117260146
SelectionSort_v1        Test 6        skipped        1000000              0              0                   0
HeapSort_v1             Test 6      processed        1000000       37692154       19765196            87386964
QuickSort_v1            Test 6        skipped        1000000              0              0                   0
MergeSort_v1            Test 6      processed        1000000       16606871       19951424            41766691

BubbleSort_v1           Test 7        skipped       10000000              0              0                   0
BubbleSort_v2           Test 7        skipped       10000000              0              0                   0
InsertionSort_v1        Test 7        skipped       10000000              0              0                   0
InsertionSort_v2        Test 7        skipped       10000000              0              0                   0
InsertionSort_v3        Test 7        skipped       10000000              0              0                   0
ShellSort_v1            Test 7      processed       10000000      624139826      404287028          1487268534
ShellSort_v2            Test 7      processed       10000000      580178096      367110546          1718728229
ShellSort_v3            Test 7      processed       10000000      353352946      141665472          1460486118
SelectionSort_v1        Test 7        skipped       10000000              0              0                   0
HeapSort_v1             Test 7      processed       10000000      444849813      231605924          1047595094
QuickSort_v1            Test 7        skipped       10000000              0              0                   0
MergeSort_v1            Test 7      processed       10000000      200958059      233222784           425703984


----------- [Group name: "3.revers"] -----------
Name                    Test #      Processed     Array size    Comparisons          Swaps       Duration (ns)
BubbleSort_v1           Test 0      processed              1              0              0                 231
BubbleSort_v2           Test 0      processed              1              0              0                  44
InsertionSort_v1        Test 0      processed              1              0              0                 114
InsertionSort_v2        Test 0      processed              1              0              0                  52
InsertionSort_v3        Test 0      processed              1              0              0                  54
ShellSort_v1            Test 0      processed              1              0              0                 153
ShellSort_v2            Test 0      processed              1              0              0                  49
ShellSort_v3            Test 0      processed              1              0              0                  75
SelectionSort_v1        Test 0      processed              1              0              0                 241
HeapSort_v1             Test 0      processed              1              0              0                  56
QuickSort_v1            Test 0      processed              1              0              0                  65
MergeSort_v1            Test 0      processed              1              0              0                 118

BubbleSort_v1           Test 1      processed             10             45             45                 181
BubbleSort_v2           Test 1      processed             10             54             45                 186
InsertionSort_v1        Test 1      processed             10             45             45                 206
InsertionSort_v2        Test 1      processed             10             45             45                 197
InsertionSort_v3        Test 1      processed             10             47             45                 250
ShellSort_v1            Test 1      processed             10             27             13                 183
ShellSort_v2            Test 1      processed             10             25             11                 271
ShellSort_v3            Test 1      processed             10             30             25                 428
SelectionSort_v1        Test 1      processed             10             45              5                 196
HeapSort_v1             Test 1      processed             10             35             21                 275
QuickSort_v1            Test 1      processed             10             54              5                 378
MergeSort_v1            Test 1      processed             10             15             34                 411

BubbleSort_v1           Test 2      processed            100           4950           4950                6778
BubbleSort_v2           Test 2      processed            100           5049           4950                6679
InsertionSort_v1        Test 2      processed            100           4950           4950                6991
InsertionSort_v2        Test 2      processed            100           4950           4950                6616
InsertionSort_v3        Test 2      processed            100           1059           4950                3709
ShellSort_v1            Test 2      processed            100            668            260                1291
ShellSort_v2            Test 2      processed            100            614            192                1604
ShellSort_v3            Test 2      processed            100            626            234                2394
SelectionSort_v1        Test 2      processed            100           4950             50                8206
HeapSort_v1             Test 2      processed            100            944            516                2907
QuickSort_v1            Test 2      processed            100           5049             50                5164
MergeSort_v1            Test 2      processed            100            316            672                2414

BubbleSort_v1           Test 3      processed           1000         499500         499500              698914
BubbleSort_v2           Test 3      processed           1000         500499         499500              708978
InsertionSort_v1        Test 3      processed           1000         499500         499500              659870
InsertionSort_v2        Test 3      processed           1000         499500         499500              698478
InsertionSort_v3        Test 3      processed           1000          16973         499500              280535
ShellSort_v1            Test 3      processed           1000          11716           4700               14158
ShellSort_v2            Test 3      processed           1000          10511           3424               18324
ShellSort_v3            Test 3      processed           1000          10788           3690               23264
SelectionSort_v1        Test 3      processed           1000         499500            500             1017458
HeapSort_v1             Test 3      processed           1000          15965           8316               33516
QuickSort_v1            Test 3      processed           1000         500499            500              511949
MergeSort_v1            Test 3      processed           1000           4932           9976               20811

BubbleSort_v1           Test 4      processed          10000       49995000       49995000            76989191
BubbleSort_v2           Test 4      processed          10000       50004999       49995000            74434999
InsertionSort_v1        Test 4      processed          10000       49995000       49995000            70517252
InsertionSort_v2        Test 4      processed          10000       49995000       49995000            74304599
InsertionSort_v3        Test 4      processed          10000         237261       49995000            30344961
ShellSort_v1            Test 4      processed          10000         172578          62560              196953
ShellSort_v2            Test 4      processed          10000         144824          36778              234213
ShellSort_v3            Test 4      processed          10000         148103          41884              306808
SelectionSort_v1        Test 4      processed          10000       49995000           5000           110143385
HeapSort_v1             Test 4      processed          10000         226682         116696              478250
QuickSort_v1            Test 4      processed          10000       50004999           5000            42490482
MergeSort_v1            Test 4      processed          10000          64608         133616              203274

BubbleSort_v1           Test 5      processed         100000     4999950000     4999950000          7200110603
BubbleSort_v2           Test 5      processed         100000     5000049999     4999950000          7173353810
InsertionSort_v1        Test 5      processed         100000     4999950000     4999950000          6790916549
InsertionSort_v2        Test 5      processed         100000     4999950000     4999950000          7253777828
InsertionSort_v3        Test 5      processed         100000        3037891     4999950000          3016640587
ShellSort_v1            Test 5      processed         100000        2244585         844560             2447380
ShellSort_v2            Test 5      processed         100000        1865861         458016             2613033
ShellSort_v3            Test 5      processed         100000        1941773         542104             3748212
SelectionSort_v1        Test 5      processed         100000     4999950000          50000         10645226445
HeapSort_v1             Test 5      processed         100000        2926640        1497434             5600971
QuickSort_v1            Test 5      processed         100000     5000049999          50000          4129812072
MergeSort_v1            Test 5      processed         100000         815024        1668928             2344126

BubbleSort_v1           Test 6        skipped        1000000              0              0                   0
BubbleSort_v2           Test 6        skipped        1000000              0              0                   0
InsertionSort_v1        Test 6        skipped        1000000              0              0                   0
InsertionSort_v2        Test 6        skipped        1000000              0              0                   0
InsertionSort_v3        Test 6        skipped        1000000              0              0                   0
ShellSort_v1            Test 6      processed        1000000       26357530        9357504            36899133
ShellSort_v2            Test 6      processed        1000000       23369384        6236456            47055849
ShellSort_v3            Test 6      processed        1000000       22864392        5662150            57447384
SelectionSort_v1        Test 6        skipped        1000000              0              0                   0
HeapSort_v1             Test 6      processed        1000000       36001436       18333408            77400576
QuickSort_v1            Test 6        skipped        1000000              0              0                   0
MergeSort_v1            Test 6      processed        1000000        9884992       19951424            36953824

BubbleSort_v1           Test 7        skipped       10000000              0              0                   0
BubbleSort_v2           Test 7        skipped       10000000              0              0                   0
InsertionSort_v1        Test 7        skipped       10000000              0              0                   0
InsertionSort_v2        Test 7        skipped       10000000              0              0                   0
InsertionSort_v3        Test 7        skipped       10000000              0              0                   0
ShellSort_v1            Test 7      processed       10000000      317626219      107626176           413292346
ShellSort_v2            Test 7      processed       10000000      270522361       62945972           411702475
ShellSort_v3            Test 7      processed       10000000      270520217       65109280           568504704
SelectionSort_v1        Test 7        skipped       10000000              0              0                   0
HeapSort_v1             Test 7      processed       10000000      426919609      216912428           898086694
QuickSort_v1            Test 7        skipped       10000000              0              0                   0
MergeSort_v1            Test 7      processed       10000000      114434624      233222784           389480815
```

##### Результаты теста с Random распределением

При этом распределении (сравнивал для N=100000).

Лучше всего себя ведет **QuickSort** (заняло `7.82` ms). Это объясняется тем, что рандомизированные значения - лучшая область для его применения, он заточен под рандомизированные распределения. Хотя у него сложность O(N^2), но в случае рандомных данных - алгоритм очень ускоряется.

Далее идет **HeapSort** (заняло `7.94` ms). Он использует бинарное дерево, что позволяет получить сложность O(N log N). Как отмечалось в прошлом ДЗ - это один из самых предсказуемых алгоритмов для различных типов распределений

Затем идет **MergeSort** (заняло `8.65` ms). Я объясняю это тем, что ему требуется дополнительная память, нужно выделять, освобождать ее, переносить элементы туда и обратно. Поэтому он отстает, хотя у него сложность тоже O(N log N).

Следом можно отметить **ShellSort**_v3 (заняло `12.03` ms). Это тоже отличный результат. Из-за особенностей алгоритма выбора шага, достигается сложность алгоритма значительно ниже чем O(N^2), несколько больше O(N log N).

Остальные алгоритмы в целом ведут себя ожидаемо.

Но вот когда смотрим на огромные N (N=10000000), когда могут работать только **MergeSort**, **HeapSort** и **ShellSort**, всё становится интереснее.

В лидеры выходит именно **MergeSort** (заняло `1.229` сек), затем идет **ShellSort** (заняло `1.818` сек) и только потом **HeapSort** (заняло `6.937` сек).  
В прошлой ДЗ мы уже видели, что на больших N **HeapSort** начинает отставать от **ShellSort**. А в данном случае **MergeSort** выигрывает всех, даже за счет накладных расходов на управление дополнительной памятью и на рекурсивные вызовы, с увеличением N он сильно сокращает количество операций сравнений и обменов.

##### Результаты теста с Digits распределением (особенность в том, что очень мало случайных значений)

При этом распределении (сравнивал для N=100000).

Лучше всего себя ведет как ни странно именно **ShellSort**_v2 (заняло `4.119` ms). У него сложность сильно меньше O(N^2), несколько больше O(N log N). Видимо в данном случае алгоритм подбора шага под конкретный вид распределения подходит как нельзя хорошо.

Затем идет алгоритм **MergeSort** (заняло `4.79` ms). Благодаря своей сложности O(N log N), он уверенно справляется и с таким типом распределения. Хотя ему и требуется больше расходовать ресурсы на работу с дополнительной памятью. Я этой дополнительной сложностью объясняю то, что алгоритм, имея теоретически лучшую сложность, слегка отстал.

Затем идет **HeapSort** (заняло `6.26` ms). Доказывает, что он очень предсказуемый алгоритм со сложностью O(N log N), применяемый для любых вариантов распределений. Да, не идеальный (шатание элементов кучи, в которой много одинаковых узлов - не дает преимуществ), но надёжный.

А вот **QuickSort** провалился (заняло `569.52` ms). Ему не нравится такое распределение. В нем огромное количество одинаковых (не уникальных) элементов, поэтому выбранные опорный элемент неудачно делит массив и алгоритм начинает скатываться к сложности O(N).

Остальные алгоритмы в целом ведут себя ожидаемо.

Когда смотрим на огромные N (N=10000000), когда могут работать только **MergeSort**, **HeapSort** и **ShellSort**, тут ситуация не особо меняется.

Лидирует **ShellSort**_v2, затем **MergeSort** и затем **HeapSort**.

##### Результаты теста с Sorted распределением (особенность в том, что уже почти отсортированный массив)

При этом распределении (сравнивал для N=100000).

Лучше всего себя показывает **MergeSort** (заняло `2.76` ms). Думаю, что это объясняется тем, что он использует преимущество частичной сортировки, ему нужно делать меньше работы при слиянии.

Далее идет **HeapSort** (заняло `5.95` ms). Доказывает, что и для такого типа распределения его можно эффективно использовать. Тут сложность в том, что для этапа формирования правильной кучи - он выстраивает элементы массива преимущественно в убывающем порядке, а только потом начинает сортировать в возрастающем порядке. А у нас изначально массив задан на 99% уже отсортированным, т.е. выполняется лишнее действие.

Затем идет как ни странно именно **ShellSort**_v1** (заняло `7.36` ms). Видимо именно алгоритм изменения шага делением на 2 показывает себя лучше для такого типа распределения.

А вот **QuickSort** опять провалился (заняло `17.1` ms). Ему такое распределение хоть и приятнее, но не очень. Опять же, текущий алгоритм выбора опорного элемента видимо неудачно делит массив и алгоритм начинает скатываться к сложности O(N), хотя держится всё же молодцом.

Когда смотрим на огромные N (N=10000000), когда могут работать только **MergeSort**, **HeapSort** и **ShellSort**, тут ситуация тоже не особо меняется.

Лидирует **MergeSort**, затем **HeapSort**, затем **ShellSort**.

##### Результаты теста с Revers распределением

При этом распределении (сравнивал для N=100000).

Лучше всего себя показывает **MergeSort** (заняло `2.34` ms).

Затем идет как ни странно именно **ShellSort**_v1** (заняло `2.45` ms).

Далее идет **HeapSort** (заняло `5.60` ms). Доказывает, что и для такого типа распределения его можно эффективно использовать.

Алгоритм **QuickSort** очень провалился (заняло `10.645` сек). Ему такое распределение ну очень сильно не нравится. При обратно отсортированных массивах он ведет себя сопоставимо **BubbleSort**.

Когда смотрим на огромные N (N=10000000), когда могут работать только **MergeSort**, **HeapSort** и **ShellSort**, тут ситуация тоже не особо меняется.

Лидирует **MergeSort**, затем **ShellSort**, затем **HeapSort**.

##### Дополнительные моменты анализа

Сравнивая все написанные ранее алгоритмы, наверное вывод такой, что при возможности - лучше использовать алгоритм слиянием (**MergeSort**), если нет требования in-place. Если же такое требование есть, то лучше реализовать пирамидальную сортировку (**HeapSort**), т.к. это очень предсказуемо-надежный и универсальный алгоритм под разные типы распределений.

Выбор алгоритма **ShellSort** тоже хороший, однако у меня нет понимания какую именно реализацию лучше взять, как определить что под данный набор подойдет именно такая функция выбора gap - я не знаю.

Алгоритм **QuickSort** очень ситуативный, поэтому наверное лучше выбрать какой-то другой.

### Уровень MIDDLE

Реализована функция для генерации текстового файла из N строчек, на каждой строке записано случайное число от 1 до T. (тут `hw08/main_external_sort.cpp` в методе **_generate_test_file**).

#### ES1

Реализация алгоритма в файле `hw08/ExternalSort.cpp` в методе **sort_v1**.

Алгоритм ExternalSort_v1 работает через создание T разных временных файлов с последующим слиянием. Файлов создается столько, сколько уникальных ключей в исходных данных. Поэтому в тестах я ограничил этот алгоритм, и если **T>1000**, то пропускаю его, чтобы не создавалось очень много файловых дескрипторов.

На ФАЗЕ 1 - читаем входной файл и распределяем числа по временным файлам. В каждый временный файл будут попадать элементы с одним и тем же значением и таким образом порядок их будет сохраняться.

На ФАЗЕ 2 - объединяем временные файлы в один выходной.
поочереди открываем временные файлы и переписываем их содержимое в итоговый файл

Из минусов этого алгоритма - создает **T** вспомогательных файлов, и при T=1000000 например - это очень неприятно. В целом, можно реализовать алгоритм так, чтобы не держать открытыми файлы постоянно, чтобы не попасть в ситуацию с ограничением на открытые файловые дескрипторы у процесса. Но тогда придется переоткрывать файлы для дозаписи после каждого числа, а это медленнее.

У меня в реализации хранение вспомогательных файлов с привязкой к ключам (уникальным значениям чисел) сделано через std::map. Потому что эта структура данных как раз автоматически выполняет сортировку по ключу. Однако, можно было бы реализовать через приоритетную очередь с динамическим количеством приоритетов (ранее в ДЗ 04 я реализовывал приоритетную очередь, но она была с фиксированным количеством приоритетов).

Также теоретически он требует, чтобы все значения были в диапазоне [1..T], т.е. эту контекстную информацию нужно знать заранее.

Однако реализация - максимально простая, работает за один проход по N элементам на чтение из входного файла, и по одному проходу на каждый вспомогательный файл, чтобы переписать из них данные в итоговый файл.

Сложность по оперативной памяти - O(1) - дополнительно в памяти ничего не держим. А вот по открытым файловым дескрипторам - O(T). Сложность алгоритма - линейная O(N) и реализация очень простая.

#### ES2

Реализация алгоритма в файле `hw08/ExternalSort.cpp` в методе **sort_v2** (продолжается в **_split**, **_merge** и **_merge_segment**).

Алгоритм ExternalSort_v2 работает через два вспомогательных файла F1 и F2. Исходим из идеи разбивания данных на сегменты и их упорядочивания. При этом известно, что отрезок (сегмент), состоящий из одного элемента, упорядочен всегда.

На ФАЗЕ 0 (выполняется только один раз для инициализации) - читаем исходный файл и распределяем по одному элементу (сегмент из N=1) в каждый вспомогательный файл.

На ФАЗЕ 1 - читаем файл с данными (начиная со второй итерации это выходной файл, там данные) и распределяем элементы по сегментами в каждый вспомогательный файл.

На ФАЗЕ 2 - в выходной файл F0 сливаем сегменты из вспомогательных файлов F1 и F2, получим в файлах данные сегментами по N элементов.

Далее будем повторять в цикле. На каждой итерации цикла задаем размер сегмента в 2 раза больше предыдущего шага. Повторяем, пока не отсортируем весь входной массив, т.е. размер сегмента должен стать больше количества элементов входных данных.

Из плюсов - работает с любыми данными, тут нет привязки к диапазону T. Также мы контролируем количество временных файлов, их всего два, нет проблемы с ограничением открытых файловых дескрипторов на процесс.

Сложность по оперативной памяти - O(1) - ничего дополнительно в памяти держать не надо. По открытым файловым дескрипторам - O(1) - всего два файла. Сложность алгоритма - O(N log N).

#### ES3

Реализация алгоритма в файле `hw08/ExternalSort.cpp` в методе **sort_v3** (продолжается в **sort_recursive** и **merge**).

Алгоритм ExternalSort_v3 работает так, что при первом проходе в память загружать блоки по 100 чисел, затем начинает сортировать их алгоритмом вставками (InsertionSort_v3) и отправлять на выход во вспомогательный файл. Затем берет этот вспомогательный файл как входной и идет в алгоритм ExternalSort_v2, передавая в алгоритм размер сегмента.

На маленьких размерах блоков InsertionSort работает быстро из-за локальности данных в массивах, хорошо кешируются процессором.

Такой подход дает некоторое ускорение. Раз уж мы предварительно сортируем блоками по 100 элементов, т.е. фактически мы уже во вспомогательном файле формируем отсортированные серии по 100 элементов. То можно в алгоритм ExternalSort_v2 передать размер сегмента, чтобы начинать не с размера 1, а с размера 100. Это позволяет пропустить несколько итераций (с размерами сегментов 1, 2, 4, 8, 16, 32, 64). Ведь основная сложность алгоритмов ExternalSort заключается в операциях ввода-вывода, а не в самой сортировке.

Я думаю, что такой подход должен бы дать приятный выигрыш в производительности. Вот и поглядим.

#### Сравнительный анализ реализаций сортировок файлов

Тестировались алгоритмы: **ExternalSort_v1**, **ExternalSort_v2**, **ExternalSort_v3**.

Тестировались алгоритмы на данных со случайным распределением, числа были из диапазона `[1..T]`.

Тестирование проводилось на наборах массивов нескольких размеров: **100**, **1000**, **10000**, **100000**, **1000000**. В двух комбинациях: сначала при T=10, затем T=N.

Сначала генерируется файл с исходными данными в каталоге `hw08/external_tests`, по одному файлу на каждую конфигурацию входных данных. Затем этот файл скармливается каждому алгоритму, и каждый алгоритм в этом же каталоге формирует итоговый файл (отличаются суффиксами `__v1`, `__v2`, `__v3`), по одному файлу на каждую конфигурацию входных данных.

При тестировании алгоритмов учитывалось только время выполнения, т.к. основная сложность связана с операциями ввода-вывода через файловую систему, которая вообще говоря в свою очередь тоже может накладывать и свои эффекты. Поэтому рассматривался только совокупный параметр затраченного времени.

После запуска программы, она сформирует таблицу-отчет:

```bash
dmitry@lachugin:~/otus/lachugin_algorithms_hw/hw08$ ./hw08_external_sort 
программа оценки работы алгоритмов внешней сортировки (ExternalSort)

==============================
TEST: N=100, T=10
генерация 100 чисел (1..10) в файле "/home/dmitry/otus/lachugin_algorithms_hw/hw08/external_tests/input_n100_t10.txt"... 
сгенерировано 100 чисел за 46718 ns
/home/dmitry/otus/lachugin_algorithms_hw/hw08/external_tests/input_n100_t10.txt:
   5 3 4 7 7 2 7 1 4 5 2 7 4 4 1 1 6 2 4 8 ...
/home/dmitry/otus/lachugin_algorithms_hw/hw08/external_tests/output_n100_t10__v1.txt:
   1 1 1 1 1 1 1 1 1 1 1 1 1 1 2 2 2 2 2 2 ...
/home/dmitry/otus/lachugin_algorithms_hw/hw08/external_tests/output_n100_t10__v2.txt:
   1 1 1 1 1 1 1 1 1 1 1 1 1 1 2 2 2 2 2 2 ...
/home/dmitry/otus/lachugin_algorithms_hw/hw08/external_tests/output_n100_t10__v3.txt:
   1 1 1 1 1 1 1 1 1 1 1 1 1 1 2 2 2 2 2 2 ...

==============================
TEST: N=1000, T=10
генерация 1000 чисел (1..10) в файле "/home/dmitry/otus/lachugin_algorithms_hw/hw08/external_tests/input_n1000_t10.txt"... 
сгенерировано 1000 чисел за 211318 ns
/home/dmitry/otus/lachugin_algorithms_hw/hw08/external_tests/input_n1000_t10.txt:
   8 1 5 9 5 9 10 4 9 9 9 3 4 4 8 4 6 3 7 8 ...
/home/dmitry/otus/lachugin_algorithms_hw/hw08/external_tests/output_n1000_t10__v1.txt:
   1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 ...
/home/dmitry/otus/lachugin_algorithms_hw/hw08/external_tests/output_n1000_t10__v2.txt:
   1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 ...
/home/dmitry/otus/lachugin_algorithms_hw/hw08/external_tests/output_n1000_t10__v3.txt:
   1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 ...

==============================
TEST: N=10000, T=10
генерация 10000 чисел (1..10) в файле "/home/dmitry/otus/lachugin_algorithms_hw/hw08/external_tests/input_n10000_t10.txt"... 
сгенерировано 10000 чисел за 1130117 ns
/home/dmitry/otus/lachugin_algorithms_hw/hw08/external_tests/input_n10000_t10.txt:
   10 3 4 6 8 1 1 9 4 6 6 6 6 7 1 3 2 10 2 2 ...
/home/dmitry/otus/lachugin_algorithms_hw/hw08/external_tests/output_n10000_t10__v1.txt:
   1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 ...
/home/dmitry/otus/lachugin_algorithms_hw/hw08/external_tests/output_n10000_t10__v2.txt:
   1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 ...
/home/dmitry/otus/lachugin_algorithms_hw/hw08/external_tests/output_n10000_t10__v3.txt:
   1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 ...

==============================
TEST: N=100000, T=10
генерация 100000 чисел (1..10) в файле "/home/dmitry/otus/lachugin_algorithms_hw/hw08/external_tests/input_n100000_t10.txt"... 
сгенерировано 100000 чисел за 6662287 ns
/home/dmitry/otus/lachugin_algorithms_hw/hw08/external_tests/input_n100000_t10.txt:
   6 8 2 4 10 6 6 10 4 5 10 6 8 5 4 3 3 5 3 1 ...
/home/dmitry/otus/lachugin_algorithms_hw/hw08/external_tests/output_n100000_t10__v1.txt:
   1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 ...
/home/dmitry/otus/lachugin_algorithms_hw/hw08/external_tests/output_n100000_t10__v2.txt:
   1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 ...
/home/dmitry/otus/lachugin_algorithms_hw/hw08/external_tests/output_n100000_t10__v3.txt:
   1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 ...

==============================
TEST: N=1000000, T=10
генерация 1000000 чисел (1..10) в файле "/home/dmitry/otus/lachugin_algorithms_hw/hw08/external_tests/input_n1000000_t10.txt"... 
сгенерировано 1000000 чисел за 67989569 ns
/home/dmitry/otus/lachugin_algorithms_hw/hw08/external_tests/input_n1000000_t10.txt:
   4 3 9 7 2 7 7 3 6 4 1 8 7 5 3 7 7 4 2 8 ...
/home/dmitry/otus/lachugin_algorithms_hw/hw08/external_tests/output_n1000000_t10__v1.txt:
   1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 ...
/home/dmitry/otus/lachugin_algorithms_hw/hw08/external_tests/output_n1000000_t10__v2.txt:
   1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 ...
/home/dmitry/otus/lachugin_algorithms_hw/hw08/external_tests/output_n1000000_t10__v3.txt:
   1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 ...

==============================
TEST: N=100, T=100
генерация 100 чисел (1..100) в файле "/home/dmitry/otus/lachugin_algorithms_hw/hw08/external_tests/input_n100_t100.txt"... 
сгенерировано 100 чисел за 9400 ns
/home/dmitry/otus/lachugin_algorithms_hw/hw08/external_tests/input_n100_t100.txt:
   84 86 85 5 11 62 66 77 51 96 76 21 85 5 65 32 58 27 94 77 ...
/home/dmitry/otus/lachugin_algorithms_hw/hw08/external_tests/output_n100_t100__v1.txt:
   1 1 1 2 3 4 5 5 5 7 10 11 12 14 15 17 18 19 21 23 ...
/home/dmitry/otus/lachugin_algorithms_hw/hw08/external_tests/output_n100_t100__v2.txt:
   1 1 1 2 3 4 5 5 5 7 10 11 12 14 15 17 18 19 21 23 ...
/home/dmitry/otus/lachugin_algorithms_hw/hw08/external_tests/output_n100_t100__v3.txt:
   1 1 1 2 3 4 5 5 5 7 10 11 12 14 15 17 18 19 21 23 ...

==============================
TEST: N=1000, T=1000
генерация 1000 чисел (1..1000) в файле "/home/dmitry/otus/lachugin_algorithms_hw/hw08/external_tests/input_n1000_t1000.txt"... 
сгенерировано 1000 чисел за 67026 ns
/home/dmitry/otus/lachugin_algorithms_hw/hw08/external_tests/input_n1000_t1000.txt:
   207 662 526 493 658 933 307 156 370 957 27 940 967 74 300 881 969 210 194 284 ...
/home/dmitry/otus/lachugin_algorithms_hw/hw08/external_tests/output_n1000_t1000__v1.txt:
   1 1 2 2 3 3 3 5 5 5 6 6 6 7 7 7 7 8 8 9 ...
/home/dmitry/otus/lachugin_algorithms_hw/hw08/external_tests/output_n1000_t1000__v2.txt:
   1 1 2 2 3 3 3 5 5 5 6 6 6 7 7 7 7 8 8 9 ...
/home/dmitry/otus/lachugin_algorithms_hw/hw08/external_tests/output_n1000_t1000__v3.txt:
   1 1 2 2 3 3 3 5 5 5 6 6 6 7 7 7 7 8 8 9 ...

==============================
TEST: N=10000, T=10000
генерация 10000 чисел (1..10000) в файле "/home/dmitry/otus/lachugin_algorithms_hw/hw08/external_tests/input_n10000_t10000.txt"... 
сгенерировано 10000 чисел за 737107 ns
/home/dmitry/otus/lachugin_algorithms_hw/hw08/external_tests/input_n10000_t10000.txt:
   9537 2807 567 7627 4203 6641 7114 7084 8024 7288 4844 3438 6658 5329 8908 3878 3767 6759 8061 9313 ...
ExternalSort_v1 пропущено: T=10000 слишком большое
/home/dmitry/otus/lachugin_algorithms_hw/hw08/external_tests/output_n10000_t10000__v2.txt:
   3 3 6 7 8 8 8 10 11 12 13 14 15 15 16 17 20 20 21 21 ...
/home/dmitry/otus/lachugin_algorithms_hw/hw08/external_tests/output_n10000_t10000__v3.txt:
   3 3 6 7 8 8 8 10 11 12 13 14 15 15 16 17 20 20 21 21 ...

==============================
TEST: N=100000, T=100000
генерация 100000 чисел (1..100000) в файле "/home/dmitry/otus/lachugin_algorithms_hw/hw08/external_tests/input_n100000_t100000.txt"... 
сгенерировано 100000 чисел за 7812368 ns
/home/dmitry/otus/lachugin_algorithms_hw/hw08/external_tests/input_n100000_t100000.txt:
   12324 48780 97820 64302 86169 32448 86902 40196 64314 41309 34803 89296 81904 59952 25874 76046 62746 47288 91262 89637 ...
ExternalSort_v1 пропущено: T=100000 слишком большое
/home/dmitry/otus/lachugin_algorithms_hw/hw08/external_tests/output_n100000_t100000__v2.txt:
   1 2 2 4 5 7 8 8 8 9 14 14 16 16 16 16 16 17 18 19 ...
/home/dmitry/otus/lachugin_algorithms_hw/hw08/external_tests/output_n100000_t100000__v3.txt:
   1 2 2 4 5 7 8 8 8 9 14 14 16 16 16 16 16 17 18 19 ...

==============================
TEST: N=1000000, T=1000000
генерация 1000000 чисел (1..1000000) в файле "/home/dmitry/otus/lachugin_algorithms_hw/hw08/external_tests/input_n1000000_t1000000.txt"... 
сгенерировано 1000000 чисел за 77451843 ns
/home/dmitry/otus/lachugin_algorithms_hw/hw08/external_tests/input_n1000000_t1000000.txt:
   162848 395306 245151 532800 670517 150598 853077 578011 276609 474995 399934 987968 814916 822617 638785 534367 182727 431712 830003 130350 ...
ExternalSort_v1 пропущено: T=1000000 слишком большое
/home/dmitry/otus/lachugin_algorithms_hw/hw08/external_tests/output_n1000000_t1000000__v2.txt:
   1 1 3 3 4 6 6 7 7 7 7 8 8 8 10 11 11 14 14 14 ...
/home/dmitry/otus/lachugin_algorithms_hw/hw08/external_tests/output_n1000000_t1000000__v3.txt:
   1 1 3 3 4 6 6 7 7 7 7 8 8 8 10 11 11 14 14 14 ...

Name                     Processing  Unique values     Array size       Duration (ns)
ExternalSort_v1           processed             10            100             3577744
ExternalSort_v2           processed             10            100             4877864
ExternalSort_v3           processed             10            100              644011
ExternalSort_v1           processed             10           1000             2521280
ExternalSort_v2           processed             10           1000            10095002
ExternalSort_v3           processed             10           1000             2585251
ExternalSort_v1           processed             10          10000             2900910
ExternalSort_v2           processed             10          10000            30500011
ExternalSort_v3           processed             10          10000            14833432
ExternalSort_v1           processed             10         100000            17052905
ExternalSort_v2           processed             10         100000           321418791
ExternalSort_v3           processed             10         100000           196882238
ExternalSort_v1           processed             10        1000000           166326194
ExternalSort_v2           processed             10        1000000          3708923298
ExternalSort_v3           processed             10        1000000          2665978818
ExternalSort_v1           processed            100            100             2398843
ExternalSort_v2           processed            100            100             1541687
ExternalSort_v3           processed            100            100              188321
ExternalSort_v1           processed           1000           1000            28015652
ExternalSort_v2           processed           1000           1000             3270434
ExternalSort_v3           processed           1000           1000             1563138
ExternalSort_v1             skipped          10000          10000                   0
ExternalSort_v2           processed          10000          10000            35727831
ExternalSort_v3           processed          10000          10000            19232777
ExternalSort_v1             skipped         100000         100000                   0
ExternalSort_v2           processed         100000         100000           418622031
ExternalSort_v3           processed         100000         100000           273683343
ExternalSort_v1             skipped        1000000        1000000                   0
ExternalSort_v2           processed        1000000        1000000          5501211907
ExternalSort_v3           processed        1000000        1000000          4037146622
```

Как раз хорошо видно несколько интересных моментов.

**ExternalSort_v1** при малом T (10) практически не зависит от N, с увеличением N показывает фактически линейное время. Хорош только при очень малом T (10). Как только становится больше T, эго эффективность падает, слишком много файлов. Объясняю это влиянием файловой системы, что сложнее работать с большим количеством открытых файлов, много операций создания/удаления/обновления файлов и ФС начинает притормаживать, возможно потому что операции буферизируются.

**ExternalSort_v2** дает средний результат. Это наверное классическая реализация, с предсказуемым поведением, не требует много файлов, не зависит от диапазона данных. Однако сложность алгоритма выше, он делает много проходов (наверное что-то вроде log (N/segment_size) ?) и каждый проход - полное считывание файла. Однако из-за того что требует всего 2 файла, это легче для файловой системы, и при увеличении T алгоритм ExternalSort_v2 начинает заметно обходить ExternalSort_v1.

**ExternalSort_v3** с предварительной сортировкой блоков дает очень хорошие результаты. Даже при малом T он обходит ExternalSort_v1, и при больших T и N обходит ExternalSort_v2 за счет того, что уменьшает количество проходов по файлу, а операции ввода-вывода дорогие, сильно дороже операций сравнения и обменов в оперативной памяти (которые в InsertionSort). Однако, чем больше N, тем сильнее сокращается разница с ExternalSort_v2. Тут влияет не очень большой размер блока сортировки (100 чисел), по идее, если его адаптировать, то на больших N станет эффективнее. Т.е. сейчас при большом N доминирует влияние слияния (операций ввода-вывода), а не ускорение от предварительной сортировки.
