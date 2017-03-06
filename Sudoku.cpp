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
        elimRow(row,number);
        elimCol(row,number);
        elimBlk(row,col,number);
    } else {
        for (int i=1; i<=9; ++i)
            mVals.set(pos+i);
    }
}

// Checking functions:

bool Sudoku::checkRows()
{
    bool success=false;
    for (unsigned row=0; row<9; ++row) {
        for (unsigned nro=1; nro<=9; ++nro) {
            if (mRows.test(9*row+nro-1))
                continue;
            int counter=0;
            unsigned rCol=0;
            for (unsigned col=0; col<9; ++col) {
                unsigned pos=position(row,col)+nro;
                if (mVals.test(pos)) {
                    ++counter;
                    rCol=col;
                }
            }
            if (counter==1) {
                setValue(row,rCol,nro);
                success=true;
            }
        }
    }
    return success;
}

bool Sudoku::checkCols()
{
    bool success=false;
    for (unsigned col=0; col<9; ++col) {
        for (unsigned nro=1; nro<=9; ++nro) {
            if (mCols.test(9*col+nro-1))
                continue;
            int counter=0;
            unsigned rRow=0;
            for (unsigned row=0; row<9; ++row) {
                unsigned pos=position(row,col)+nro;
                if (mVals.test(pos)) {
                    ++counter;
                    rRow=row;
                }
            }
            if (counter==1) {
                setValue(rRow,col,nro);
                success=true;
            }
        }
    }
    return success;
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
    if (row>8 || number<1 || number>9)
        throw std::runtime_error("Erroneous value! (Sudoku::elimRow)");
    unsigned rpos = 9*row+number-1;
    if (mRows.test(rpos))
        throw std::runtime_error("This value was already set! (Sudoku::elimRow)");
    mRows.set(rpos);
    for (unsigned pos=90*row; pos<90*(row+1); pos+=10) {
        if (!mVals.test(pos)) {
            mVals.reset(pos+number);
        }
    }
}

void Sudoku::elimCol(unsigned int col, int number)
{
    if (col>8 || number<1 || number>9)
        throw std::runtime_error("Erroneous value! (Sudoku::elimCol)");
    unsigned cpos = 9*col+number-1;
    if (mCols.test(cpos))
        throw std::runtime_error("This value was already set! (Sudoku::elimCol)");
    mCols.set(cpos);
    for (unsigned pos = 10*col; pos<810; pos+=90) {
        if (!mVals.test(pos))
            mVals.reset(pos+number);
    }
}

void Sudoku::elimBlk(unsigned int row, unsigned int col, int number)
{
}

// Checking: to blocks

bool Sudoku::rowsToBlks()
{
    return false;
}

bool Sudoku::colsToBlks()
{
    return false;
}

// Checking: from blocks

bool Sudoku::blksToRows()
{
    return false;
}

bool Sudoku::blksToCols()
{
    return false;
}

// Finalization

bool Sudoku::finished()
{
    if (mRows.all() && mCols.all() && mBlks.all())
        return true;
    return false;
}

// Appending blk ranges

void Sudoku::blkRange(unsigned int blkRow, unsigned int blkCol)
{
    vector<unsigned> blkRange;
    for (unsigned rowPos=3*blkRow; rowPos<3*blkRow+3; ++rowPos) {
        for (unsigned colPos=3*blkCol; colPos<3*blkCol+3); ++colPos) {
            blkRange.push_back(position(rowPos,colPos));
        }
    }
    mBlkRanges.push_back(blkRange);
}
