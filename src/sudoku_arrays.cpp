#include <iostream>
#include <vector>
#include <unordered_set>
#include <cmath>
#include <numeric>
#include <algorithm>
#include "sudoku_arrays.h"

int * Sudoku::get_row(int r)
{
    return puzzle[r];
}

int * Sudoku::get_col(int c)
{
    for (int r = 0; r < 9; r++)
    {
        col[r] = puzzle[r][c];
    }
    return col;
}

int * Sudoku::get_block(int r, int c)
{
    r = r / 3 * 3; //this gives the top row for the block of the given r. by abusing that int floors numbers ex 5/3 = 1 *3 = 3;
    c = c / 3 * 3;
    for (int y = r; y < r + 3; y++)
    {
        for (int x = c; x < c + 3; x++)
        {
            block[y*3+x]=puzzle[y][x];
        }
    }
    return block;
}

int * Sudoku::get_block(int b) //will get a block by number (left to right, top to bottom, starting with 0. )
{
    int r = b / 3 * 3; //see other get_block for explanation Simply said, block 0, 1 and 2 give 0, 3, 4 and 5 give 3, 6, 7 and 8 give 6
    int c = b % 3 * 3; //0, 3, 6 repeating.
    return get_block(r, c);
}

void Sudoku::initialize_options()
{
    for (int r = 0; r < 9; r++) //initialize options
    {
        std::unordered_set<int> defaultoptions = {1, 2, 3, 4, 5, 6, 7, 8, 9};
        std::unordered_set<int> row[9];
        for (int c = 0; c < 9; c++)
        {
            allOptions[r][c] = defaultoptions;
        }
    }
}
std::unordered_set<int> Sudoku::get_options(int r, int c)
{
    if (puzzle[r][c] != 0)
        return {puzzle[r][c]};
    std::unordered_set<int> options = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    int * row = get_row(r);
    int * col = get_col(c);
    int * block = get_block(r, c);
    for (int i = 0; i < 9; i++)
    {
        options.erase(row[i]);
        options.erase(col[i]);
        options.erase(block[i]);
    }
    return options;
}

void Sudoku::update_options() //updates all options
{
    for (int r = 0; r < 9; r++)
    {
        for (int c = 0; c < 9; c++)
        {
            allOptions[r][c] = get_options(r, c);
        }
    }
}

void Sudoku::update_options(int r, int c) //updates just the options affected by a certain square.
{
    for (int i = 0; i < 9; i++) //update the row and collumn
    {
        allOptions[r][i] = get_options(r, i);
        allOptions[i][c] = get_options(i, c);
    }
    r = r / 3 * 3; //change r and c to the top left square of the block
    c = c / 3 * 3;
    for (int y = r; y < r + 3; y++)
    {
        for (int x = c; x < c + 3; x++)
        {
            allOptions[y][x] = get_options(y, x);
        }
    }
}

std::unordered_set<int> * Sudoku::get_options_row(int r)
{
    return allOptions[r];
}

std::unordered_set<int> * Sudoku::get_options_row_except(int r, int c)
{
    std::unordered_set<int> * options = Sudoku::get_options_row(r);
    int j = 0;
    for (int i = 0; i < 9; i++){
        if (i != c)
        {
            oerow[j] = options[i];
            j++;
        }
    }
    return oerow;
}

std::unordered_set<int> * Sudoku::get_options_col(int c)
{
    
    for (int r = 0; r < 9; r++)
    {
        ocol[r] = allOptions[r][c];
    }
    return ocol;
}

std::unordered_set<int> * Sudoku::get_options_col_except(int r, int c)
{
    
    int i = 0;
    for (int y = 0; y < 9; y++)
    {
        if (y != r)
        {
        oecol[i] = allOptions[y][c];
        i++;
        }
    }
    return oecol;
}

std::unordered_set<int> * Sudoku::get_options_block(int r, int c)
{
    r = r / 3 * 3; //this gives the top row for the block of the given r. by abusing that int floors numbers ex 5/3 = 1 *3 = 3;
    c = c / 3 * 3;
    int i = 0;
    for (int y = r; y < r + 3; y++)
    {
        for (int x = c; x < c + 3; x++)
        {
            oblock[i] = allOptions[y][x];
            i++;
        }
    }
    return oblock;
}

std::unordered_set<int> * Sudoku::get_options_block_except(int r, int c)
{
    int r0 = r / 3 * 3; //this gives the top row for the block of the given r. by abusing that int floors numbers ex 5/3 = 1 *3 = 3;
    int c0 = c / 3 * 3;
    int i = 0;
    for (int y = r0; y < r0 + 3; y++)
    {
        for (int x = c0; x < c0 + 3; x++)
        {
            if (r != y and c != x){
            oeblock[i] = allOptions[y][x];
            i++;
            }
        }
    }
    return oeblock;
}

bool Sudoku::bruteforce(int r, int c)
{
    if (r == 9)
        return true; //returns true if you've finished the last row
    int r2, c2;

    while (puzzle[r][c] != 0) //if the square is already set, try next ones until you find a 0
    {
        if (c == 8) //calculate the next piece
        {
            c = 0;
            r++;
        }
        else
            c++;
        if (r > 8)
            return true; //if you find the end of the puzzle before a zero, return true.
    }
    if (c == 8) //calculate the next piece
    {
        c2 = 0;
        r2 = r + 1;
    }
    else
    {
        r2 = r;
        c2 = c + 1;
    }
    std::unordered_set<int> options = get_options(r, c);
    if (options.size() == 0)
        return false; //if there are no options something must have gone wrong before. return false
    for (int o : options)
    {
        puzzle[r][c] = o; //set the square to one of the options
        if (bruteforce(r2, c2) == true)
            return true;
        puzzle[r][c] = 0; //return the square to 0;
    }
    return false;
}

bool Sudoku::solve_comparison()
{
    update_options();
    bool changed;
    do
    {
        changed = false;
        for (int r = 0; r < 9; r++)
        {
            for (int c = 0; c < 9; c++)
            {
                if (puzzle[r][c] != 0)
                    continue; //go to the next one if this square is solved
                std::unordered_set<int> options = allOptions[r][c];
                if (options.size() == 1)
                { //if there is only one option no comparison is needed
                    puzzle[r][c] = *begin(options);
                    update_options(r, c);
                    changed = true;
                    continue;
                }
                std::unordered_set<int> uniqueOptions = options;
                std::unordered_set<int> * optionsRow = get_options_row_except(r, c);
                for (int i = 0; i<9;i++)
                {
                    for (auto o : optionsRow[i])
                    {
                        uniqueOptions.erase(o);
                    }
                }
                if (uniqueOptions.size() == 1)
                {
                    puzzle[r][c] = *begin(uniqueOptions);
                    update_options(r, c);
                    changed = true;
                    continue;
                }

                uniqueOptions = options;
                std::unordered_set<int> * optionsCol = get_options_col_except(r, c);
                for (int i = 0; i<9;i++)
                {
                    for (auto o : optionsCol[i])
                    {
                        uniqueOptions.erase(o);
                    }
                }
                if (uniqueOptions.size() == 1)
                {
                    puzzle[r][c] = *begin(uniqueOptions);
                    update_options(r, c);
                    changed = true;
                    continue;
                }

                uniqueOptions = options;
                std::unordered_set<int> * optionsBlock = get_options_block_except(r, c);
                for (int i = 0; i<9;i++)
                {
                    for (auto o : optionsBlock[i])
                    {
                        uniqueOptions.erase(o);
                    }
                }
                if (uniqueOptions.size() == 1)
                {
                    puzzle[r][c] = *begin(uniqueOptions);
                    update_options(r, c);
                    changed = true;
                    continue;
                }
            }
        }
    } while (changed == true);
    return is_solved();
}



void Sudoku::set_puzzle(std::string puzzleString)
{
    solved = false; //because we don't know if the new puzzle is solved, set it to false.
    std::vector<std::vector<int>> newPuzzle;
    for (int r = 0; r < 9; r++)
    {
        std::vector<int> row;
        for (int c = 0; c < 9; c++)
        {
            puzzle[r][c] = (int)puzzleString[r * 9 + c] - '0'; //converts char to int and puts it in the row
        }
    }
}

void Sudoku::print_puzzle()
{
    for (int r = 0; r < 9; r++)
    {
        if (r % 3 == 0 and r != 0)
            std::cout << "---------------------" << std::endl;
        for (int c = 0; c < 9; c++)
        {
            if (c % 3 == 0 and c != 0)
                std::cout << "| ";
            std::cout << puzzle[r][c] << " ";
        }
        std::cout << std::endl;
    }
}

std::string Sudoku::toString()
{
    std::string puzzle_string;
    for (int r = 0; r < 9; r++)
    {
        for (int c = 0; c < 9; c++)
        {
            puzzle_string += std::to_string(puzzle[r][c]);
        }
    }
    return puzzle_string;
}

bool Sudoku::is_solved()
{
    if (solved == false)
    { //if solved is set to false, check if the puzzle is solved before returning solved
        for (int i = 0; i < 9; i++)
        {
            int * row = get_row(i);
            int * col = get_col(i);
            int * block = get_block(i);
            if (std::accumulate(row, row+9, 0) != 45)
                return false;
            if (std::accumulate(col, col+9, 0) != 45)
                return false;
            if (std::accumulate(block, block+9, 0) != 45)
                return false;
        }
        solved = true;
    }
    return solved;
}

void Sudoku::solve()
{
    if (solve_comparison())
        return;
    //std::cout << "Solve failed, brute forcing" << std::endl;
    bruteforce();
}

Sudoku::Sudoku()
{
    initialize_options();
}


Sudoku::Sudoku(std::string puzzleString)
{
    initialize_options();
    set_puzzle(puzzleString);
}

std::ostream &operator<<(std::ostream &output, Sudoku &sudoku)
{
    output << sudoku.toString();
    return output;
}

std::istream &operator>>(std::istream &input, Sudoku &sudoku)
{
    std::string puzzleString;
    input >> puzzleString;
    sudoku.set_puzzle(puzzleString);
    return input;
}