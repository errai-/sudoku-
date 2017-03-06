#ifndef SUDOKUHANDLE
#define SUDOKUHANDLE

#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include <fstream>
#include <regex>

using std::cout;
using std::endl;
using std::vector;
using std::string;
using std::exception;
using std::ifstream;

#include "Sudoku.h"

class SudokuHandle {
private:
    vector<Sudoku> mSudokus;
    
public:
    SudokuHandle() {}
    
    bool add(string file);
    
    void loop(int limit = -1);
    
    void clear() { mSudokus.clear(); }
};


#endif // SUDOKUHANDLE
