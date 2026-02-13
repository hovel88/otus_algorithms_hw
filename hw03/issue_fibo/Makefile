CXXFLAGS += -std=c++17 -O2 -Wall -I.
LDFLAGS  += -lstdc++fs
CXX      := g++

all: \
hw03_fibo_checker \
hw03_fibo_recursive \
hw03_fibo_iterative \
hw03_fibo_gold \
hw03_fibo_matrix \

clean:
	find . -name '*.o' -exec rm -f {} \;
	find . -name '*.a' -exec rm -f {} \;
	find . -name '*.d' -exec rm -f {} \;
	find . -name 'hw03_fibo*' -exec rm -f {} \;

hw03_fibo_recursive_files = main_recursive.cpp algo_recursive.cpp
hw03_fibo_iterative_files = main_iterative.cpp algo_iterative.cpp
hw03_fibo_gold_files = main_gold.cpp algo_gold.cpp
hw03_fibo_matrix_files = main_matrix.cpp algo_matrix.cpp
hw03_fibo_checker_files = checker.cpp checker_internals.cpp algo_recursive.cpp algo_iterative.cpp algo_gold.cpp algo_matrix.cpp

hw03_fibo_recursive: $(hw03_fibo_recursive_files)
	$(CXX) $(LDFLAGS) $(CXXFLAGS) -o $@ $(hw03_fibo_recursive_files)

hw03_fibo_iterative: $(hw03_fibo_iterative_files)
	$(CXX) $(LDFLAGS) $(CXXFLAGS) -o $@ $(hw03_fibo_iterative_files)

hw03_fibo_gold: $(hw03_fibo_gold_files)
	$(CXX) $(LDFLAGS) $(CXXFLAGS) -o $@ $(hw03_fibo_gold_files)

hw03_fibo_matrix: $(hw03_fibo_matrix_files)
	$(CXX) $(LDFLAGS) $(CXXFLAGS) -o $@ $(hw03_fibo_matrix_files)

hw03_fibo_checker: $(hw03_fibo_checker_files)
	$(CXX) $(LDFLAGS) $(CXXFLAGS) -o $@ $(hw03_fibo_checker_files)
