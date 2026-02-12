CXXFLAGS += -std=c++17 -O2 -Wall -I.
LDFLAGS  += -lstdc++fs
CXX      := g++

all: \
hw03_power_checker \
hw03_power_iterative \
hw03_power_div_power \
hw03_power_binary_power \

clean:
	find . -name '*.o' -exec rm -f {} \;
	find . -name '*.a' -exec rm -f {} \;
	find . -name '*.d' -exec rm -f {} \;
	find . -name 'hw03_power*' -exec rm -f {} \;

hw03_power_iterative_files = main_iterative.cpp algo_iterative.cpp
hw03_power_div_power_files = main_div_power.cpp algo_div_power.cpp
hw03_power_binary_power_files = main_binary_power.cpp algo_binary_power.cpp
hw03_power_checker_files = checker.cpp checker_internals.cpp algo_iterative.cpp algo_div_power.cpp algo_binary_power.cpp

hw03_power_iterative: $(hw03_power_iterative_files)
	$(CXX) $(LDFLAGS) $(CXXFLAGS) -o $@ $(hw03_power_iterative_files)

hw03_power_div_power: $(hw03_power_div_power_files)
	$(CXX) $(LDFLAGS) $(CXXFLAGS) -o $@ $(hw03_power_div_power_files)

hw03_power_binary_power: $(hw03_power_binary_power_files)
	$(CXX) $(LDFLAGS) $(CXXFLAGS) -o $@ $(hw03_power_binary_power_files)

hw03_power_checker: $(hw03_power_checker_files)
	$(CXX) $(LDFLAGS) $(CXXFLAGS) -o $@ $(hw03_power_checker_files)
