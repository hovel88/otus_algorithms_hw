# ДЗ 07: Алгоритмы сортировки (SelectionSort, HeapSort)

## Цель

Создание, тестирование и сравнение алгоритмов сортировки:

* сортировка выбором (SelectionSort)
* сортировка пирамидальная или сортировка кучи (HeapSort)

## Подготовка

Основные характеристики алгоритмов сортировки:

1. Стабильность - когда алгоритм не меняет порядок элементов с одинаковыми ключами (устойчивость). Обычно имеет смысл при сортировке объектов/записей, а не отдельных чисел.
2. Адаптивность - когда алгоритм сортировки предварительного отсортированного массива выполняется значительно быстрее, т.е. адаптируется к данным.
3. On-line - когда алгоритм анализирует данные последовательно получая элементы, а не на изначально полном объеме данных, т.е. для каждого нового элемента определяется его положение в уже отсортированном массиве.

| | SelectionSort | HeapSort |
| -- | -- | -- |
| Сложность | O(N^2) | O(N log N) |
| Стабильность | - | - |
| Адаптивность | - | - |
| On-line | - | - |

Для сборки исходников запустить `make`:

```bash
dmitry@lachugin:~/otus/lachugin_algorithms_hw/hw07$ make
g++ -lstdc++fs -std=c++17 -O2 -Wall -I. -o hw07_sort main_sort.cpp SelectionSort.cpp HeapSort.cpp
g++ -lstdc++fs -std=c++17 -O2 -Wall -I. -o hw07_checker checker.cpp checker_internals.cpp BubbleSort.cpp InsertionSort.cpp ShellSort.cpp SelectionSort.cpp HeapSort.cpp
```

Основными исследуемыми характеристиками алгоритмов сортировки являются:

* время выполнения сортировки массива размером N (**Duration (ns)**)
* количество операций сравнений (**Comparisons**)
* количество операций перестановки элементов (**Swaps**)

Все алгоритмы организованы в классы, унаследованные от базового интерфейсного класса **ISort**, это позволит легче производить тестирование, а также спрятать в базовом классе некоторые общие вещи.

**TODO:** Нужно распаковать архив `sorting_tests.zip` с тестами в каталог `hw07/sorting-tests`.

## Решение

### Уровень JUNIOR

#### SEL1

Реализация алгоритма в файле `hw07/SelectionSort.cpp` в методе **sort_v1**.

На каждом шаге находим минимальный элемент в неотсортированной части массива и меняем его с первым элементом этой части.

#### HIP1

Реализация алгоритма в файле `hw07/HeapSort.cpp` в методе **sort_v1**.

Сначала строим из массива кучу (heap), затем многократно свапаем максимальный элемент (корень) с минимальным элементом (листом), и восстанавливаем свойство кучи (если основное свойство кучи нарушается, родитель меняется с бОльшим из своих потомков).

Куча это полное бинарное дерево, а основное свойство кучи - приоритет вершины больше приоритетов её потомков.  
Высота кучи: `log2(n + 1)` с округлением к большему целому.  
Потомки есть у первых `size/2` элементов кучи.  
Левый потомок узла с индексом **i** имеет индекс `2*i + 1`.  
Правый потомок узла с индексом **i** имеет индекс `2*i + 2`.  
Предок узла с индексом **p** имеет индекс `(p - 1)/2`.

### Уровень MIDDLE

#### Сравнительный анализ базовых реализаций

Тестировались алгоритмы: **SelectionSort_v1**, **HeapSort_v1**.

Тестирование проводилось на наборах массивов нескольких размеров: **100**, **1000**, **10000**, **100000**.  
Не проводил тестирование на размере массива 1000000, т.к. уже были получены показательные результаты и на меньшем объеме данных.

Для теста создавался один массив максимального размера, заполнялся произвольными числами от 0 до 99.  
Затем, все варианты массивов меньшего размера формировались путём создания копии первых N элементов из массива максимального размера.  
А также для каждого подопытного алгоритма сортировки формировалась своя копия исходного массива произвольных чисел.  
Таким образом старался привести эксперимент ближе к одинаковым условиям.

После запуска программы, она сформирует таблицу-отчет:

```bash
dmitry@lachugin:~/otus/lachugin_algorithms_hw/hw07$ ./hw07_sort 
программа оценки работы алгоритмов сортировки (SelectionSort, HeapSort) в базовых версиях
Первые и последние 20 элементов массива из 100000 элементов: 
   99 21 73 51 21 4 37 2 28 28 27 97 21 53 6 73 38 12 57 12 ... 19 69 43 17 2 86 72 45 13 19 91 77 27 46 9 43 57 58 49 48
Name                     Array size    Comparisons          Swaps       Duration (ns)
SelectionSort_v1                100           4950             92               14834
HeapSort_v1                     100           1014            566                4847
SelectionSort_v1               1000         499500            988             1451928
HeapSort_v1                    1000          16799           9010               62242
SelectionSort_v1              10000       49995000           9903           113182518
HeapSort_v1                   10000         234345         123312              526371
SelectionSort_v1             100000     4999950000          98985         10418949726
HeapSort_v1                  100000        3002545        1562697             6178940
```

Можно наблюдать, что у **SelectionSort** квадратичная сложность, пусть и мало операций обменов, но очень много операций сравнений. С увеличением N на порядки растет и время работы алгоритма.

У алгоритма **HeapSort** сложность по операциям сравнений и времени ощутимо меньше, потому что это бинарное дерево, оно и работает быстрее, хотя приходится жертвовать количеством обменов.

### Уровень SENIOR

#### Комплексное тестирование алгоритмов

Для тестов используются наборы данных с несколькими различными характеристиками:

* random — массив из случайных чисел (`hw07/sorting-tests/0.random`)
* digits — массив из случайных цифр (`hw07/sorting-tests/1.digits`)
* sorted — на 99% отсортированный массив (`hw07/sorting-tests/2.sorted`)
* revers — обратно-отсортированный массив (`hw07/sorting-tests/3.revers`)

Написана система тестирования.

Тестируются все алгоритмы (включая алгоритмы из прошлой ДЗ с их оптимизациями): **BubbleSort_v1**, **BubbleSort_v2**, **InsertionSort_v1**, **InsertionSort_v2**, **InsertionSort_v3**, **ShellSort_v1**, **ShellSort_v2**, **ShellSort_v3**, **SelectionSort_v1**, **HeapSort_v1**.

Я ограничил **SelectionSort** также как и **BubbleSort** и **InsertionSort** по размеру N, т.к. занимало очень много времени.

После запуска, программа выводит сводную таблицу с результатами:

```bash
dmitry@lachugin:~/otus/lachugin_algorithms_hw/hw07$ ./hw07_checker 
Подготовлено 8 тестов из каталога "/home/dmitry/otus/lachugin_algorithms_hw/hw07/sorting-tests/0.random"
Подготовлено 8 тестов из каталога "/home/dmitry/otus/lachugin_algorithms_hw/hw07/sorting-tests/1.digits"
Подготовлено 8 тестов из каталога "/home/dmitry/otus/lachugin_algorithms_hw/hw07/sorting-tests/2.sorted"
Подготовлено 8 тестов из каталога "/home/dmitry/otus/lachugin_algorithms_hw/hw07/sorting-tests/3.revers"
Запускаем тест группы: "0.random"
   исходный массив из 1 элементов: 
   0
  OK  --> BubbleSort_v1 (size=1): comps=0  swaps=0  duration=422
  OK  --> BubbleSort_v2 (size=1): comps=0  swaps=0  duration=147
  OK  --> InsertionSort_v1 (size=1): comps=0  swaps=0  duration=143
  OK  --> InsertionSort_v2 (size=1): comps=0  swaps=0  duration=98
  OK  --> InsertionSort_v3 (size=1): comps=0  swaps=0  duration=100
  OK  --> ShellSort_v1 (size=1): comps=0  swaps=0  duration=47630
  OK  --> ShellSort_v2 (size=1): comps=0  swaps=0  duration=132
  OK  --> ShellSort_v3 (size=1): comps=0  swaps=0  duration=389
  OK  --> SelectionSort_v1 (size=1): comps=0  swaps=0  duration=203
  OK  --> HeapSort_v1 (size=1): comps=0  swaps=0  duration=150
   первые и последние 5 элементов массива из 10 элементов: 
   7 0 6 1 3 ... 7 0 6 1 3
  OK  --> BubbleSort_v1 (size=10): comps=45  swaps=16  duration=14230
  OK  --> BubbleSort_v2 (size=10): comps=52  swaps=16  duration=414
  OK  --> InsertionSort_v1 (size=10): comps=24  swaps=16  duration=332
  OK  --> InsertionSort_v2 (size=10): comps=24  swaps=16  duration=284
  OK  --> InsertionSort_v3 (size=10): comps=55  swaps=16  duration=476
  OK  --> ShellSort_v1 (size=10): comps=29  swaps=10  duration=378
  OK  --> ShellSort_v2 (size=10): comps=25  swaps=10  duration=381
  OK  --> ShellSort_v3 (size=10): comps=23  swaps=12  duration=1015
  OK  --> SelectionSort_v1 (size=10): comps=45  swaps=8  duration=298
  OK  --> HeapSort_v1 (size=10): comps=39  swaps=28  duration=573
   первые и последние 10 элементов массива из 100 элементов: 
   74 11 72 33 40 28 23 17 66 61 ... 74 11 72 33 40 28 23 17 66 61
  OK  --> BubbleSort_v1 (size=100): comps=4950  swaps=2157  duration=15699
  OK  --> BubbleSort_v2 (size=100): comps=4819  swaps=2157  duration=15967
  OK  --> InsertionSort_v1 (size=100): comps=2253  swaps=2157  duration=5814
  OK  --> InsertionSort_v2 (size=100): comps=2253  swaps=2157  duration=4846
  OK  --> InsertionSort_v3 (size=100): comps=1153  swaps=2157  duration=7236
  OK  --> ShellSort_v1 (size=100): comps=877  swaps=413  duration=4944
  OK  --> ShellSort_v2 (size=100): comps=834  swaps=403  duration=5638
  OK  --> ShellSort_v3 (size=100): comps=734  swaps=331  duration=6576
  OK  --> SelectionSort_v1 (size=100): comps=4950  swaps=95  duration=16056
  OK  --> HeapSort_v1 (size=100): comps=1034  swaps=591  duration=5461
   первые и последние 10 элементов массива из 1000 элементов: 
   561 233 825 817 336 551 811 627 485 812 ... 561 233 825 817 336 551 811 627 485 812
  OK  --> BubbleSort_v1 (size=1000): comps=499500  swaps=247489  duration=1198747
  OK  --> BubbleSort_v2 (size=1000): comps=499372  swaps=247489  duration=1153676
  OK  --> InsertionSort_v1 (size=1000): comps=248479  swaps=247489  duration=582076
  OK  --> InsertionSort_v2 (size=1000): comps=248479  swaps=247489  duration=539491
  OK  --> InsertionSort_v3 (size=1000): comps=18169  swaps=247489  duration=285476
  OK  --> ShellSort_v1 (size=1000): comps=15296  swaps=7815  duration=94597
  OK  --> ShellSort_v2 (size=1000): comps=14379  swaps=6975  duration=99862
  OK  --> ShellSort_v3 (size=1000): comps=14113  swaps=6741  duration=108039
  OK  --> SelectionSort_v1 (size=1000): comps=499500  swaps=993  duration=1633325
  OK  --> HeapSort_v1 (size=1000): comps=16850  swaps=9065  duration=61104
   первые и последние 10 элементов массива из 10000 элементов: 
   3839 1880 4323 1741 3390 3908 186 4357 9885 3111 ... 3839 1880 4323 1741 3390 3908 186 4357 9885 3111
  OK  --> BubbleSort_v1 (size=10000): comps=49995000  swaps=25288624  duration=118158672
  OK  --> BubbleSort_v2 (size=10000): comps=49997497  swaps=25288624  duration=114729473
  OK  --> InsertionSort_v1 (size=10000): comps=25298615  swaps=25288624  duration=36071861
  OK  --> InsertionSort_v2 (size=10000): comps=25298615  swaps=25288624  duration=37514191
  OK  --> InsertionSort_v3 (size=10000): comps=247903  swaps=25288624  duration=15042550
  OK  --> ShellSort_v1 (size=10000): comps=267306  swaps=152486  duration=796659
  OK  --> ShellSort_v2 (size=10000): comps=255750  swaps=147412  duration=877648
  OK  --> ShellSort_v3 (size=10000): comps=203477  swaps=96320  duration=864434
  OK  --> SelectionSort_v1 (size=10000): comps=49995000  swaps=9992  duration=106085481
  OK  --> HeapSort_v1 (size=10000): comps=235395  swaps=124222  duration=509537
   первые и последние 10 элементов массива из 100000 элементов: 
   8167 4911 1309 46835 39659 7048 82594 57334 60184 49556 ... 8167 4911 1309 46835 39659 7048 82594 57334 60184 49556
  OK  --> BubbleSort_v1 (size=100000): comps=4999950000  swaps=2501737763  duration=12704342487
  OK  --> BubbleSort_v2 (size=100000): comps=5000040547  swaps=2501737763  duration=13271785347
  OK  --> InsertionSort_v1 (size=100000): comps=2501837753  swaps=2501737763  duration=3380192553
  OK  --> InsertionSort_v2 (size=100000): comps=2501837753  swaps=2501737763  duration=3558060159
  OK  --> InsertionSort_v3 (size=100000): comps=3145309  swaps=2501737763  duration=1458090821
  OK  --> ShellSort_v1 (size=100000): comps=4562610  swaps=3113279  duration=11149029
  OK  --> ShellSort_v2 (size=100000): comps=3920114  swaps=2503799  duration=12353501
  OK  --> ShellSort_v3 (size=100000): comps=2677097  swaps=1266093  duration=11632168
  OK  --> SelectionSort_v1 (size=100000): comps=4999950000  swaps=99987  duration=10445350104
  OK  --> HeapSort_v1 (size=100000): comps=3020053  swaps=1575003  duration=6950214
   первые и последние 10 элементов массива из 1000000 элементов: 
   106757 84762 359524 454600 539912 260065 748214 424144 828940 441559 ... 106757 84762 359524 454600 539912 260065 748214 424144 828940 441559
  FAIL  --> BubbleSort_v1 (size=1000000): comps=0  swaps=0  duration=0
  FAIL  --> BubbleSort_v2 (size=1000000): comps=0  swaps=0  duration=0
  FAIL  --> InsertionSort_v1 (size=1000000): comps=0  swaps=0  duration=0
  FAIL  --> InsertionSort_v2 (size=1000000): comps=0  swaps=0  duration=0
  FAIL  --> InsertionSort_v3 (size=1000000): comps=0  swaps=0  duration=0
  OK  --> ShellSort_v1 (size=1000000): comps=68027835  swaps=50530714  duration=162041941
  OK  --> ShellSort_v2 (size=1000000): comps=62283286  swaps=44889440  duration=174902336
  OK  --> ShellSort_v3 (size=1000000): comps=33226631  swaps=15880632  duration=145896608
  FAIL  --> SelectionSort_v1 (size=1000000): comps=0  swaps=0  duration=0
  OK  --> HeapSort_v1 (size=1000000): comps=36792142  swaps=19046520  duration=212510335
   первые и последние 10 элементов массива из 10000000 элементов: 
   3014676 6210057 6922993 3173860 8803028 1921990 3755739 4739250 7609328 4600455 ... 3014676 6210057 6922993 3173860 8803028 1921990 3755739 4739250 7609328 4600455
  FAIL  --> BubbleSort_v1 (size=10000000): comps=0  swaps=0  duration=0
  FAIL  --> BubbleSort_v2 (size=10000000): comps=0  swaps=0  duration=0
  FAIL  --> InsertionSort_v1 (size=10000000): comps=0  swaps=0  duration=0
  FAIL  --> InsertionSort_v2 (size=10000000): comps=0  swaps=0  duration=0
  FAIL  --> InsertionSort_v3 (size=10000000): comps=0  swaps=0  duration=0
  OK  --> ShellSort_v1 (size=10000000): comps=1041605900  swaps=826620860  duration=2227106864
  OK  --> ShellSort_v2 (size=10000000): comps=1040073945  swaps=832098474  duration=2748544272
  OK  --> ShellSort_v3 (size=10000000): comps=398594255  swaps=191906810  duration=1759895075
  FAIL  --> SelectionSort_v1 (size=10000000): comps=0  swaps=0  duration=0
  OK  --> HeapSort_v1 (size=10000000): comps=434640382  swaps=223831712  duration=5382069045

Запускаем тест группы: "1.digits"
   исходный массив из 1 элементов: 
   0
  OK  --> BubbleSort_v1 (size=1): comps=0  swaps=0  duration=213
  OK  --> BubbleSort_v2 (size=1): comps=0  swaps=0  duration=56
  OK  --> InsertionSort_v1 (size=1): comps=0  swaps=0  duration=119
  OK  --> InsertionSort_v2 (size=1): comps=0  swaps=0  duration=53
  OK  --> InsertionSort_v3 (size=1): comps=0  swaps=0  duration=61
  OK  --> ShellSort_v1 (size=1): comps=0  swaps=0  duration=110
  OK  --> ShellSort_v2 (size=1): comps=0  swaps=0  duration=50
  OK  --> ShellSort_v3 (size=1): comps=0  swaps=0  duration=149
  OK  --> SelectionSort_v1 (size=1): comps=0  swaps=0  duration=118
  OK  --> HeapSort_v1 (size=1): comps=0  swaps=0  duration=113
   первые и последние 5 элементов массива из 10 элементов: 
   6 1 7 3 2 ... 6 1 7 3 2
  OK  --> BubbleSort_v1 (size=10): comps=45  swaps=21  duration=239
  OK  --> BubbleSort_v2 (size=10): comps=49  swaps=21  duration=226
  OK  --> InsertionSort_v1 (size=10): comps=28  swaps=21  duration=172
  OK  --> InsertionSort_v2 (size=10): comps=28  swaps=21  duration=146
  OK  --> InsertionSort_v3 (size=10): comps=51  swaps=21  duration=309
  OK  --> ShellSort_v1 (size=10): comps=32  swaps=15  duration=281
  OK  --> ShellSort_v2 (size=10): comps=28  swaps=15  duration=270
  OK  --> ShellSort_v3 (size=10): comps=29  swaps=19  duration=475
  OK  --> SelectionSort_v1 (size=10): comps=45  swaps=7  duration=172
  OK  --> HeapSort_v1 (size=10): comps=37  swaps=27  duration=259
   первые и последние 10 элементов массива из 100 элементов: 
   5 2 0 7 9 7 8 7 9 5 ... 5 2 0 7 9 7 8 7 9 5
  OK  --> BubbleSort_v1 (size=100): comps=4950  swaps=2401  duration=9434
  OK  --> BubbleSort_v2 (size=100): comps=4959  swaps=2401  duration=9758
  OK  --> InsertionSort_v1 (size=100): comps=2498  swaps=2401  duration=3889
  OK  --> InsertionSort_v2 (size=100): comps=2498  swaps=2401  duration=3359
  OK  --> InsertionSort_v3 (size=100): comps=1161  swaps=2401  duration=4612
  OK  --> ShellSort_v1 (size=100): comps=691  swaps=228  duration=2344
  OK  --> ShellSort_v2 (size=100): comps=644  swaps=208  duration=2360
  OK  --> ShellSort_v3 (size=100): comps=623  swaps=219  duration=3106
  OK  --> SelectionSort_v1 (size=100): comps=4950  swaps=88  duration=10005
  OK  --> HeapSort_v1 (size=100): comps=980  swaps=524  duration=3577
   первые и последние 10 элементов массива из 1000 элементов: 
   2 9 8 9 4 8 6 8 5 5 ... 2 9 8 9 4 8 6 8 5 5
  OK  --> BubbleSort_v1 (size=1000): comps=499500  swaps=228217  duration=557836
  OK  --> BubbleSort_v2 (size=1000): comps=495450  swaps=228217  duration=526668
  OK  --> InsertionSort_v1 (size=1000): comps=229214  swaps=228217  duration=302506
  OK  --> InsertionSort_v2 (size=1000): comps=229214  swaps=228217  duration=311104
  OK  --> InsertionSort_v3 (size=1000): comps=18261  swaps=228217  duration=163218
  OK  --> ShellSort_v1 (size=1000): comps=10449  swaps=2863  duration=27882
  OK  --> ShellSort_v2 (size=1000): comps=9991  swaps=2458  duration=28349
  OK  --> ShellSort_v3 (size=1000): comps=9726  swaps=2226  duration=35361
  OK  --> SelectionSort_v1 (size=1000): comps=499500  swaps=894  duration=1015849
  OK  --> HeapSort_v1 (size=1000): comps=15801  swaps=8271  duration=43167
   первые и последние 10 элементов массива из 10000 элементов: 
   1 0 8 4 8 9 9 4 4 7 ... 1 0 8 4 8 9 9 4 4 7
  OK  --> BubbleSort_v1 (size=10000): comps=49995000  swaps=22471502  duration=104458545
  OK  --> BubbleSort_v2 (size=10000): comps=49500490  swaps=22471502  duration=104641259
  OK  --> InsertionSort_v1 (size=10000): comps=22481500  swaps=22471502  duration=30581334
  OK  --> InsertionSort_v2 (size=10000): comps=22481500  swaps=22471502  duration=32645975
  OK  --> InsertionSort_v3 (size=10000): comps=248871  swaps=22471502  duration=12975192
  OK  --> ShellSort_v1 (size=10000): comps=153568  swaps=37573  duration=403397
  OK  --> ShellSort_v2 (size=10000): comps=135590  swaps=25870  duration=367350
  OK  --> ShellSort_v3 (size=10000): comps=129952  swaps=21427  duration=422644
  OK  --> SelectionSort_v1 (size=10000): comps=49995000  swaps=8999  duration=106541951
  OK  --> HeapSort_v1 (size=10000): comps=217737  swaps=112694  duration=516162
   первые и последние 10 элементов массива из 100000 элементов: 
   2 7 8 2 4 7 2 0 3 4 ... 2 7 8 2 4 7 2 0 3 4
  OK  --> BubbleSort_v1 (size=100000): comps=4999950000  swaps=2246197432  duration=12432226907
  OK  --> BubbleSort_v2 (size=100000): comps=4949954955  swaps=2246197432  duration=13026502004
  OK  --> InsertionSort_v1 (size=100000): comps=2246297430  swaps=2246197432  duration=2994490152
  OK  --> InsertionSort_v2 (size=100000): comps=2246297430  swaps=2246197432  duration=3203873230
  OK  --> InsertionSort_v3 (size=100000): comps=3154353  swaps=2246197432  duration=1319295025
  OK  --> ShellSort_v1 (size=100000): comps=1860949  swaps=401432  duration=3925133
  OK  --> ShellSort_v2 (size=100000): comps=1697481  swaps=267916  duration=3963876
  OK  --> ShellSort_v3 (size=100000): comps=1635712  swaps=211740  duration=4858336
  OK  --> SelectionSort_v1 (size=100000): comps=4999950000  swaps=90000  duration=10449585917
  OK  --> HeapSort_v1 (size=100000): comps=2766452  swaps=1420513  duration=6143899
   первые и последние 10 элементов массива из 1000000 элементов: 
   5 5 4 8 9 1 7 4 8 2 ... 5 5 4 8 9 1 7 4 8 2
  FAIL  --> BubbleSort_v1 (size=1000000): comps=0  swaps=0  duration=0
  FAIL  --> BubbleSort_v2 (size=1000000): comps=0  swaps=0  duration=0
  FAIL  --> InsertionSort_v1 (size=1000000): comps=0  swaps=0  duration=0
  FAIL  --> InsertionSort_v2 (size=1000000): comps=0  swaps=0  duration=0
  FAIL  --> InsertionSort_v3 (size=1000000): comps=0  swaps=0  duration=0
  OK  --> ShellSort_v1 (size=1000000): comps=22383596  swaps=4789821  duration=55372859
  OK  --> ShellSort_v2 (size=1000000): comps=20307786  swaps=2781582  duration=56674090
  OK  --> ShellSort_v3 (size=1000000): comps=19600952  swaps=2127650  duration=62868983
  FAIL  --> SelectionSort_v1 (size=1000000): comps=0  swaps=0  duration=0
  OK  --> HeapSort_v1 (size=1000000): comps=33483726  swaps=17110562  duration=75832312
   первые и последние 10 элементов массива из 10000000 элементов: 
   3 4 5 5 4 7 4 9 5 3 ... 3 4 5 5 4 7 4 9 5 3
  FAIL  --> BubbleSort_v1 (size=10000000): comps=0  swaps=0  duration=0
  FAIL  --> BubbleSort_v2 (size=10000000): comps=0  swaps=0  duration=0
  FAIL  --> InsertionSort_v1 (size=10000000): comps=0  swaps=0  duration=0
  FAIL  --> InsertionSort_v2 (size=10000000): comps=0  swaps=0  duration=0
  FAIL  --> InsertionSort_v3 (size=10000000): comps=0  swaps=0  duration=0
  OK  --> ShellSort_v1 (size=10000000): comps=270196822  swaps=54253917  duration=641826916
  OK  --> ShellSort_v2 (size=10000000): comps=237536810  swaps=28239358  duration=572638112
  OK  --> ShellSort_v3 (size=10000000): comps=229224325  swaps=21272122  duration=667234215
  FAIL  --> SelectionSort_v1 (size=10000000): comps=0  swaps=0  duration=0
  OK  --> HeapSort_v1 (size=10000000): comps=394975504  swaps=201210998  duration=831294815

Запускаем тест группы: "2.sorted"
   исходный массив из 1 элементов: 
   0
  OK  --> BubbleSort_v1 (size=1): comps=0  swaps=0  duration=206
  OK  --> BubbleSort_v2 (size=1): comps=0  swaps=0  duration=59
  OK  --> InsertionSort_v1 (size=1): comps=0  swaps=0  duration=126
  OK  --> InsertionSort_v2 (size=1): comps=0  swaps=0  duration=120
  OK  --> InsertionSort_v3 (size=1): comps=0  swaps=0  duration=57
  OK  --> ShellSort_v1 (size=1): comps=0  swaps=0  duration=123
  OK  --> ShellSort_v2 (size=1): comps=0  swaps=0  duration=254
  OK  --> ShellSort_v3 (size=1): comps=0  swaps=0  duration=269
  OK  --> SelectionSort_v1 (size=1): comps=0  swaps=0  duration=218
  OK  --> HeapSort_v1 (size=1): comps=0  swaps=0  duration=234
   первые и последние 5 элементов массива из 10 элементов: 
   0 6 2 3 4 ... 0 6 2 3 4
  OK  --> BubbleSort_v1 (size=10): comps=45  swaps=9  duration=258
  OK  --> BubbleSort_v2 (size=10): comps=45  swaps=9  duration=172
  OK  --> InsertionSort_v1 (size=10): comps=18  swaps=9  duration=142
  OK  --> InsertionSort_v2 (size=10): comps=18  swaps=9  duration=118
  OK  --> InsertionSort_v3 (size=10): comps=59  swaps=9  duration=245
  OK  --> ShellSort_v1 (size=10): comps=22  swaps=1  duration=150
  OK  --> ShellSort_v2 (size=10): comps=27  swaps=9  duration=239
  OK  --> ShellSort_v3 (size=10): comps=14  swaps=1  duration=369
  OK  --> SelectionSort_v1 (size=10): comps=45  swaps=1  duration=154
  OK  --> HeapSort_v1 (size=10): comps=41  swaps=29  duration=361
   первые и последние 10 элементов массива из 100 элементов: 
   0 1 2 3 4 5 6 7 8 9 ... 0 1 2 3 4 5 6 7 8 9
  OK  --> BubbleSort_v1 (size=100): comps=4950  swaps=87  duration=4020
  OK  --> BubbleSort_v2 (size=100): comps=3510  swaps=87  duration=2647
  OK  --> InsertionSort_v1 (size=100): comps=186  swaps=87  duration=335
  OK  --> InsertionSort_v2 (size=100): comps=186  swaps=87  duration=330
  OK  --> InsertionSort_v3 (size=100): comps=1245  swaps=87  duration=910
  OK  --> ShellSort_v1 (size=100): comps=590  swaps=87  duration=1075
  OK  --> ShellSort_v2 (size=100): comps=511  swaps=31  duration=998
  OK  --> ShellSort_v3 (size=100): comps=517  swaps=71  duration=1867
  OK  --> SelectionSort_v1 (size=100): comps=4950  swaps=1  duration=7800
  OK  --> HeapSort_v1 (size=100): comps=1081  swaps=645  duration=2823
   первые и последние 10 элементов массива из 1000 элементов: 
   0 1 2 3 4 5 6 7 8 9 ... 0 1 2 3 4 5 6 7 8 9
  OK  --> BubbleSort_v1 (size=1000): comps=499500  swaps=6604  duration=291130
  OK  --> BubbleSort_v2 (size=1000): comps=496930  swaps=6604  duration=296650
  OK  --> InsertionSort_v1 (size=1000): comps=7603  swaps=6604  duration=9592
  OK  --> InsertionSort_v2 (size=1000): comps=7603  swaps=6604  duration=9732
  OK  --> InsertionSort_v3 (size=1000): comps=18375  swaps=6604  duration=11726
  OK  --> ShellSort_v1 (size=1000): comps=10710  swaps=2722  duration=23647
  OK  --> ShellSort_v2 (size=1000): comps=10413  swaps=2446  duration=26104
  OK  --> ShellSort_v3 (size=1000): comps=10357  swaps=2450  duration=31542
  OK  --> SelectionSort_v1 (size=1000): comps=499500  swaps=10  duration=1003868
  OK  --> HeapSort_v1 (size=1000): comps=17560  swaps=9680  duration=34535
   первые и последние 10 элементов массива из 10000 элементов: 
   0 1 2 3 4 5 6 7 8 9 ... 0 1 2 3 4 5 6 7 8 9
  OK  --> BubbleSort_v1 (size=10000): comps=49995000  swaps=666392  duration=30831506
  OK  --> BubbleSort_v2 (size=10000): comps=49737454  swaps=666392  duration=31865884
  OK  --> InsertionSort_v1 (size=10000): comps=676391  swaps=666392  duration=1087327
  OK  --> InsertionSort_v2 (size=10000): comps=676391  swaps=666392  duration=998011
  OK  --> InsertionSort_v3 (size=10000): comps=248665  swaps=666392  duration=566860
  OK  --> ShellSort_v1 (size=10000): comps=176309  swaps=56474  duration=449243
  OK  --> ShellSort_v2 (size=10000): comps=171387  swaps=57928  duration=536639
  OK  --> ShellSort_v3 (size=10000): comps=163917  swaps=51640  duration=590377
  OK  --> SelectionSort_v1 (size=10000): comps=49995000  swaps=100  duration=107611638
  OK  --> HeapSort_v1 (size=10000): comps=244307  swaps=131480  duration=442515
   первые и последние 10 элементов массива из 100000 элементов: 
   0 1 2 3 4 5 6 7 8 9 ... 0 1 2 3 4 5 6 7 8 9
  OK  --> BubbleSort_v1 (size=100000): comps=4999950000  swaps=65248998  duration=3014106601
  OK  --> BubbleSort_v2 (size=100000): comps=4983441034  swaps=65248998  duration=3000511256
  OK  --> InsertionSort_v1 (size=100000): comps=65348997  swaps=65248998  duration=90727924
  OK  --> InsertionSort_v2 (size=100000): comps=65348997  swaps=65248998  duration=93323818
  OK  --> InsertionSort_v3 (size=100000): comps=3146427  swaps=65248998  duration=43590103
  OK  --> ShellSort_v1 (size=100000): comps=2833579  swaps=1334970  duration=7249049
  OK  --> ShellSort_v2 (size=100000): comps=2509526  swaps=1042150  duration=8275435
  OK  --> ShellSort_v3 (size=100000): comps=2258426  swaps=796742  duration=8783847
  OK  --> SelectionSort_v1 (size=100000): comps=4999950000  swaps=1000  duration=10461704090
  OK  --> HeapSort_v1 (size=100000): comps=3110584  swaps=1648620  duration=5501612
   первые и последние 10 элементов массива из 1000000 элементов: 
   0 1 2 3 4 5 6 7 8 9 ... 0 1 2 3 4 5 6 7 8 9
  FAIL  --> BubbleSort_v1 (size=1000000): comps=0  swaps=0  duration=0
  FAIL  --> BubbleSort_v2 (size=1000000): comps=0  swaps=0  duration=0
  FAIL  --> InsertionSort_v1 (size=1000000): comps=0  swaps=0  duration=0
  FAIL  --> InsertionSort_v2 (size=1000000): comps=0  swaps=0  duration=0
  FAIL  --> InsertionSort_v3 (size=1000000): comps=0  swaps=0  duration=0
  OK  --> ShellSort_v1 (size=1000000): comps=38939516  swaps=20954426  duration=99424435
  OK  --> ShellSort_v2 (size=1000000): comps=37313903  swaps=19378884  duration=118996375
  OK  --> ShellSort_v3 (size=1000000): comps=28889906  swaps=11038830  duration=112291950
  FAIL  --> SelectionSort_v1 (size=1000000): comps=0  swaps=0  duration=0
  OK  --> HeapSort_v1 (size=1000000): comps=37692154  swaps=19765196  duration=78920126
   первые и последние 10 элементов массива из 10000000 элементов: 
   0 1 2 3 4 5 6 7 8 9 ... 0 1 2 3 4 5 6 7 8 9
  FAIL  --> BubbleSort_v1 (size=10000000): comps=0  swaps=0  duration=0
  FAIL  --> BubbleSort_v2 (size=10000000): comps=0  swaps=0  duration=0
  FAIL  --> InsertionSort_v1 (size=10000000): comps=0  swaps=0  duration=0
  FAIL  --> InsertionSort_v2 (size=10000000): comps=0  swaps=0  duration=0
  FAIL  --> InsertionSort_v3 (size=10000000): comps=0  swaps=0  duration=0
  OK  --> ShellSort_v1 (size=10000000): comps=624139826  swaps=404287028  duration=1452808502
  OK  --> ShellSort_v2 (size=10000000): comps=580178096  swaps=367110546  duration=1671987266
  OK  --> ShellSort_v3 (size=10000000): comps=353352946  swaps=141665472  duration=1421431325
  FAIL  --> SelectionSort_v1 (size=10000000): comps=0  swaps=0  duration=0
  OK  --> HeapSort_v1 (size=10000000): comps=444849813  swaps=231605924  duration=1003908419

Запускаем тест группы: "3.revers"
   исходный массив из 1 элементов: 
   0
  OK  --> BubbleSort_v1 (size=1): comps=0  swaps=0  duration=219
  OK  --> BubbleSort_v2 (size=1): comps=0  swaps=0  duration=61
  OK  --> InsertionSort_v1 (size=1): comps=0  swaps=0  duration=110
  OK  --> InsertionSort_v2 (size=1): comps=0  swaps=0  duration=53
  OK  --> InsertionSort_v3 (size=1): comps=0  swaps=0  duration=59
  OK  --> ShellSort_v1 (size=1): comps=0  swaps=0  duration=307
  OK  --> ShellSort_v2 (size=1): comps=0  swaps=0  duration=52
  OK  --> ShellSort_v3 (size=1): comps=0  swaps=0  duration=160
  OK  --> SelectionSort_v1 (size=1): comps=0  swaps=0  duration=216
  OK  --> HeapSort_v1 (size=1): comps=0  swaps=0  duration=348
   первые и последние 5 элементов массива из 10 элементов: 
   9 8 7 6 5 ... 9 8 7 6 5
  OK  --> BubbleSort_v1 (size=10): comps=45  swaps=45  duration=174
  OK  --> BubbleSort_v2 (size=10): comps=54  swaps=45  duration=166
  OK  --> InsertionSort_v1 (size=10): comps=45  swaps=45  duration=211
  OK  --> InsertionSort_v2 (size=10): comps=45  swaps=45  duration=161
  OK  --> InsertionSort_v3 (size=10): comps=47  swaps=45  duration=246
  OK  --> ShellSort_v1 (size=10): comps=27  swaps=13  duration=227
  OK  --> ShellSort_v2 (size=10): comps=25  swaps=11  duration=221
  OK  --> ShellSort_v3 (size=10): comps=30  swaps=25  duration=590
  OK  --> SelectionSort_v1 (size=10): comps=45  swaps=5  duration=199
  OK  --> HeapSort_v1 (size=10): comps=35  swaps=21  duration=265
   первые и последние 10 элементов массива из 100 элементов: 
   99 98 97 96 95 94 93 92 91 90 ... 99 98 97 96 95 94 93 92 91 90
  OK  --> BubbleSort_v1 (size=100): comps=4950  swaps=4950  duration=6722
  OK  --> BubbleSort_v2 (size=100): comps=5049  swaps=4950  duration=6741
  OK  --> InsertionSort_v1 (size=100): comps=4950  swaps=4950  duration=6989
  OK  --> InsertionSort_v2 (size=100): comps=4950  swaps=4950  duration=6620
  OK  --> InsertionSort_v3 (size=100): comps=1059  swaps=4950  duration=3238
  OK  --> ShellSort_v1 (size=100): comps=668  swaps=260  duration=1302
  OK  --> ShellSort_v2 (size=100): comps=614  swaps=192  duration=1557
  OK  --> ShellSort_v3 (size=100): comps=626  swaps=234  duration=2120
  OK  --> SelectionSort_v1 (size=100): comps=4950  swaps=50  duration=8199
  OK  --> HeapSort_v1 (size=100): comps=944  swaps=516  duration=2788
   первые и последние 10 элементов массива из 1000 элементов: 
   999 998 997 996 995 994 993 992 991 990 ... 999 998 997 996 995 994 993 992 991 990
  OK  --> BubbleSort_v1 (size=1000): comps=499500  swaps=499500  duration=689571
  OK  --> BubbleSort_v2 (size=1000): comps=500499  swaps=499500  duration=690508
  OK  --> InsertionSort_v1 (size=1000): comps=499500  swaps=499500  duration=678026
  OK  --> InsertionSort_v2 (size=1000): comps=499500  swaps=499500  duration=705237
  OK  --> InsertionSort_v3 (size=1000): comps=16973  swaps=499500  duration=274590
  OK  --> ShellSort_v1 (size=1000): comps=11716  swaps=4700  duration=14082
  OK  --> ShellSort_v2 (size=1000): comps=10511  swaps=3424  duration=16010
  OK  --> ShellSort_v3 (size=1000): comps=10788  swaps=3690  duration=21715
  OK  --> SelectionSort_v1 (size=1000): comps=499500  swaps=500  duration=1004354
  OK  --> HeapSort_v1 (size=1000): comps=15965  swaps=8316  duration=33875
   первые и последние 10 элементов массива из 10000 элементов: 
   9999 9998 9997 9996 9995 9994 9993 9992 9991 9990 ... 9999 9998 9997 9996 9995 9994 9993 9992 9991 9990
  OK  --> BubbleSort_v1 (size=10000): comps=49995000  swaps=49995000  duration=73351288
  OK  --> BubbleSort_v2 (size=10000): comps=50004999  swaps=49995000  duration=72143733
  OK  --> InsertionSort_v1 (size=10000): comps=49995000  swaps=49995000  duration=68236148
  OK  --> InsertionSort_v2 (size=10000): comps=49995000  swaps=49995000  duration=72555825
  OK  --> InsertionSort_v3 (size=10000): comps=237261  swaps=49995000  duration=28016863
  OK  --> ShellSort_v1 (size=10000): comps=172578  swaps=62560  duration=178330
  OK  --> ShellSort_v2 (size=10000): comps=144824  swaps=36778  duration=200536
  OK  --> ShellSort_v3 (size=10000): comps=148103  swaps=41884  duration=285587
  OK  --> SelectionSort_v1 (size=10000): comps=49995000  swaps=5000  duration=108021604
  OK  --> HeapSort_v1 (size=10000): comps=226682  swaps=116696  duration=434659
   первые и последние 10 элементов массива из 100000 элементов: 
   99999 99998 99997 99996 99995 99994 99993 99992 99991 99990 ... 99999 99998 99997 99996 99995 99994 99993 99992 99991 99990
  OK  --> BubbleSort_v1 (size=100000): comps=4999950000  swaps=4999950000  duration=7066714589
  OK  --> BubbleSort_v2 (size=100000): comps=5000049999  swaps=4999950000  duration=7038011229
  OK  --> InsertionSort_v1 (size=100000): comps=4999950000  swaps=4999950000  duration=6626347381
  OK  --> InsertionSort_v2 (size=100000): comps=4999950000  swaps=4999950000  duration=7101158890
  OK  --> InsertionSort_v3 (size=100000): comps=3037891  swaps=4999950000  duration=2912006053
  OK  --> ShellSort_v1 (size=100000): comps=2244585  swaps=844560  duration=2329878
  OK  --> ShellSort_v2 (size=100000): comps=1865861  swaps=458016  duration=2464854
  OK  --> ShellSort_v3 (size=100000): comps=1941773  swaps=542104  duration=3657667
  OK  --> SelectionSort_v1 (size=100000): comps=4999950000  swaps=50000  duration=10466935006
  OK  --> HeapSort_v1 (size=100000): comps=2926640  swaps=1497434  duration=5384677
   первые и последние 10 элементов массива из 1000000 элементов: 
   999999 999998 999997 999996 999995 999994 999993 999992 999991 999990 ... 999999 999998 999997 999996 999995 999994 999993 999992 999991 999990
  FAIL  --> BubbleSort_v1 (size=1000000): comps=0  swaps=0  duration=0
  FAIL  --> BubbleSort_v2 (size=1000000): comps=0  swaps=0  duration=0
  FAIL  --> InsertionSort_v1 (size=1000000): comps=0  swaps=0  duration=0
  FAIL  --> InsertionSort_v2 (size=1000000): comps=0  swaps=0  duration=0
  FAIL  --> InsertionSort_v3 (size=1000000): comps=0  swaps=0  duration=0
  OK  --> ShellSort_v1 (size=1000000): comps=26357530  swaps=9357504  duration=36796162
  OK  --> ShellSort_v2 (size=1000000): comps=23369384  swaps=6236456  duration=41516536
  OK  --> ShellSort_v3 (size=1000000): comps=22864392  swaps=5662150  duration=51944608
  FAIL  --> SelectionSort_v1 (size=1000000): comps=0  swaps=0  duration=0
  OK  --> HeapSort_v1 (size=1000000): comps=36001436  swaps=18333408  duration=69568181
   первые и последние 10 элементов массива из 10000000 элементов: 
   9999999 9999998 9999997 9999996 9999995 9999994 9999993 9999992 9999991 9999990 ... 9999999 9999998 9999997 9999996 9999995 9999994 9999993 9999992 9999991 9999990
  FAIL  --> BubbleSort_v1 (size=10000000): comps=0  swaps=0  duration=0
  FAIL  --> BubbleSort_v2 (size=10000000): comps=0  swaps=0  duration=0
  FAIL  --> InsertionSort_v1 (size=10000000): comps=0  swaps=0  duration=0
  FAIL  --> InsertionSort_v2 (size=10000000): comps=0  swaps=0  duration=0
  FAIL  --> InsertionSort_v3 (size=10000000): comps=0  swaps=0  duration=0
  OK  --> ShellSort_v1 (size=10000000): comps=317626219  swaps=107626176  duration=391278446
  OK  --> ShellSort_v2 (size=10000000): comps=270522361  swaps=62945972  duration=393638790
  OK  --> ShellSort_v3 (size=10000000): comps=270520217  swaps=65109280  duration=541821780
  FAIL  --> SelectionSort_v1 (size=10000000): comps=0  swaps=0  duration=0
  OK  --> HeapSort_v1 (size=10000000): comps=426919609  swaps=216912428  duration=849919912

----------- [Group name: "0.random"] -----------
Name                    Test #      Processed     Array size    Comparisons          Swaps       Duration (ns)
BubbleSort_v1           Test 0      processed              1              0              0                 422
BubbleSort_v2           Test 0      processed              1              0              0                 147
InsertionSort_v1        Test 0      processed              1              0              0                 143
InsertionSort_v2        Test 0      processed              1              0              0                  98
InsertionSort_v3        Test 0      processed              1              0              0                 100
ShellSort_v1            Test 0      processed              1              0              0               47630
ShellSort_v2            Test 0      processed              1              0              0                 132
ShellSort_v3            Test 0      processed              1              0              0                 389
SelectionSort_v1        Test 0      processed              1              0              0                 203
HeapSort_v1             Test 0      processed              1              0              0                 150

BubbleSort_v1           Test 1      processed             10             45             16               14230
BubbleSort_v2           Test 1      processed             10             52             16                 414
InsertionSort_v1        Test 1      processed             10             24             16                 332
InsertionSort_v2        Test 1      processed             10             24             16                 284
InsertionSort_v3        Test 1      processed             10             55             16                 476
ShellSort_v1            Test 1      processed             10             29             10                 378
ShellSort_v2            Test 1      processed             10             25             10                 381
ShellSort_v3            Test 1      processed             10             23             12                1015
SelectionSort_v1        Test 1      processed             10             45              8                 298
HeapSort_v1             Test 1      processed             10             39             28                 573

BubbleSort_v1           Test 2      processed            100           4950           2157               15699
BubbleSort_v2           Test 2      processed            100           4819           2157               15967
InsertionSort_v1        Test 2      processed            100           2253           2157                5814
InsertionSort_v2        Test 2      processed            100           2253           2157                4846
InsertionSort_v3        Test 2      processed            100           1153           2157                7236
ShellSort_v1            Test 2      processed            100            877            413                4944
ShellSort_v2            Test 2      processed            100            834            403                5638
ShellSort_v3            Test 2      processed            100            734            331                6576
SelectionSort_v1        Test 2      processed            100           4950             95               16056
HeapSort_v1             Test 2      processed            100           1034            591                5461

BubbleSort_v1           Test 3      processed           1000         499500         247489             1198747
BubbleSort_v2           Test 3      processed           1000         499372         247489             1153676
InsertionSort_v1        Test 3      processed           1000         248479         247489              582076
InsertionSort_v2        Test 3      processed           1000         248479         247489              539491
InsertionSort_v3        Test 3      processed           1000          18169         247489              285476
ShellSort_v1            Test 3      processed           1000          15296           7815               94597
ShellSort_v2            Test 3      processed           1000          14379           6975               99862
ShellSort_v3            Test 3      processed           1000          14113           6741              108039
SelectionSort_v1        Test 3      processed           1000         499500            993             1633325
HeapSort_v1             Test 3      processed           1000          16850           9065               61104

BubbleSort_v1           Test 4      processed          10000       49995000       25288624           118158672
BubbleSort_v2           Test 4      processed          10000       49997497       25288624           114729473
InsertionSort_v1        Test 4      processed          10000       25298615       25288624            36071861
InsertionSort_v2        Test 4      processed          10000       25298615       25288624            37514191
InsertionSort_v3        Test 4      processed          10000         247903       25288624            15042550
ShellSort_v1            Test 4      processed          10000         267306         152486              796659
ShellSort_v2            Test 4      processed          10000         255750         147412              877648
ShellSort_v3            Test 4      processed          10000         203477          96320              864434
SelectionSort_v1        Test 4      processed          10000       49995000           9992           106085481
HeapSort_v1             Test 4      processed          10000         235395         124222              509537

BubbleSort_v1           Test 5      processed         100000     4999950000     2501737763         12704342487
BubbleSort_v2           Test 5      processed         100000     5000040547     2501737763         13271785347
InsertionSort_v1        Test 5      processed         100000     2501837753     2501737763          3380192553
InsertionSort_v2        Test 5      processed         100000     2501837753     2501737763          3558060159
InsertionSort_v3        Test 5      processed         100000        3145309     2501737763          1458090821
ShellSort_v1            Test 5      processed         100000        4562610        3113279            11149029
ShellSort_v2            Test 5      processed         100000        3920114        2503799            12353501
ShellSort_v3            Test 5      processed         100000        2677097        1266093            11632168
SelectionSort_v1        Test 5      processed         100000     4999950000          99987         10445350104
HeapSort_v1             Test 5      processed         100000        3020053        1575003             6950214

BubbleSort_v1           Test 6        skipped        1000000              0              0                   0
BubbleSort_v2           Test 6        skipped        1000000              0              0                   0
InsertionSort_v1        Test 6        skipped        1000000              0              0                   0
InsertionSort_v2        Test 6        skipped        1000000              0              0                   0
InsertionSort_v3        Test 6        skipped        1000000              0              0                   0
ShellSort_v1            Test 6      processed        1000000       68027835       50530714           162041941
ShellSort_v2            Test 6      processed        1000000       62283286       44889440           174902336
ShellSort_v3            Test 6      processed        1000000       33226631       15880632           145896608
SelectionSort_v1        Test 6        skipped        1000000              0              0                   0
HeapSort_v1             Test 6      processed        1000000       36792142       19046520           212510335

BubbleSort_v1           Test 7        skipped       10000000              0              0                   0
BubbleSort_v2           Test 7        skipped       10000000              0              0                   0
InsertionSort_v1        Test 7        skipped       10000000              0              0                   0
InsertionSort_v2        Test 7        skipped       10000000              0              0                   0
InsertionSort_v3        Test 7        skipped       10000000              0              0                   0
ShellSort_v1            Test 7      processed       10000000     1041605900      826620860          2227106864
ShellSort_v2            Test 7      processed       10000000     1040073945      832098474          2748544272
ShellSort_v3            Test 7      processed       10000000      398594255      191906810          1759895075
SelectionSort_v1        Test 7        skipped       10000000              0              0                   0
HeapSort_v1             Test 7      processed       10000000      434640382      223831712          5382069045


----------- [Group name: "1.digits"] -----------
Name                    Test #      Processed     Array size    Comparisons          Swaps       Duration (ns)
BubbleSort_v1           Test 0      processed              1              0              0                 213
BubbleSort_v2           Test 0      processed              1              0              0                  56
InsertionSort_v1        Test 0      processed              1              0              0                 119
InsertionSort_v2        Test 0      processed              1              0              0                  53
InsertionSort_v3        Test 0      processed              1              0              0                  61
ShellSort_v1            Test 0      processed              1              0              0                 110
ShellSort_v2            Test 0      processed              1              0              0                  50
ShellSort_v3            Test 0      processed              1              0              0                 149
SelectionSort_v1        Test 0      processed              1              0              0                 118
HeapSort_v1             Test 0      processed              1              0              0                 113

BubbleSort_v1           Test 1      processed             10             45             21                 239
BubbleSort_v2           Test 1      processed             10             49             21                 226
InsertionSort_v1        Test 1      processed             10             28             21                 172
InsertionSort_v2        Test 1      processed             10             28             21                 146
InsertionSort_v3        Test 1      processed             10             51             21                 309
ShellSort_v1            Test 1      processed             10             32             15                 281
ShellSort_v2            Test 1      processed             10             28             15                 270
ShellSort_v3            Test 1      processed             10             29             19                 475
SelectionSort_v1        Test 1      processed             10             45              7                 172
HeapSort_v1             Test 1      processed             10             37             27                 259

BubbleSort_v1           Test 2      processed            100           4950           2401                9434
BubbleSort_v2           Test 2      processed            100           4959           2401                9758
InsertionSort_v1        Test 2      processed            100           2498           2401                3889
InsertionSort_v2        Test 2      processed            100           2498           2401                3359
InsertionSort_v3        Test 2      processed            100           1161           2401                4612
ShellSort_v1            Test 2      processed            100            691            228                2344
ShellSort_v2            Test 2      processed            100            644            208                2360
ShellSort_v3            Test 2      processed            100            623            219                3106
SelectionSort_v1        Test 2      processed            100           4950             88               10005
HeapSort_v1             Test 2      processed            100            980            524                3577

BubbleSort_v1           Test 3      processed           1000         499500         228217              557836
BubbleSort_v2           Test 3      processed           1000         495450         228217              526668
InsertionSort_v1        Test 3      processed           1000         229214         228217              302506
InsertionSort_v2        Test 3      processed           1000         229214         228217              311104
InsertionSort_v3        Test 3      processed           1000          18261         228217              163218
ShellSort_v1            Test 3      processed           1000          10449           2863               27882
ShellSort_v2            Test 3      processed           1000           9991           2458               28349
ShellSort_v3            Test 3      processed           1000           9726           2226               35361
SelectionSort_v1        Test 3      processed           1000         499500            894             1015849
HeapSort_v1             Test 3      processed           1000          15801           8271               43167

BubbleSort_v1           Test 4      processed          10000       49995000       22471502           104458545
BubbleSort_v2           Test 4      processed          10000       49500490       22471502           104641259
InsertionSort_v1        Test 4      processed          10000       22481500       22471502            30581334
InsertionSort_v2        Test 4      processed          10000       22481500       22471502            32645975
InsertionSort_v3        Test 4      processed          10000         248871       22471502            12975192
ShellSort_v1            Test 4      processed          10000         153568          37573              403397
ShellSort_v2            Test 4      processed          10000         135590          25870              367350
ShellSort_v3            Test 4      processed          10000         129952          21427              422644
SelectionSort_v1        Test 4      processed          10000       49995000           8999           106541951
HeapSort_v1             Test 4      processed          10000         217737         112694              516162

BubbleSort_v1           Test 5      processed         100000     4999950000     2246197432         12432226907
BubbleSort_v2           Test 5      processed         100000     4949954955     2246197432         13026502004
InsertionSort_v1        Test 5      processed         100000     2246297430     2246197432          2994490152
InsertionSort_v2        Test 5      processed         100000     2246297430     2246197432          3203873230
InsertionSort_v3        Test 5      processed         100000        3154353     2246197432          1319295025
ShellSort_v1            Test 5      processed         100000        1860949         401432             3925133
ShellSort_v2            Test 5      processed         100000        1697481         267916             3963876
ShellSort_v3            Test 5      processed         100000        1635712         211740             4858336
SelectionSort_v1        Test 5      processed         100000     4999950000          90000         10449585917
HeapSort_v1             Test 5      processed         100000        2766452        1420513             6143899

BubbleSort_v1           Test 6        skipped        1000000              0              0                   0
BubbleSort_v2           Test 6        skipped        1000000              0              0                   0
InsertionSort_v1        Test 6        skipped        1000000              0              0                   0
InsertionSort_v2        Test 6        skipped        1000000              0              0                   0
InsertionSort_v3        Test 6        skipped        1000000              0              0                   0
ShellSort_v1            Test 6      processed        1000000       22383596        4789821            55372859
ShellSort_v2            Test 6      processed        1000000       20307786        2781582            56674090
ShellSort_v3            Test 6      processed        1000000       19600952        2127650            62868983
SelectionSort_v1        Test 6        skipped        1000000              0              0                   0
HeapSort_v1             Test 6      processed        1000000       33483726       17110562            75832312

BubbleSort_v1           Test 7        skipped       10000000              0              0                   0
BubbleSort_v2           Test 7        skipped       10000000              0              0                   0
InsertionSort_v1        Test 7        skipped       10000000              0              0                   0
InsertionSort_v2        Test 7        skipped       10000000              0              0                   0
InsertionSort_v3        Test 7        skipped       10000000              0              0                   0
ShellSort_v1            Test 7      processed       10000000      270196822       54253917           641826916
ShellSort_v2            Test 7      processed       10000000      237536810       28239358           572638112
ShellSort_v3            Test 7      processed       10000000      229224325       21272122           667234215
SelectionSort_v1        Test 7        skipped       10000000              0              0                   0
HeapSort_v1             Test 7      processed       10000000      394975504      201210998           831294815


----------- [Group name: "2.sorted"] -----------
Name                    Test #      Processed     Array size    Comparisons          Swaps       Duration (ns)
BubbleSort_v1           Test 0      processed              1              0              0                 206
BubbleSort_v2           Test 0      processed              1              0              0                  59
InsertionSort_v1        Test 0      processed              1              0              0                 126
InsertionSort_v2        Test 0      processed              1              0              0                 120
InsertionSort_v3        Test 0      processed              1              0              0                  57
ShellSort_v1            Test 0      processed              1              0              0                 123
ShellSort_v2            Test 0      processed              1              0              0                 254
ShellSort_v3            Test 0      processed              1              0              0                 269
SelectionSort_v1        Test 0      processed              1              0              0                 218
HeapSort_v1             Test 0      processed              1              0              0                 234

BubbleSort_v1           Test 1      processed             10             45              9                 258
BubbleSort_v2           Test 1      processed             10             45              9                 172
InsertionSort_v1        Test 1      processed             10             18              9                 142
InsertionSort_v2        Test 1      processed             10             18              9                 118
InsertionSort_v3        Test 1      processed             10             59              9                 245
ShellSort_v1            Test 1      processed             10             22              1                 150
ShellSort_v2            Test 1      processed             10             27              9                 239
ShellSort_v3            Test 1      processed             10             14              1                 369
SelectionSort_v1        Test 1      processed             10             45              1                 154
HeapSort_v1             Test 1      processed             10             41             29                 361

BubbleSort_v1           Test 2      processed            100           4950             87                4020
BubbleSort_v2           Test 2      processed            100           3510             87                2647
InsertionSort_v1        Test 2      processed            100            186             87                 335
InsertionSort_v2        Test 2      processed            100            186             87                 330
InsertionSort_v3        Test 2      processed            100           1245             87                 910
ShellSort_v1            Test 2      processed            100            590             87                1075
ShellSort_v2            Test 2      processed            100            511             31                 998
ShellSort_v3            Test 2      processed            100            517             71                1867
SelectionSort_v1        Test 2      processed            100           4950              1                7800
HeapSort_v1             Test 2      processed            100           1081            645                2823

BubbleSort_v1           Test 3      processed           1000         499500           6604              291130
BubbleSort_v2           Test 3      processed           1000         496930           6604              296650
InsertionSort_v1        Test 3      processed           1000           7603           6604                9592
InsertionSort_v2        Test 3      processed           1000           7603           6604                9732
InsertionSort_v3        Test 3      processed           1000          18375           6604               11726
ShellSort_v1            Test 3      processed           1000          10710           2722               23647
ShellSort_v2            Test 3      processed           1000          10413           2446               26104
ShellSort_v3            Test 3      processed           1000          10357           2450               31542
SelectionSort_v1        Test 3      processed           1000         499500             10             1003868
HeapSort_v1             Test 3      processed           1000          17560           9680               34535

BubbleSort_v1           Test 4      processed          10000       49995000         666392            30831506
BubbleSort_v2           Test 4      processed          10000       49737454         666392            31865884
InsertionSort_v1        Test 4      processed          10000         676391         666392             1087327
InsertionSort_v2        Test 4      processed          10000         676391         666392              998011
InsertionSort_v3        Test 4      processed          10000         248665         666392              566860
ShellSort_v1            Test 4      processed          10000         176309          56474              449243
ShellSort_v2            Test 4      processed          10000         171387          57928              536639
ShellSort_v3            Test 4      processed          10000         163917          51640              590377
SelectionSort_v1        Test 4      processed          10000       49995000            100           107611638
HeapSort_v1             Test 4      processed          10000         244307         131480              442515

BubbleSort_v1           Test 5      processed         100000     4999950000       65248998          3014106601
BubbleSort_v2           Test 5      processed         100000     4983441034       65248998          3000511256
InsertionSort_v1        Test 5      processed         100000       65348997       65248998            90727924
InsertionSort_v2        Test 5      processed         100000       65348997       65248998            93323818
InsertionSort_v3        Test 5      processed         100000        3146427       65248998            43590103
ShellSort_v1            Test 5      processed         100000        2833579        1334970             7249049
ShellSort_v2            Test 5      processed         100000        2509526        1042150             8275435
ShellSort_v3            Test 5      processed         100000        2258426         796742             8783847
SelectionSort_v1        Test 5      processed         100000     4999950000           1000         10461704090
HeapSort_v1             Test 5      processed         100000        3110584        1648620             5501612

BubbleSort_v1           Test 6        skipped        1000000              0              0                   0
BubbleSort_v2           Test 6        skipped        1000000              0              0                   0
InsertionSort_v1        Test 6        skipped        1000000              0              0                   0
InsertionSort_v2        Test 6        skipped        1000000              0              0                   0
InsertionSort_v3        Test 6        skipped        1000000              0              0                   0
ShellSort_v1            Test 6      processed        1000000       38939516       20954426            99424435
ShellSort_v2            Test 6      processed        1000000       37313903       19378884           118996375
ShellSort_v3            Test 6      processed        1000000       28889906       11038830           112291950
SelectionSort_v1        Test 6        skipped        1000000              0              0                   0
HeapSort_v1             Test 6      processed        1000000       37692154       19765196            78920126

BubbleSort_v1           Test 7        skipped       10000000              0              0                   0
BubbleSort_v2           Test 7        skipped       10000000              0              0                   0
InsertionSort_v1        Test 7        skipped       10000000              0              0                   0
InsertionSort_v2        Test 7        skipped       10000000              0              0                   0
InsertionSort_v3        Test 7        skipped       10000000              0              0                   0
ShellSort_v1            Test 7      processed       10000000      624139826      404287028          1452808502
ShellSort_v2            Test 7      processed       10000000      580178096      367110546          1671987266
ShellSort_v3            Test 7      processed       10000000      353352946      141665472          1421431325
SelectionSort_v1        Test 7        skipped       10000000              0              0                   0
HeapSort_v1             Test 7      processed       10000000      444849813      231605924          1003908419


----------- [Group name: "3.revers"] -----------
Name                    Test #      Processed     Array size    Comparisons          Swaps       Duration (ns)
BubbleSort_v1           Test 0      processed              1              0              0                 219
BubbleSort_v2           Test 0      processed              1              0              0                  61
InsertionSort_v1        Test 0      processed              1              0              0                 110
InsertionSort_v2        Test 0      processed              1              0              0                  53
InsertionSort_v3        Test 0      processed              1              0              0                  59
ShellSort_v1            Test 0      processed              1              0              0                 307
ShellSort_v2            Test 0      processed              1              0              0                  52
ShellSort_v3            Test 0      processed              1              0              0                 160
SelectionSort_v1        Test 0      processed              1              0              0                 216
HeapSort_v1             Test 0      processed              1              0              0                 348

BubbleSort_v1           Test 1      processed             10             45             45                 174
BubbleSort_v2           Test 1      processed             10             54             45                 166
InsertionSort_v1        Test 1      processed             10             45             45                 211
InsertionSort_v2        Test 1      processed             10             45             45                 161
InsertionSort_v3        Test 1      processed             10             47             45                 246
ShellSort_v1            Test 1      processed             10             27             13                 227
ShellSort_v2            Test 1      processed             10             25             11                 221
ShellSort_v3            Test 1      processed             10             30             25                 590
SelectionSort_v1        Test 1      processed             10             45              5                 199
HeapSort_v1             Test 1      processed             10             35             21                 265

BubbleSort_v1           Test 2      processed            100           4950           4950                6722
BubbleSort_v2           Test 2      processed            100           5049           4950                6741
InsertionSort_v1        Test 2      processed            100           4950           4950                6989
InsertionSort_v2        Test 2      processed            100           4950           4950                6620
InsertionSort_v3        Test 2      processed            100           1059           4950                3238
ShellSort_v1            Test 2      processed            100            668            260                1302
ShellSort_v2            Test 2      processed            100            614            192                1557
ShellSort_v3            Test 2      processed            100            626            234                2120
SelectionSort_v1        Test 2      processed            100           4950             50                8199
HeapSort_v1             Test 2      processed            100            944            516                2788

BubbleSort_v1           Test 3      processed           1000         499500         499500              689571
BubbleSort_v2           Test 3      processed           1000         500499         499500              690508
InsertionSort_v1        Test 3      processed           1000         499500         499500              678026
InsertionSort_v2        Test 3      processed           1000         499500         499500              705237
InsertionSort_v3        Test 3      processed           1000          16973         499500              274590
ShellSort_v1            Test 3      processed           1000          11716           4700               14082
ShellSort_v2            Test 3      processed           1000          10511           3424               16010
ShellSort_v3            Test 3      processed           1000          10788           3690               21715
SelectionSort_v1        Test 3      processed           1000         499500            500             1004354
HeapSort_v1             Test 3      processed           1000          15965           8316               33875

BubbleSort_v1           Test 4      processed          10000       49995000       49995000            73351288
BubbleSort_v2           Test 4      processed          10000       50004999       49995000            72143733
InsertionSort_v1        Test 4      processed          10000       49995000       49995000            68236148
InsertionSort_v2        Test 4      processed          10000       49995000       49995000            72555825
InsertionSort_v3        Test 4      processed          10000         237261       49995000            28016863
ShellSort_v1            Test 4      processed          10000         172578          62560              178330
ShellSort_v2            Test 4      processed          10000         144824          36778              200536
ShellSort_v3            Test 4      processed          10000         148103          41884              285587
SelectionSort_v1        Test 4      processed          10000       49995000           5000           108021604
HeapSort_v1             Test 4      processed          10000         226682         116696              434659

BubbleSort_v1           Test 5      processed         100000     4999950000     4999950000          7066714589
BubbleSort_v2           Test 5      processed         100000     5000049999     4999950000          7038011229
InsertionSort_v1        Test 5      processed         100000     4999950000     4999950000          6626347381
InsertionSort_v2        Test 5      processed         100000     4999950000     4999950000          7101158890
InsertionSort_v3        Test 5      processed         100000        3037891     4999950000          2912006053
ShellSort_v1            Test 5      processed         100000        2244585         844560             2329878
ShellSort_v2            Test 5      processed         100000        1865861         458016             2464854
ShellSort_v3            Test 5      processed         100000        1941773         542104             3657667
SelectionSort_v1        Test 5      processed         100000     4999950000          50000         10466935006
HeapSort_v1             Test 5      processed         100000        2926640        1497434             5384677

BubbleSort_v1           Test 6        skipped        1000000              0              0                   0
BubbleSort_v2           Test 6        skipped        1000000              0              0                   0
InsertionSort_v1        Test 6        skipped        1000000              0              0                   0
InsertionSort_v2        Test 6        skipped        1000000              0              0                   0
InsertionSort_v3        Test 6        skipped        1000000              0              0                   0
ShellSort_v1            Test 6      processed        1000000       26357530        9357504            36796162
ShellSort_v2            Test 6      processed        1000000       23369384        6236456            41516536
ShellSort_v3            Test 6      processed        1000000       22864392        5662150            51944608
SelectionSort_v1        Test 6        skipped        1000000              0              0                   0
HeapSort_v1             Test 6      processed        1000000       36001436       18333408            69568181

BubbleSort_v1           Test 7        skipped       10000000              0              0                   0
BubbleSort_v2           Test 7        skipped       10000000              0              0                   0
InsertionSort_v1        Test 7        skipped       10000000              0              0                   0
InsertionSort_v2        Test 7        skipped       10000000              0              0                   0
InsertionSort_v3        Test 7        skipped       10000000              0              0                   0
ShellSort_v1            Test 7      processed       10000000      317626219      107626176           391278446
ShellSort_v2            Test 7      processed       10000000      270522361       62945972           393638790
ShellSort_v3            Test 7      processed       10000000      270520217       65109280           541821780
SelectionSort_v1        Test 7        skipped       10000000              0              0                   0
HeapSort_v1             Test 7      processed       10000000      426919609      216912428           849919912
```

Можно наблюдать, что **SelectionSort** имеет квадратичную сложность даже на отсортированном массиве, зато имеет и минимальное количество обменов. А алгоритм **HeapSort** дает гарантированную O(N log N) сложность в лучшем, среднем и худшем случае.

При этом оба алгоритма достаточно однородны по времени при разных типах распределений. **SelectionSort** и  **HeapSort** показывают хоть и немного отличающееся время выполнения, но в очень незначительных пределах. При N=100000:

| | **SelectionSort** | **HeapSort** |
| -- | -- | -- |
| Random | `10445350104` ns | `6950214` ns |
| Digits | `10449585917` ns | `6143899` ns |
| Sorted | `10461704090` ns | `5501612` ns |
| Revers | `10466935006` ns | `5384677` ns |

##### Результаты теста с Random распределением

Алгоритм **HeapSort** начинает лучше показывать себя на больших N, в силу более сложной логики и необходимости поддерживать структуру кучи с прыжками по индексам. Но если по началу эта сложность дает некоторую просадку, в сравнении с тем же **InsertionSort**, то затем алгоритм начинает отрываться.

Интересно, что на N=10000000 (на которых отрабатываю только ShellSort и HeapSort), **ShellSort** показал себя лучше чем **HeapSort**. Впрочем и ShellSort_v2 и ShellSort_v1 имеют больше операций сравнений и обменов, а работают быстрее, чем **HeapSort** (в особенности алгоритм ShellSort_v3 работает быстрее).  
Я могу это объяснить либо особенностями алгоритма выбора шагов (видимо она дает лучшие результаты). Либо, начинает оказывать влияние то, что бинарному дереву для работы всё же приходится поддерживать структуру кучи, прыгать по индексам.

Также любопытно наблюдать за **SelectionSort**. Он очень предсказуемый, он всегда делает максимум `n(n-1)/2` сравнений и максимум `n-1` обменов. Его порядок по времени сравним с **BubbleSort**, зато он очень предсказуемый. Кстати, операций сравнений столько же, сколько и у **BubbleSort**, а операций обмена меньше (прямо кардинально меньше на больших N), но при этом время выполнения очень близкое с **BubbleSort**.  
Я могу это объяснить наверное только кешированием: ведь элементы массива располагаются близко друг к другу, и для **BubbleSort** наверное процессор более эффективно оптимизирует работу с памятью, а в случае **SelectionSort**, элемент может перетаскиваться очень далеко, и видимо случается намного больше промахов кеша и вытеснений.

##### Результаты теста с Digits распределением (особенность в том, что очень мало случайных значений)

При малом количестве уникальных значений в массиве, и при большом размере массива, - **HeapSort** опять же показывает себя хуже алгоритмов **ShellSort**.  
Как я понимаю, когда много одинаковых элементов - это идеальный случай для блочных сортировок вставками (которые использует ShellSort на каждом шаге), т.к. элементы массива часто уже стоят на своих местах или рядом. В то время как **HeapSort** же всё равно строит кучу и просеивает элементы, не получая преимущества от повторений.

##### Результаты теста с Sorted распределением (особенность в том, что уже почти отсортированный массив)

Алгоритм **SelectionSort** почти никак не реагирует на отсортированность, ему практически всё равно, отсортирован массив или нет. При N=100000 в сравнении с вариантом Random массива показатели операций сравнений и время выполнения остались почти такими же (`10.45` сек вместо `10.46` сек), хотя да, операций обменов стало на 2 порядка меньше (`1000` вместо `99987`).

Алгоритм **HeapSort** тоже не особо отреагировал на отсортированность. При N=100000 операций сравнений и операций обменов даже чуть больше, чем в варианте с Random массивом. Хотя по времени отработал побыстрее (`0.0055` сек вместо `0.007` сек).

На этом типе распределения, хорошо себя показывает **ShellSort**. При N=100000 в сравнении с Random массивом время выполнения сократилось до `0.0087` сек вместо `0.0116` сек. Потому что внутри он использует сортировку вставками по блокам. А сама сортировка вставками (**InsertionSort**) для почти отсортированного массива в сравнении с Random массивом показала ускорение в 33 раза (`0.0435` сек вместо `1.458` сек), хоть это и и всё равно медленно в абсолютных показателях, но хорошая адаптируемость в целом.

##### Результаты теста с Revers распределением

Алгоритмы **HeapSort** и **SelectionSort** ведут себя практически также как и на Random распределении и на Sorted распределении.

В таком распределении опять же лучше себя чувствует **ShellSort** (в особенности вариант ShellSort_v3), работает даже быстрее, чем на Random распределении.

##### Дополнительные моменты анализа

**SelectionSort** особенный. Наверное из-за его понятности, предсказуемости и минимальных требований к операциям обмена, его стоит использовать только в случае очень небольших N, и в случае, когда обмены очень-очень дороги. Например, если сортируются огромные структуры. Но даже и тут, вероятно, лучше окажется InsertionSort, хотя всё надо проверять.

**HeapSort** выглядит очень надежным вариантом. Он имеет гарантированную сложность O(N log N) даже в худшем случае. Да, он медленнее, чем ShellSort, сложнее сам по себе из-за поддержания структуры кучи и операций с бинарным деревом (сложнее ли в реализации - тут спорно, для Шелла надо реализовывать алгоритмы выбора шагов). Но он очень предсказуемо ведет себя на всех вариантах распределений массивов, наверное из-за такой предсказуемости он хорошо подойдет для каких-нибудь критических систем.
