CXXFLAGS += -std=c++17 -O2 -Wall -I.
LDFLAGS  += -lstdc++fs
CXX      := g++

all: \
hw03_primes_checker \
hw03_primes_base \
hw03_primes_sqrt \
hw03_primes_odd \
hw03_primes_sqrt_div_2_3 \
hw03_primes_div_primes \
hw03_primes_eratosthenes \

clean:
	find . -name '*.o' -exec rm -f {} \;
	find . -name '*.a' -exec rm -f {} \;
	find . -name '*.d' -exec rm -f {} \;
	find . -name 'hw03_primes*' -exec rm -f {} \;

hw03_primes_base_files = main_base.cpp algo_base.cpp
hw03_primes_sqrt_files = main_sqrt.cpp algo_sqrt.cpp
hw03_primes_odd_files = main_odd.cpp algo_odd.cpp
hw03_primes_sqrt_div_2_3_files = main_sqrt_div_2_3.cpp algo_sqrt_div_2_3.cpp
hw03_primes_div_primes_files = main_div_primes.cpp algo_div_primes.cpp
hw03_primes_eratosthenes_files = main_eratosthenes.cpp algo_eratosthenes.cpp
hw03_primes_checker_files = checker.cpp checker_internals.cpp algo_base.cpp algo_sqrt.cpp algo_odd.cpp algo_sqrt_div_2_3.cpp algo_div_primes.cpp algo_eratosthenes.cpp

hw03_primes_base: $(hw03_primes_base_files)
	$(CXX) $(LDFLAGS) $(CXXFLAGS) -o $@ $(hw03_primes_base_files)

hw03_primes_sqrt: $(hw03_primes_sqrt_files)
	$(CXX) $(LDFLAGS) $(CXXFLAGS) -o $@ $(hw03_primes_sqrt_files)

hw03_primes_odd: $(hw03_primes_odd_files)
	$(CXX) $(LDFLAGS) $(CXXFLAGS) -o $@ $(hw03_primes_odd_files)

hw03_primes_sqrt_div_2_3: $(hw03_primes_sqrt_div_2_3_files)
	$(CXX) $(LDFLAGS) $(CXXFLAGS) -o $@ $(hw03_primes_sqrt_div_2_3_files)

hw03_primes_div_primes: $(hw03_primes_div_primes_files)
	$(CXX) $(LDFLAGS) $(CXXFLAGS) -o $@ $(hw03_primes_div_primes_files)

hw03_primes_eratosthenes: $(hw03_primes_eratosthenes_files)
	$(CXX) $(LDFLAGS) $(CXXFLAGS) -o $@ $(hw03_primes_eratosthenes_files)

hw03_primes_checker: $(hw03_primes_checker_files)
	$(CXX) $(LDFLAGS) $(CXXFLAGS) -o $@ $(hw03_primes_checker_files)
