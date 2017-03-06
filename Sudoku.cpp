#include "Sudoku.h"

void Sudoku::print()
{
    for (unsigned row=0; row<9; ++row) {
        for (unsigned col=0; col<9; ++col) {
            cout << value(row,col);
        }
        cout << endl;
    }
}

bool Sudoku::readLine(string line)
{
    if (mLines>8)
        throw std::runtime_error("Too many sudoku lines to be read! (Sudoku::readLine)");
    if (line.length()!=9)
        throw std::runtime_error("Wrong line length! (Sudoku::readLine)");
    
    for (unsigned i=0; i<9; ++i) {
        int number = int(line[i]-'0');
        setValue(mLines,i,number);
    }
    ++mLines;
    return true;
}

bool Sudoku::solve()
{
    if (mLines!=9)
        throw std::runtime_error("An incorrect number of lines spotted! (Sudoku::solve)");
    
    bool change = true;
    while (change) {
        change = (checkCells() || checkRows() || checkCols() || checkBlks());
        if (change) continue;
        change = (rowsToBlks() || colsToBlks());
        if (change) continue;
        change = (blksToRows() || blksToCols());
    }
    
    if (finished())
        return true;
    return false;
}

inline unsigned Sudoku::position(unsigned int row, unsigned int col)
{
    return 10*(9*row+col);
}

inline int Sudoku::value(unsigned row, unsigned col)
{
    int value = 0;
    unsigned pos = position(row,col);
    if (mVals.test(pos)) {
        // Value according to the bit position
        for (unsigned i=1; i<=9; ++i) {
            ++value;
            if (mVals.test(pos+i))
                break;
        }
    }
    return value;
}

void Sudoku::setValue(unsigned int row, unsigned int col, int number)
{
    unsigned pos = position(row,col);
    if (number<0 || number>9)
        throw std::runtime_error("Problematic value offered! (Sudoku::setValue)");
    if (mVals.test(pos))
        throw std::runtime_error("Cell value already set! (Sudoku::setValue)");
    if (number>0) {
        mVals.set(pos);
        mVals.set(pos+number);
    }
}

// Checking functions:

bool Sudoku::checkRows()
{
    return false;
}

bool Sudoku::checkCols()
{
    return false;
}

bool Sudoku::checkBlks()
{
    return false;
}

bool Sudoku::checkCells()
{
    return false;
}

// Elimination functions:

void Sudoku::elimRow(unsigned int row, int number)
{
}

void Sudoku::elimCol(unsigned int col, int number)
{
}

void Sudoku::elimBlk(unsigned int row, unsigned int col, int number)
{
}

// Checking: to blocks

bool Sudoku::rowsToBlks()
{
}

bool Sudoku::colsToBlks()
{
}

// Checking: from blocks

bool Sudoku::blksToRows()
{
}

bool Sudoku::blksToCols()
{
}

// Finalization

bool Sudoku::finished()
{
}
