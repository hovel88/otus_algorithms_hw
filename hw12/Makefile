CXXFLAGS += -std=c++17 -O2 -Wall -I.
LDFLAGS  += -lstdc++fs
CXX      := g++

all: \
hw12_hash \

clean:
	find . -name '*.o' -exec rm -f {} \;
	find . -name '*.a' -exec rm -f {} \;
	find . -name '*.d' -exec rm -f {} \;
	find . -name 'hw12_*' -exec rm -f {} \;

hw12_hash_files = main_hash.cpp
hw12_hash_inc = IHashTable.h Hasher.h ChainedHashTable.h AddressingHashTable.h

hw12_hash: $(hw12_hash_files) $(hw12_hash_inc)
	$(CXX) $(LDFLAGS) $(CXXFLAGS) -o $@ $(hw12_hash_files)
