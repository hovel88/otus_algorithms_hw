CXXFLAGS += -std=c++17 -O2 -Wall -I.
LDFLAGS  += -lstdc++fs
CXX      := g++

all: \
hw05_king \
hw05_knight \
hw05_rook \
hw05_bishop \
hw05_queen \
hw05_chess_checker \

clean:
	find . -name '*.o' -exec rm -f {} \;
	find . -name '*.a' -exec rm -f {} \;
	find . -name '*.d' -exec rm -f {} \;
	find . -name 'hw05_*' -exec rm -f {} \;

hw05_king_files = main_king.cpp bit_count.cpp
hw05_knight_files = main_knight.cpp bit_count.cpp
hw05_rook_files = main_rook.cpp bit_count.cpp
hw05_bishop_files = main_bishop.cpp bit_count.cpp
hw05_queen_files = main_queen.cpp bit_count.cpp
hw05_chess_checker_files = checker.cpp checker_internals.cpp bit_count.cpp

hw05_king: $(hw05_king_files) chess_king.h chess_board.h
	$(CXX) $(LDFLAGS) $(CXXFLAGS) -o $@ $(hw05_king_files)

hw05_knight: $(hw05_knight_files) chess_knight.h chess_board.h
	$(CXX) $(LDFLAGS) $(CXXFLAGS) -o $@ $(hw05_knight_files)

hw05_rook: $(hw05_rook_files) chess_rook.h chess_board.h
	$(CXX) $(LDFLAGS) $(CXXFLAGS) -o $@ $(hw05_rook_files)

hw05_bishop: $(hw05_bishop_files) chess_bishop.h chess_board.h
	$(CXX) $(LDFLAGS) $(CXXFLAGS) -o $@ $(hw05_bishop_files)

hw05_queen: $(hw05_queen_files) chess_queen.h chess_board.h
	$(CXX) $(LDFLAGS) $(CXXFLAGS) -o $@ $(hw05_queen_files)

hw05_chess_checker: $(hw05_chess_checker_files)
	$(CXX) $(LDFLAGS) $(CXXFLAGS) -o $@ $(hw05_chess_checker_files)
