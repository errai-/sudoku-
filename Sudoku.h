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
using std::pair;


class Sudoku {
private:
    bitset<810> mVals;
    bitset<81> mRows;
    bitset<81> mCols;
    bitset<81> mBlks;
    string mName;
    int mLines;

    unsigned position(unsigned row, unsigned col);
    int value(unsigned row, unsigned col);
    void setValue(unsigned row, unsigned col, int number);
    pair<unsigned,unsigned> pos2rowcol(unsigned position);
    void elimRow(unsigned row, int number);
    void elimCol(unsigned col, int number);
    void elimBlk(unsigned row, unsigned col, int number);
    bool checkCells();
    bool checkRows();
    bool checkCols();
    bool checkBlks();
    bool rowsToBlks();
    bool colsToBlks();
    bool blksToCols();
    bool blksToRows();
    bool finished();
    vector<unsigned> blkRange(unsigned blkRow, unsigned blkCol);
    void numStatus(int number);
    
public:
    Sudoku(string name) : mName(name), mLines(0) {}
    
    ~Sudoku() {}
    
    bool readLine(string line);

    void print();
    
    bool solve();
    
    string getName() { return mName; }
};

#endif // SUDOKU
