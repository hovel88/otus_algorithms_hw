CXXFLAGS += -std=c++17 -O2 -Wall -I.
LDFLAGS  += -lstdc++fs
CXX      := g++

all: \
hw15_graph \

clean:
	find . -name '*.o' -exec rm -f {} \;
	find . -name '*.a' -exec rm -f {} \;
	find . -name '*.d' -exec rm -f {} \;
	find . -name 'hw15_*' -exec rm -f {} \;

hw15_graph_files = main.cpp
hw15_graph_inc = graph.h

hw15_graph: $(hw15_graph_files) $(hw15_graph_inc)
	$(CXX) $(LDFLAGS) $(CXXFLAGS) -o $@ $(hw15_graph_files)
