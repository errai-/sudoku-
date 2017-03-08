#include "Sudoku.h"

void Sudoku::init()
{
    for (unsigned row=0; row<9; ++row) {
        for (unsigned col=0; col<9; ++col) {
            for (unsigned nro=1; nro<=9; ++nro) {
                mVals.set(position(row,col)+nro);
            }
        }
    }
}

void Sudoku::print()
{
    for (unsigned row=0; row<9; ++row) {
        for (unsigned col=0; col<9; ++col) {
            cout << value(row,col);
        }
        cout << endl;
    }
    cout << endl;
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
        change = (blksToRowsCols());
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
        for (int i=1; i<=9; ++i)
            mVals.reset(pos+i);
        mVals.set(pos);
        mVals.set(pos+number);
        elimRow(row,number);
        elimCol(col,number);
        elimBlk(row,col,number);
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
    bool success=false;
    for (unsigned blkRow=0; blkRow<3; ++blkRow) {
        for (unsigned blkCol=0; blkCol<3; ++blkCol) {
            vector<unsigned> positions = blkRange(blkRow,blkCol);
            for (unsigned nro=1; nro<=9; ++nro) {
                unsigned bpos = 9*(3*blkRow+blkCol)+nro-1;
                if (mBlks.test(bpos))
                    continue;
                int counter=0;
                unsigned rPos=0;
                for (auto &pos : positions) {
                    if (mVals.test(pos+nro)) {
                        ++counter;
                        rPos=pos;
                    }
                }
                if (counter==1) {
                    pair<unsigned,unsigned> rowcol = pos2rowcol(rPos);
                    setValue(rowcol.first,rowcol.second,nro);
                    success=true;
                }
            }
        }
    }
    return success;
}

bool Sudoku::checkCells()
{
    bool success=false;
    for (unsigned row=0; row<9; ++row) {
        for (unsigned col=0; col<9; ++col) {
            unsigned pos = position(row,col);
            if (mVals.test(pos))
                continue;
            int counter=0;
            unsigned rNro=0;
            for (unsigned nro=1; nro<=9; ++nro) {
                if (mVals.test(pos+nro)) {
                    ++counter;
                    rNro=nro;
                }
            }
            if (counter==1) {
                setValue(row,col,rNro);
                success=true;
            }
        }
    }
    return success;
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
    for (unsigned pos=position(row,0); pos<position(row+1,0); pos+=10) {
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
    for (unsigned pos = position(0,col); pos<position(9,0); pos+=90) {
        if (!mVals.test(pos))
            mVals.reset(pos+number);
    }
}

void Sudoku::elimBlk(unsigned int row, unsigned int col, int number)
{
    if (col>8 || row>8 || number<1 || number>9)
        throw std::runtime_error("Erroneous value! (Sudoku::elimBlk)");
    unsigned blkRow = row/3;
    unsigned blkCol = col/3;
    unsigned bpos = 9*(3*blkRow+blkCol)+number-1;
    if (mBlks.test(bpos))
        throw std::runtime_error("This value was already set! (Sudoku::elimBlk)");
    mBlks.set(bpos);
    vector<unsigned> positions = blkRange(blkRow,blkCol);
    for (auto &pos : positions) {
        if (!mVals.test(pos))
            mVals.reset(pos+number);
    }
}

// Checking: to blocks

bool Sudoku::rowsToBlks()
{
    bool success = false;
    for (unsigned row=0; row<9; ++row) {
        for (unsigned nro=1; nro<=9; ++nro) {
            unsigned rpos = 9*row+nro-1;
            if (mRows.test(rpos))
                continue;
            unsigned pos = position(row,0)+nro;
            unsigned colPos = 3;
            for (unsigned col=0; col<9; ++col) {
                if (mVals.test(pos)) {
                    if (colPos==3) {
                        colPos = col/3;
                    } else {
                        unsigned block = col/3;
                        if (colPos!=block) {
                            colPos = 3;
                            break;
                        }
                    }
                }
                pos += 10;
            }
            if (colPos!=3) {
                unsigned rowPos = row/3;
                for (unsigned elimRow = 3*rowPos; elimRow<3*(rowPos+1); ++elimRow) {
                    if (elimRow==row)
                        continue;
                    unsigned init = position(elimRow,3*colPos)+nro;
                    for (pos=init; pos<init+30; pos+=10) {
                        if (mVals.test(pos))
                            success = true;
                        mVals.reset(pos);
                    }
                }
            }
        }
    }
    return success;
}

bool Sudoku::colsToBlks()
{
    bool success = false;
    for (unsigned col=0; col<9; ++col) {
        for (unsigned nro=1; nro<=9; ++nro) {
            unsigned cpos = 9*col+nro-1;
            if (mCols.test(cpos))
                continue;
            unsigned pos = position(0,col)+nro;
            unsigned rowPos = 3;
            for (unsigned row=0; row<9; ++row) {
                if (mVals.test(pos)) {
                    if (rowPos==3) {
                        rowPos = row/3;
                    } else {
                        unsigned block = row/3;
                        if (rowPos!=block) {
                            rowPos = 3;
                            break;
                        }
                    }
                }
                pos += 90;
            }
            if (rowPos!=3) {
                unsigned colPos = col/3;
                for (unsigned elimCol = 3*colPos; elimCol<3*(colPos+1); ++elimCol) {
                    if (elimCol==col)
                        continue;
                    unsigned init = position(3*rowPos,elimCol)+nro;
                    for (pos=init; pos<init+270; pos+=90) {
                        if (mVals.test(pos))
                            success = true;
                        mVals.reset(pos);
                    }
                }
            }
        }
    }
    return success;
}

// Checking: from blocks

bool Sudoku::blksToRowsCols()
{
    bool success = false;
    for (unsigned blkRow=0; blkRow<3; ++blkRow) {
        for (unsigned blkCol=0; blkCol<3; ++blkCol) {
            vector<unsigned> pos = blkRange(blkRow,blkCol);
            for (unsigned nro=1; nro<=9; ++nro) {
                unsigned bpos = 9*(3*blkRow+blkCol)+nro-1;
                if (mBlks.test(bpos))
                    continue;
                unsigned rPos=3,cPos=3;
                for (unsigned i=0; i<9; ++i) {
                    if (mVals.test(pos[i]+nro)) {
                        if (rPos==3) {
                            rPos = i/3;
                        } else {
                            unsigned lRow = i/3;
                            if (rPos!=lRow)
                                rPos = 4;
                        }
                        if (cPos==3) {
                            cPos = i%3;
                        } else {
                            unsigned lCol = i%3;
                            if (cPos!=lCol)
                                cPos = 4;
                        }
                    }
                }
                if (rPos<3) {
                    unsigned row = rPos+3*blkRow;
                    for (unsigned col=0; col<9; ++col) {
                        unsigned colPos = col/3;
                        if (colPos==blkCol)
                            continue;
                        unsigned pos = position(row,col)+nro;
                        if (mVals.test(pos))
                            success = true;
                        mVals.reset(pos);
                    }
                }
                if (cPos<3) {
                    unsigned col = cPos+3*blkCol;
                    for (unsigned row=0; row<9; ++row) {
                        unsigned rowPos = row/3;
                        if (rowPos==blkRow)
                            continue;
                        unsigned pos = position(row,col)+nro;
                        if (mVals.test(pos))
                            success = true;
                        mVals.reset(pos);
                    }
                }
            }
        }
    }
    return success;
}

// Finalization

bool Sudoku::finished()
{
    if (mRows.all() && mCols.all() && mBlks.all())
        return true;
    return false;
}

// Appending blk ranges

vector<unsigned> Sudoku::blkRange(unsigned int blkRow, unsigned int blkCol)
{
    vector<unsigned> blkRange;
    for (unsigned rowPos=3*blkRow; rowPos<3*blkRow+3; ++rowPos) {
        for (unsigned colPos=3*blkCol; colPos<3*blkCol+3; ++colPos) {
            blkRange.push_back(position(rowPos,colPos));
        }
    }
    return blkRange;
}

void Sudoku::numStatus(int number)
{
    for (unsigned row=0; row<9; ++row) {
        for (unsigned col=0; col<9; ++col) {
            unsigned pos=position(row,col);
            if (mVals.test(pos)) {
                cout << 1;
            } else if (mVals.test(pos+number)) {
                cout << 2;
            } else {
                cout << 0;
            }
        }
        cout << endl;
    }
    cout << endl;
}

pair<unsigned int, unsigned int> Sudoku::pos2rowcol(unsigned int position)
{
    unsigned row = position/90;
    unsigned col = position/10;
    col = col%9;
    return std::make_pair(row,col);
}

unsigned int Sudoku::counter()
{
    unsigned count = 0;
    unsigned pos = 0;
    while (pos < 810) {
        if (mVals.test(pos))
            ++count;
        pos += 10;
    }
    return count;
}

void Sudoku::possible(unsigned int row, unsigned int col)
{
    unsigned pos = position(row,col);
    for (unsigned nro=1; nro<=9; ++nro) {
        if (mVals.test(pos+nro))
            cout << nro;
    }
    cout << endl;
}
