CXXFLAGS += -std=c++17 -O2 -Wall -I.
LDFLAGS  += -lstdc++fs
CXX      := g++

all: \
hw11_search \

clean:
	find . -name '*.o' -exec rm -f {} \;
	find . -name '*.a' -exec rm -f {} \;
	find . -name '*.d' -exec rm -f {} \;
	find . -name 'hw11_*' -exec rm -f {} \;

hw11_search_files = main_search.cpp BST.cpp AVL_V1.cpp AVL_V2.cpp Treap.cpp Splay.cpp RandBST.cpp
hw11_search_inc = ITree.h BST.h AVL_V1.h AVL_V2.h Treap.h Splay.h RandBST.h

hw11_search: $(hw11_search_files) $(hw11_search_inc)
	$(CXX) $(LDFLAGS) $(CXXFLAGS) -o $@ $(hw11_search_files)
