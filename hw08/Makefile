CXXFLAGS += -std=c++17 -O2 -Wall -I.
LDFLAGS  += -lstdc++fs
CXX      := g++

all: \
hw08_sort \
hw08_external_sort \
hw08_checker \

clean:
	find . -name '*.o' -exec rm -f {} \;
	find . -name '*.a' -exec rm -f {} \;
	find . -name '*.d' -exec rm -f {} \;
	find . -name 'hw08_*' -exec rm -f {} \;

hw08_sort_files = main_sort.cpp QuickSort.cpp MergeSort.cpp
hw08_sort_inc = ISort.h QuickSort.h MergeSort.h

hw08_external_sort_files = main_external_sort.cpp ExternalSort.cpp InsertionSort.cpp
hw08_external_sort_inc = ExternalSort.h InsertionSort.h

hw08_checker_files = checker.cpp checker_internals.cpp BubbleSort.cpp InsertionSort.cpp ShellSort.cpp SelectionSort.cpp HeapSort.cpp QuickSort.cpp MergeSort.cpp
hw08_checker_inc = checker_internals.h ISort.h BubbleSort.h InsertionSort.h ShellSort.h SelectionSort.h HeapSort.h QuickSort.h MergeSort.h

hw08_sort: $(hw08_sort_files) $(hw08_sort_inc)
	$(CXX) $(LDFLAGS) $(CXXFLAGS) -o $@ $(hw08_sort_files)

hw08_external_sort: $(hw08_external_sort_files) $(hw08_external_sort_inc)
	$(CXX) $(LDFLAGS) $(CXXFLAGS) -o $@ $(hw08_external_sort_files)

hw08_checker: $(hw08_checker_files) $(hw08_checker_inc)
	$(CXX) $(LDFLAGS) $(CXXFLAGS) -o $@ $(hw08_checker_files)
