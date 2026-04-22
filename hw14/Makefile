CXXFLAGS += -std=c++17 -O2 -Wall -I.
LDFLAGS  += -lstdc++fs
CXX      := g++

all: \
hw14_graph \

clean:
	find . -name '*.o' -exec rm -f {} \;
	find . -name '*.a' -exec rm -f {} \;
	find . -name '*.d' -exec rm -f {} \;
	find . -name 'hw14_*' -exec rm -f {} \;

hw14_graph_files = main.cpp
hw14_graph_inc =

hw14_graph: $(hw14_graph_files) $(hw14_graph_inc)
	$(CXX) $(LDFLAGS) $(CXXFLAGS) -o $@ $(hw14_graph_files)

graph: graph_undirected.dot
	dot -Tpng graph_undirected.dot -o graph.png
