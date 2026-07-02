CXXFLAGS += -std=c++17 -O3 -march=native -Wall -I.
LDFLAGS  += -lstdc++fs
CXX      := g++

all: \
hw22_hll_cms \

clean:
	find . -name '*.o' -exec rm -f {} \;
	find . -name '*.a' -exec rm -f {} \;
	find . -name '*.d' -exec rm -f {} \;
	find . -name 'hw22_*' -exec rm -f {} \;

hw22_hll_cms_files = main.cpp
hw22_hll_cms_inc = common.h i_probabilistic_counter.h algo_hll.h algo_cms.h

hw22_hll_cms: $(hw22_hll_cms_files) $(hw22_hll_cms_inc)
	$(CXX) $(LDFLAGS) $(CXXFLAGS) -o $@ $(hw22_hll_cms_files)
