CXXFLAGS += -std=c++17 -Wall -I. -O2
LDFLAGS  += -lstdc++fs
CXX      := g++

all: \
hw02_tickets \
hw02_tickets_checker \

clean:
	find . -name '*.o' -exec rm -f {} \;
	find . -name '*.a' -exec rm -f {} \;
	find . -name '*.d' -exec rm -f {} \;
	find . -name 'hw02_tickets*' -exec rm -f {} \;

hw02_tickets_files = main.cpp tickets_algo.cpp
hw02_tickets_checker_files = checker.cpp checker_internals.cpp tickets_algo.cpp

hw02_tickets: $(hw02_tickets_files)
	$(CXX) $(LDFLAGS) $(CXXFLAGS) -o $@ $(hw02_tickets_files)

hw02_tickets_checker: $(hw02_tickets_checker_files)
	$(CXX) $(LDFLAGS) $(CXXFLAGS) -o $@ $(hw02_tickets_checker_files)
