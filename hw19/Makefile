CXXFLAGS += -std=c++17 -O2 -Wall -I.
LDFLAGS  += -lstdc++fs
CXX      := g++

all: \
hw19_string \

clean:
	find . -name '*.o' -exec rm -f {} \;
	find . -name '*.a' -exec rm -f {} \;
	find . -name '*.d' -exec rm -f {} \;
	find . -name 'hw19_*' -exec rm -f {} \;

hw19_string_files = main.cpp
hw19_string_inc = StringSearch.h StringAutomaton.h

hw19_string: $(hw19_string_files) $(hw19_string_inc)
	$(CXX) $(LDFLAGS) $(CXXFLAGS) -o $@ $(hw19_string_files)
