CXXFLAGS += -std=c++17 -O2 -Wall -I.
LDFLAGS  += -lstdc++fs
CXX      := g++

all: \
hw10_search \

clean:
	find . -name '*.o' -exec rm -f {} \;
	find . -name '*.a' -exec rm -f {} \;
	find . -name '*.d' -exec rm -f {} \;
	find . -name 'hw10_*' -exec rm -f {} \;

hw10_search_files = main_search.cpp BST.cpp AVL_V1.cpp AVL_V2.cpp Treap.cpp
hw10_search_inc = ITree.h BST.h AVL_V1.h AVL_V2.h Treap.h

hw10_search: $(hw10_search_files) $(hw10_search_inc)
	$(CXX) $(LDFLAGS) $(CXXFLAGS) -o $@ $(hw10_search_files)
