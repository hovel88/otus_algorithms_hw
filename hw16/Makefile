CXXFLAGS += -std=c++17 -O2 -Wall -I.
LDFLAGS  += -lstdc++fs
CXX      := g++

all: \
hw16_graph \

clean:
	find . -name '*.o' -exec rm -f {} \;
	find . -name '*.a' -exec rm -f {} \;
	find . -name '*.d' -exec rm -f {} \;
	find . -name 'hw16_*' -exec rm -f {} \;

hw16_graph_files = main.cpp
hw16_graph_inc = weighted_graph.h union_find.h

hw16_graph: $(hw16_graph_files) $(hw16_graph_inc)
	$(CXX) $(LDFLAGS) $(CXXFLAGS) -o $@ $(hw16_graph_files)
