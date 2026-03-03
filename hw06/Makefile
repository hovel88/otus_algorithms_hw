CXXFLAGS += -std=c++17 -O2 -Wall -I.
LDFLAGS  += -lstdc++fs
CXX      := g++

all: \
hw06_sort \
hw06_sort_adv \
hw06_checker \

clean:
	find . -name '*.o' -exec rm -f {} \;
	find . -name '*.a' -exec rm -f {} \;
	find . -name '*.d' -exec rm -f {} \;
	find . -name 'hw06_*' -exec rm -f {} \;

hw06_sort_files = main_sort.cpp BubbleSort.cpp InsertionSort.cpp ShellSort.cpp
hw06_sort_inc = ISort.h BubbleSort.h InsertionSort.h ShellSort.h

hw06_sort_adv_files = main_sort_adv.cpp BubbleSort.cpp InsertionSort.cpp ShellSort.cpp
hw06_sort_adv_inc = ISort.h BubbleSort.h InsertionSort.h ShellSort.h

hw06_checker_files = checker.cpp checker_internals.cpp BubbleSort.cpp InsertionSort.cpp ShellSort.cpp
hw06_checker_inc = checker_internals.h ISort.h BubbleSort.h InsertionSort.h ShellSort.h

hw06_sort: $(hw06_sort_files) $(hw06_sort_inc)
	$(CXX) $(LDFLAGS) $(CXXFLAGS) -o $@ $(hw06_sort_files)

hw06_sort_adv: $(hw06_sort_adv_files) $(hw06_sort_adv_inc)
	$(CXX) $(LDFLAGS) $(CXXFLAGS) -o $@ $(hw06_sort_adv_files)

hw06_checker: $(hw06_checker_files) $(hw06_checker_inc)
	$(CXX) $(LDFLAGS) $(CXXFLAGS) -o $@ $(hw06_checker_files)
