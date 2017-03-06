#include "SudokuHandle.h"

bool SudokuHandle::add(string file)
{
    cout << "Adding file " << file << endl;
    ifstream reading(file);
    if (reading.is_open()) {
        string line;
        std::regex header("Grid [0-9]{1,9}");
        std::regex numbers("[0-9]{9}");
        // Loop over sudokus
        while (getline(reading,line)) {
            if (!std::regex_match(line,header))
                return false;
            Sudoku newSud(line);
            // Loop over sudoku lines
            for (int i=0; i<9; ++i) {
                getline(reading,line);
                if (!std::regex_match(line,numbers))
                    return false;
                newSud.readLine(line);
            }
            mSudokus.push_back(newSud);
        }
    } else {
        return false;
    }
    return true;
}

void SudokuHandle::loop(int limit)
{
    unsigned iter = (limit < 0) ? mSudokus.size() : limit;
    cout << "Starting sudoku loop: " << iter << " iterations." << endl;
    for (unsigned i = 0; i < iter; ++i) {
        cout << "Sudoku: " << mSudokus[i].getName() << " status: ";
        if (mSudokus[i].solve())
            cout << "solved!" << endl;
        else
            cout << "unsolved. :(" << endl;
        mSudokus[i].print();
    }
}
