CXXFLAGS += -std=c++17 -O2 -Wall -I.
LDFLAGS  += -lstdc++fs
CXX      := g++

all: \
hw09_sort \

clean:
	find . -name '*.o' -exec rm -f {} \;
	find . -name '*.a' -exec rm -f {} \;
	find . -name '*.d' -exec rm -f {} \;
	find . -name 'hw09_*' -exec rm -f {} \;

hw09_sort_files = main_sort.cpp CountingSort.cpp RadixSort.cpp BucketSort.cpp
hw09_sort_inc = SortStatistic.h CountingSort.h RadixSort.h BucketSort.h

hw09_sort: $(hw09_sort_files) $(hw09_sort_inc)
	$(CXX) $(LDFLAGS) $(CXXFLAGS) -o $@ $(hw09_sort_files)
