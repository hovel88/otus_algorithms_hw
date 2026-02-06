CXXFLAGS += -std=c++17 -O2 -Wall -I.
LDFLAGS  += -lstdc++fs
CXX      := g++

all: \
hw02_strings \
hw02_strings_checker \

clean:
	find . -name '*.o' -exec rm -f {} \;
	find . -name '*.a' -exec rm -f {} \;
	find . -name '*.d' -exec rm -f {} \;
	find . -name 'hw02_strings*' -exec rm -f {} \;

hw02_strings_files = main.cpp strings_algo.cpp
hw02_strings_checker_files = checker.cpp checker_internals.cpp strings_algo.cpp

hw02_strings: $(hw02_strings_files)
	$(CXX) $(LDFLAGS) $(CXXFLAGS) -o $@ $(hw02_strings_files)

hw02_strings_checker: $(hw02_strings_checker_files)
	$(CXX) $(LDFLAGS) $(CXXFLAGS) -o $@ $(hw02_strings_checker_files)
