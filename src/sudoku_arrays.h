#include <vector>
#include <string>
#include <unordered_set>

#ifndef SUDOKU_H
#define SUDOKU_H
class Sudoku
{
    int puzzle[9][9];
    std::unordered_set<int> allOptions[9][9];
    bool solved = false;

    int * get_row(int r);
    int * get_col(int c);
    int * get_block(int r, int c);
    int * get_block(int b);   
    void initialize_options();
    std::unordered_set<int> get_options(int r, int c);
    void update_options();
    void update_options(int r, int c);
    std::unordered_set<int> * get_options_row(int r);
    std::unordered_set<int> * get_options_row_except(int r, int c);
    std::unordered_set<int> * get_options_col(int c);
    std::unordered_set<int> * get_options_col_except(int r, int c);
    std::unordered_set<int> * get_options_block(int r, int c);
    std::unordered_set<int> * get_options_block_except(int r, int c);
    bool bruteforce(int r = 0, int c = 0);   
    bool solve_comparison();

    public:
        int col[9];
        int row[9];
        int block[9];
        std::unordered_set<int> ocol[9];
        std::unordered_set<int> orow[9];
        std::unordered_set<int> oblock[9];
        std::unordered_set<int> oecol[8];
        std::unordered_set<int> oerow[8];
        std::unordered_set<int> oeblock[8];
        void set_puzzle(std::string puzzleString);
        void print_puzzle();
        std::string toString();
        bool is_solved();
        void solve();
        Sudoku();
        Sudoku(std::string puzzleString);
        friend std::ostream& operator << (std::ostream &output, Sudoku& sudoku);
        friend std::istream& operator >> (std::istream &input, Sudoku& sudoku);
};


#endif