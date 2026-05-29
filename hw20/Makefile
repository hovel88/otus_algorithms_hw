CXXFLAGS += -std=c++17 -O2 -Wall -I.
LDFLAGS  += -lstdc++fs
CXX      := g++

all: \
hw20_rle \

clean:
	find . -name '*.o' -exec rm -f {} \;
	find . -name '*.a' -exec rm -f {} \;
	find . -name '*.d' -exec rm -f {} \;
	find . -name 'hw20_*' -exec rm -f {} \;

hw20_rle_files = main.cpp
hw20_rle_inc = RLE.h

hw20_rle: $(hw20_rle_files) $(hw20_rle_inc)
	$(CXX) $(LDFLAGS) $(CXXFLAGS) -o $@ $(hw20_rle_files)
