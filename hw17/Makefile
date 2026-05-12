CXXFLAGS += -std=c++17 -O2 -Wall -I.
LDFLAGS  += -lstdc++fs
CXX      := g++

all: \
hw17_graph \

clean:
	find . -name '*.o' -exec rm -f {} \;
	find . -name '*.a' -exec rm -f {} \;
	find . -name '*.d' -exec rm -f {} \;
	find . -name 'hw17_*' -exec rm -f {} \;

hw17_graph_files = main.cpp
hw17_graph_inc = weighted_graph_sp.h common.h priority_queue.h

hw17_graph: $(hw17_graph_files) $(hw17_graph_inc)
	$(CXX) $(LDFLAGS) $(CXXFLAGS) -o $@ $(hw17_graph_files)
