
CXX=g++
CXXFLAGS=-O2 -march=native -std=c++14 -Wall -Wextra

sudoku: main.cpp sudokuhandle.o sudoku.o
	$(CXX) $(CXXFLAGS) $^ -o $@

sudokuhandle.o: SudokuHandle.cpp SudokuHandle.h
	$(CXX) $(CXXFLAGS) -c SudokuHandle.cpp  -o $@

sudoku.o: Sudoku.cpp Sudoku.h
	$(CXX) $(CXXFLAGS) -c Sudoku.cpp -o $@
