CXXFLAGS += -std=c++17 -Wall -I. -O0
LDFLAGS  += -lstdc++fs
CXX      := g++

all: \
hw04_dyn_arrays \
hw04_priority_queue \
hw04_space_array \

clean:
	find . -name '*.o' -exec rm -f {} \;
	find . -name '*.a' -exec rm -f {} \;
	find . -name '*.d' -exec rm -f {} \;
	find . -name 'hw04_*' -exec rm -f {} \;

hw04_dyn_arrays: main_dyn_arrays.cpp SingleArray.h VectorArray.h FactorArray.h MatrixArray.h ArrayList.h
	$(CXX) $(LDFLAGS) $(CXXFLAGS) -o $@ main_dyn_arrays.cpp

hw04_priority_queue: main_priority_queue.cpp PriorityQueue.h
	$(CXX) $(LDFLAGS) $(CXXFLAGS) -o $@ main_priority_queue.cpp

hw04_space_array: main_space_array.cpp SpaceArray.h
	$(CXX) $(LDFLAGS) $(CXXFLAGS) -o $@ main_space_array.cpp
