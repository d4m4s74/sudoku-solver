#include <vector>
#include <string>
#include <unordered_set>

#ifndef SUDOKU_H
#define SUDOKU_H
class Sudoku
{
    friend class Sudoku_generator;
    std::vector<std::vector<int>> puzzle;
    std::vector<std::vector<std::unordered_set<int>>> allOptions;
    bool solved = false;

    std::vector<int> get_row(int r);
    std::vector<int> get_col(int c);
    std::vector<int> get_block(int r, int c);
    std::vector<int> get_block(int b);   
    void initialize_options();
    std::unordered_set<int> get_options(int r, int c);
    void update_options();
    void update_options(int r, int c);
    std::vector<std::unordered_set<int>> get_options_row(int r);
    std::vector<std::unordered_set<int>> get_options_row_except(int r, int c);
    std::vector<std::unordered_set<int>> get_options_row_except(int r, int c1, int c2);
    std::vector<std::unordered_set<int>> get_options_row_except(int r, int c1, int c2, int c3);
    std::vector<std::unordered_set<int>> get_options_col(int c);
    std::vector<std::unordered_set<int>> get_options_col_except(int r, int c);
    std::vector<std::unordered_set<int>> get_options_col_except(int r1, int r2, int c);
    std::vector<std::unordered_set<int>> get_options_col_except(int r1, int r2, int r3, int c);
    std::vector<std::unordered_set<int>> get_options_block(int r, int c);
    std::vector<std::unordered_set<int>> get_options_block(int b);
    std::vector<std::unordered_set<int>> get_options_block_except(int r, int c);
    std::vector<std::unordered_set<int>> get_options_block_except(int r1, int c1, int r2, int c2);
    std::vector<std::unordered_set<int>> get_options_block_except(int r1, int c1, int r2, int c2, int r3, int c3);
    
    std::unordered_set<int> unordered_set_difference(std::unordered_set<int> left, std::unordered_set<int> right);
    std::unordered_set<int> unordered_set_difference(std::unordered_set<int> left, std::vector<int> right);
    std::unordered_set<int> unordered_set_difference(std::unordered_set<int> left, std::vector<std::unordered_set<int>> right);
    std::unordered_set<int> unordered_set_intersection(std::unordered_set<int> left, std::unordered_set<int> right);
    std::unordered_set<int> unordered_set_3_way_intersection(std::unordered_set<int> left, std::unordered_set<int> middle, std::unordered_set<int> right);
    bool backtrack(int r = 0, int c = 0);   
    bool check_solved_cells();
    bool hidden_singles();
    bool naked_pairs();
    bool naked_triples();
    bool hidden_pairs();
    bool hidden_triples();

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