CXXFLAGS += -std=c++17 -O2 -Wall -I.
LDFLAGS  += -lstdc++fs
CXX      := g++

all: \
hw07_sort \
hw07_checker \

clean:
	find . -name '*.o' -exec rm -f {} \;
	find . -name '*.a' -exec rm -f {} \;
	find . -name '*.d' -exec rm -f {} \;
	find . -name 'hw07_*' -exec rm -f {} \;

hw07_sort_files = main_sort.cpp SelectionSort.cpp HeapSort.cpp
hw07_sort_inc = ISort.h SelectionSort.h HeapSort.h

hw07_checker_files = checker.cpp checker_internals.cpp BubbleSort.cpp InsertionSort.cpp ShellSort.cpp SelectionSort.cpp HeapSort.cpp
hw07_checker_inc = checker_internals.h ISort.h BubbleSort.h InsertionSort.h ShellSort.h SelectionSort.h HeapSort.h

hw07_sort: $(hw07_sort_files) $(hw07_sort_inc)
	$(CXX) $(LDFLAGS) $(CXXFLAGS) -o $@ $(hw07_sort_files)

hw07_checker: $(hw07_checker_files) $(hw07_checker_inc)
	$(CXX) $(LDFLAGS) $(CXXFLAGS) -o $@ $(hw07_checker_files)
