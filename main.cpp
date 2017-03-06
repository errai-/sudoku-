#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include <fstream>

using std::cout;
using std::endl;
using std::vector;
using std::string;
using std::exception;
using std::ifstream;

#include "SudokuHandle.h"

int main(void) {
    cout << "Starting the sudoku solver." << endl;
    
    try {
        SudokuHandle handle;
        if (!handle.add("p096_sudoku.txt"))
            throw std::runtime_error("Input file functioning oddly!");
        handle.loop(3);
        return 0;
    } catch(exception &e) {
        cout << endl << endl << "An error occurred: " << e.what() << endl;
        return 1;
    }
}
