#include <iostream>
#include <fstream>
#include <array>

#define SUDOKU_DIM 9

inline void printArr(const std::array<std::array<char, SUDOKU_DIM>, SUDOKU_DIM> &arr)
{
    for (int i = 0; i < 9; ++i) {
        for (int k = 0; k < 9; ++k) {
            std::cout << (arr[i][k] == '0' ? '.' : (arr[i][k])) << " ";
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

inline bool checkValid(std::array<std::array<char, SUDOKU_DIM>, SUDOKU_DIM> &arr, int x, int y)
{
    int added = arr[x][y];
    for (int a = 0; a < 9; ++a) {
        if (a != x && arr[a][y] == added)
            return false;
        if (a != y && arr[x][a] == added)
            return false;
    }
    for (int a = x - x%3, i = 0; i < 3; ++i, ++a) {
        for (int b = y - y%3, k = 0; k < 3; ++k, ++b) {
            if (a != x && b != y && arr[a][b] == added)
                return false;
        }
    }
    return true;
}

bool bruteSudoku(std::array<std::array<char, SUDOKU_DIM>, SUDOKU_DIM> &arr, int x, int y)
{
    for (; x < SUDOKU_DIM; ++x) {
        for (; y < SUDOKU_DIM; ++y) {
            if (arr[x][y] < '1' || arr[x][y] > '9') {
                for (int i = '1'; i <= '9'; ++i) {
                    arr[x][y] = i;
                    if (checkValid(arr, x, y)) {
                        if (bruteSudoku(arr, x, y)) {
                            return true;
                        }
                    }
                    arr[x][y] = '0';
                }
                return false;
            } else if (!checkValid(arr, x, y)) {
                return false;
            }
        }
        y = 0;
    }
    return true;
}

void solveSudoku(const std::string &inSudoku, std::ostream &ostrm)
{
    if (inSudoku.length() != SUDOKU_DIM * SUDOKU_DIM) {
        ostrm << "\n";
        return;
    }

    std::array<std::array<char, SUDOKU_DIM>, SUDOKU_DIM> arr;
    auto sudIt = inSudoku.begin();
    for (int i = 0; i < SUDOKU_DIM; ++i) {
        for (int k = 0; k < SUDOKU_DIM; ++k, ++sudIt) {
            arr[i][k] = *sudIt;
        }
    }
    if(bruteSudoku(arr, 0, 0)) {
        for (int i = 0; i < SUDOKU_DIM; ++i) {
            for (int k = 0; k < SUDOKU_DIM; ++k) {
                ostrm << arr[i][k];
            }
        }
    }
    ostrm << "\n";
    ostrm.flush();
}

int main(int argc, char **argv) {
    if (argc % 2 == 0) {
        std::cerr << "Wrong number of arguments!" << std::endl;
        return EXIT_FAILURE;
    }
    std::ifstream ifs;
    std::ofstream ofs;

    for (int i = 1; i < argc; i += 2) {
        std::string arg(argv[i]);
        if (arg == "-i") {
            ifs.open(argv[i+1], std::fstream::binary | std::fstream::in);
            if (!ifs.is_open()) {
                std::cerr << "couldn't open input file!" << std::endl;
                return EXIT_FAILURE;
            }
        }
        else if (arg == "-o") {
            ofs.open(argv[i+1], std::fstream::binary | std::fstream::out);
            if (!ofs.is_open()) {
                std::cerr << "couldn't open output file!" << std::endl;
                return EXIT_FAILURE;
            }
        }
        else {
            std::cout << "unhandled argument: " << arg << "\n";
            return EXIT_FAILURE;
        }
    }
    std::istream &istrm = ifs.is_open() ? ifs : std::cin;
    std::ostream &ostrm = ofs.is_open() ? ofs : std::cout;

    std::string inSudoku;
    while (std::getline(istrm, inSudoku)) {
        solveSudoku(inSudoku, ostrm);
    }

    return EXIT_SUCCESS;
}