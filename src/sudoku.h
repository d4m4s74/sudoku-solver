#include <vector>
#include <string>
#include <unordered_set>

#ifndef SUDOKU_H
#define SUDOKU_H
class Sudoku
{
    std::vector<std::vector<int>> puzzle;
    std::vector<std::vector<std::unordered_set<int>>> allOptions;
    bool solved = false;

    std::vector<int> get_row(int r);
    std::vector<int> get_col(int c);
    std::vector<int> get_block(int r, int c);
    std::vector<int> get_block(int b);   
    std::unordered_set<int> get_options(int r, int c);
    void update_options();
    void update_options(int r, int c);
    std::vector<std::unordered_set<int>> get_options_row(int r);
    std::vector<std::unordered_set<int>> get_options_row_except(int r, int c);
    std::vector<std::unordered_set<int>> get_options_col(int c);
    std::vector<std::unordered_set<int>> get_options_col_except(int r, int c);
    std::vector<std::unordered_set<int>> get_options_block(int r, int c);
    std::vector<std::unordered_set<int>> get_options_block_except(int r, int c);
    bool bruteforce(int r = 0, int c = 0);   
    bool solve_comparison();

    public:
        std::vector<std::vector<int>> get_puzzle();
        void set_puzzle(std::vector<std::vector<int>> puzzle);
        void set_puzzle(std::string puzzleString);
        void print_puzzle();
        std::string toString();
        bool is_solved();
        void solve();
        Sudoku();
        Sudoku(std::vector<std::vector<int>> puzzle);
        Sudoku(std::string puzzleString);
        friend std::ostream& operator << (std::ostream &output, Sudoku& sudoku);
        friend std::istream& operator >> (std::istream &input, Sudoku& sudoku);
};


#endif