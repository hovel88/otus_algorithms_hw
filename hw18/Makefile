CXXFLAGS += -std=c++17 -O2 -Wall -I.
LDFLAGS  += -lstdc++fs
CXX      := g++

all: \
hw18_string \

clean:
	find . -name '*.o' -exec rm -f {} \;
	find . -name '*.a' -exec rm -f {} \;
	find . -name '*.d' -exec rm -f {} \;
	find . -name 'hw18_*' -exec rm -f {} \;

hw18_string_files = main.cpp
hw18_string_inc = StringSearch.h

hw18_string: $(hw18_string_files) $(hw18_string_inc)
	$(CXX) $(LDFLAGS) $(CXXFLAGS) -o $@ $(hw18_string_files)
