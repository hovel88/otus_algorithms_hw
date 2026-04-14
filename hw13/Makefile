CXXFLAGS += -std=c++17 -O2 -Wall -I.
LDFLAGS  += -lstdc++fs
CXX      := g++

all: \
hw13_hash \

clean:
	find . -name '*.o' -exec rm -f {} \;
	find . -name '*.a' -exec rm -f {} \;
	find . -name '*.d' -exec rm -f {} \;
	find . -name 'hw13_*' -exec rm -f {} \;

hw13_hash_files = main_hash.cpp
hw13_hash_inc = IHashTable.h Hasher.h ChainedHashTable.h AddressingHashTable.h TrieTable.h

hw13_hash: $(hw13_hash_files) $(hw13_hash_inc)
	$(CXX) $(LDFLAGS) $(CXXFLAGS) -o $@ $(hw13_hash_files)
