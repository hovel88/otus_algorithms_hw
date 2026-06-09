CXXFLAGS += -std=c++17 -O2 -Wall -I.
LDFLAGS  += -lstdc++fs
CXX      := g++

all: \
hw21_ex6 \
hw21_ex6_checker \

clean:
	find . -name '*.o' -exec rm -f {} \;
	find . -name '*.a' -exec rm -f {} \;
	find . -name '*.d' -exec rm -f {} \;
	find . -name 'hw21_*' -exec rm -f {} \;

hw21_ex6_files = main.cpp
hw21_ex6_inc = algo.h
hw21_ex6_checker_files = main_checker.cpp checker_internals.cpp
hw21_ex6_checker_inc = algo.h checker_internals.h

hw21_ex6: $(hw21_ex6_files) $(hw21_ex6_inc)
	$(CXX) $(LDFLAGS) $(CXXFLAGS) -o $@ $(hw21_ex6_files)

hw21_ex6_checker: $(hw21_ex6_checker_files) $(hw21_ex6_checker_inc)
	$(CXX) $(LDFLAGS) $(CXXFLAGS) -o $@ $(hw21_ex6_checker_files)
