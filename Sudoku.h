// Sudoku base class. Remember: rows and columns range from 0 to 8

#ifndef SUDOKU
#define SUDOKU

#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include <fstream>
#include <bitset>

using std::cout;
using std::endl;
using std::vector;
using std::string;
using std::exception;
using std::ifstream;
using std::bitset;


class Sudoku {
private:
    bitset<810> mVals;
    string mName;
    int mLines;

    unsigned position(unsigned row, unsigned col);
    int value(unsigned row, unsigned col);
    void setValue(unsigned row, unsigned col, int number);
    
public:
    Sudoku(string name) : mName(name), mLines(0) {}
    
    bool readLine(string line);

    void print();
    
    bool solve();
    
    string getName() { return mName; }
};

#endif // SUDOKU
