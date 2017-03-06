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
    bitset<81> mRows;
    bitset<81> mCols;
    bitset<81> mBlks;
    string mName;
    int mLines;
    vector< vector<unsigned> > mBlkRanges;

    unsigned position(unsigned row, unsigned col);
    int value(unsigned row, unsigned col);
    void setValue(unsigned row, unsigned col, int number);
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
    void blkRange(unsigned blkRow, unsigned blkCol);
    
public:
    Sudoku(string name) : mName(name), mLines(0) {
        for (unsigned blkRow=0; blkRow<3; ++blkRow) {
            for (unsigned blkCol=0; blkCol<3; ++blkCol) {
                blkRange(blkRow, blkCol);
            }
        }
    }
    
    bool readLine(string line);

    void print();
    
    bool solve();
    
    string getName() { return mName; }
};

#endif // SUDOKU
