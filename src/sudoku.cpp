#include <iostream>
#include <vector>
#include <unordered_set>
#include <cmath>
#include <numeric>
#include <algorithm>
#include "sudoku.h"

//next to add: naked pairs/triples
std::vector<int> Sudoku::get_row(int r)
{
    return puzzle[r];
}

std::vector<int> Sudoku::get_col(int c)
{
    std::vector<int> col;
    for (int r = 0; r < 9; r++)
    {
        col.push_back(puzzle[r][c]);
    }
    return col;
}

std::vector<int> Sudoku::get_block(int r, int c)
{
    std::vector<int> block;
    r = r / 3 * 3; //this gives the top row for the block of the given r. by abusing that int floors numbers ex 5/3 = 1 *3 = 3;
    c = c / 3 * 3;
    for (int y = r; y < r + 3; y++)
    {
        for (int x = c; x < c + 3; x++)
        {
            block.push_back(puzzle[y][x]);
        }
    }
    return block;
}

std::vector<int> Sudoku::get_block(int b) //will get a block by number (left to right, top to bottom, starting with 0. )
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
        std::vector<std::unordered_set<int>> row;
        for (int c = 0; c < 9; c++)
        {
            row.push_back(defaultoptions);
        }
        allOptions.push_back(row);
    }
}
std::unordered_set<int> Sudoku::get_options(int r, int c)
{
    if (puzzle[r][c] != 0)
        return {puzzle[r][c]};
    std::unordered_set<int> options = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    std::vector<int> row = get_row(r);
    std::vector<int> col = get_col(c);
    std::vector<int> block = get_block(r, c);
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

std::vector<std::unordered_set<int>> Sudoku::get_options_row(int r)
{
    return allOptions[r];
}

std::vector<std::unordered_set<int>> Sudoku::get_options_row_except(int r, int c)
{
    std::vector<std::unordered_set<int>> row = Sudoku::get_options_row(r);
    row.erase(row.begin() + c);
    return row;
}

std::vector<std::unordered_set<int>> Sudoku::get_options_col(int c)
{
    std::vector<std::unordered_set<int>> col;
    for (int r = 0; r < 9; r++)
    {
        col.push_back(allOptions[r][c]);
    }
    return col;
}

std::vector<std::unordered_set<int>> Sudoku::get_options_col_except(int r, int c)
{
    std::vector<std::unordered_set<int>> col = get_options_col(c);
    col.erase(col.begin() + r);
    return col;
}

std::vector<std::unordered_set<int>> Sudoku::get_options_block(int r, int c)
{
    std::vector<std::unordered_set<int>> block;
    r = r / 3 * 3; //this gives the top row for the block of the given r. by abusing that int floors numbers ex 5/3 = 1 *3 = 3;
    c = c / 3 * 3;
    for (int y = r; y < r + 3; y++)
    {
        for (int x = c; x < c + 3; x++)
        {
            block.push_back(allOptions[y][x]);
        }
    }
    return block;
}

std::vector<std::unordered_set<int>> Sudoku::get_options_block(int b) //will get a block by number (left to right, top to bottom, starting with 0. )
{
    int r = b / 3 * 3; //see other get_block for explanation Simply said, block 0, 1 and 2 give 0, 3, 4 and 5 give 3, 6, 7 and 8 give 6
    int c = b % 3 * 3; //0, 3, 6 repeating.
    return get_options_block(r, c);
}

std::vector<std::unordered_set<int>> Sudoku::get_options_block_except(int r, int c)
{
    std::vector<std::unordered_set<int>> block = get_options_block(r, c);
    int i = r % 3 * 3 + c % 3;
    block.erase(block.begin() + i);
    return block;
}

bool Sudoku::backtrack(int r, int c)
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
        if (backtrack(r2, c2) == true)
            return true;
        puzzle[r][c] = 0; //return the square to 0;
    }
    return false;
}

std::unordered_set<int> Sudoku::unordered_set_difference(std::unordered_set<int> left, std::unordered_set<int> right)
{
    for (auto item : right)
    {
        left.erase(item);
    }
    return left;
}

std::unordered_set<int> Sudoku::unordered_set_difference(std::unordered_set<int> left, std::vector<std::unordered_set<int>> right)
{
    for (auto set : right)
    {
        for (auto item : set)
        {
            left.erase(item);
        }
    }
    return left;
}

bool Sudoku::check_solved_cells()
{
    std::vector<std::vector<int>> puzzleCopy = puzzle;                             //make working copy
    std::vector<std::vector<std::unordered_set<int>>> allOptionsCopy = allOptions; //make a working copy
    bool found = false;
    for (int r = 0; r < 9; r++)
    {
        for (int c = 0; c < 9; c++)
        {
            if (puzzle[r][c] == 0)
            { //if the square is not yet solved
                if (allOptions[r][c].size() == 1)
                { //and there's only one option
                    found = true;
                    int n = *allOptions[r][c].begin(); //get that option
                    //std::cout << "solved cell " << r << "," << c << "=" << n << std::endl;
                    puzzleCopy[r][c] = n; //put it onto the puzzle
                    //remove it from the squares this square sees
                    for (int i = 0; i < 9; i++)
                    {
                        if (i != c)
                        { //remove n from every place in the row except te piece itself
                            allOptionsCopy[r][i].erase(n);
                        }
                        if (i != r)
                        { //same for the column
                            allOptionsCopy[i][c].erase(n);
                        }
                    }
                    int r0 = r / 3 * 3; //find the top row of the block (abusing int truncation)
                    int c0 = c / 3 * 3; //find left column of the block
                    for (int y = r0; y < r0 + 3; y++)
                    {
                        for (int x = c0; x < c0 + 3; x++)
                        {
                            if (!(r == y and c == x)) //if it isn't the square itself remove n
                            {
                                allOptionsCopy[y][x].erase(n);
                            }
                        }
                    }
                }
            }
        }
    }
    puzzle = puzzleCopy;
    allOptions = allOptionsCopy;
    return found;
}

bool Sudoku::hidden_singles() //currently causes segfault
{
    bool found = false;
    std::vector<std::vector<std::unordered_set<int>>> allOptionsCopy = allOptions; //make a working copy
    for (int r = 0; r < 9; r++)
    {
        for (int c = 0; c < 9; c++)
        {
            if (puzzle[r][c] == 0)
            {                                                                                                            //if the square is not yet solved
                std::unordered_set<int> options = allOptions[r][c];                                                      //get the options
                std::unordered_set<int> uniqueOptions = unordered_set_difference(options, get_options_row_except(r, c)); //find if there's an unique option
                if (uniqueOptions.size() == 1)                                                                           //if so
                {
                    found = true;
                    //std::cout << "found hidden single" << std::endl;
                    allOptionsCopy[r][c] = {*uniqueOptions.begin()}; //replace options with just the number;
                    continue;
                }
                uniqueOptions = unordered_set_difference(options, get_options_col_except(r, c)); //find if there's an unique option
                if (uniqueOptions.size() == 1)                                                   //if so
                {
                    found = true;
                    //std::cout << "found hidden single" << std::endl;
                    allOptionsCopy[r][c] = {*uniqueOptions.begin()}; //replace options with just the number;
                    continue;
                }
                uniqueOptions = unordered_set_difference(options, get_options_block_except(r, c)); //find if there's an unique option
                if (uniqueOptions.size() == 1)                                                     //if so
                {
                    found = true;
                    //std::cout << "found hidden single" << std::endl;
                    allOptionsCopy[r][c] = {*uniqueOptions.begin()}; //replace options with just the number;
                    continue;
                }
            }
        }
    }
    allOptions = allOptionsCopy; //replace allOptions contents with its copy

    return found;
}

bool Sudoku::naked_pairs()
{
    std::vector<std::vector<std::unordered_set<int>>> allOptionsCopy = allOptions;
    bool found = false;
    for (int i = 0; 1 < 9; i++) //doing both rows and cols in one loop
    {
        std::vector<std::unordered_set<int>> optionsRow = get_options_row(i);
        for (int j = 0; j < 8; j++) //only have to check up to the second to last
        {
            if (optionsRow[j].size() == 2){ //if there are two options exactly
                if (std::find(optionsRow.begin()+j+1,optionsRow.end(),optionsRow[j]) != optionsRow.end()) //if you find a second set with the exact same options
                {
                    int first = j; //index of the first pair, to make it easier to read
                    int second = std::find(optionsRow.begin()+j+1,optionsRow.end(),optionsRow[j]) - optionsRow.begin(); //index of the second
                    int r = i; //to make things easier to read, r for the row of the pair.
                    for(int c = 0; c < 9; c++)
                    {
                        if (c != first and c != second)
                        {
                            allOptionsCopy[r][c] = unordered_set_difference(allOptions[r][c],allOptions[r][first]); //delete numbers in the pair from the rest of the row
                            if (!found and allOptionsCopy[r][c] != allOptions[r][c]) found = true; //if this had any effect set found to true
                        }
                    }
                }
            }
        }
        std::vector<std::unordered_set<int>> optionsCol = get_options_col(i);
        for (int j = 0; j < 8; j++) //only have to check up to the second to last
        {
            if (optionsCol[j].size() == 2){ //if there are two options exactly
                if (std::find(optionsCol.begin()+j+1,optionsCol.end(),optionsCol[j]) != optionsCol.end()) //if you find a second set with the exact same options
                {
                    int first = j; //index of the first pair, to make it easier to read
                    int second = std::find(optionsCol.begin()+j+1,optionsCol.end(),optionsCol[j]) - optionsCol.begin(); //index of the second
                    int c = i; //to make things easier to read, c for the column of the pair.
                    for(int r = 0; r < 9; r++)
                    {
                        if (r != first and r != second)
                        {
                            allOptionsCopy[r][c] = unordered_set_difference(allOptions[r][c],allOptions[first][c]); //delete numbers in the pair from the rest of the row
                            if (!found and allOptionsCopy[r][c] != allOptions[r][c]) found = true; //if this had any effect set found to true
                        }
                    }
                }
            }
        }
        std::vector<std::unordered_set<int>> optionsBlock = get_options_block(i);
        for (int j = 0; j < 8; j++) //only have to check up to the second to last
        {
            if (optionsBlock[j].size() == 2){ //if there are two options exactly
                if (std::find(optionsBlock.begin()+j+1,optionsBlock.end(),optionsBlock[j]) != optionsBlock.end()) //if you find a second set with the exact same options
                {
                    int first = j; //index of the first pair, to make it easier to read
                    int second = std::find(optionsBlock.begin()+j+1,optionsBlock.end(),optionsBlock[j]) - optionsBlock.begin(); //index of the second
                    int r0 = i / 3 * 3; 
                    int c0 = i % 3 * 3;
                    int r1 = r0+first/3;
                    int c1 = c0+first%3;
                    int r2 = r0+second/3;
                    int c2 = c0+second%3;
                    for(int r = r0; r < r0+3; r++)
                    {
                        for (int c = c0; c < c0+3; c++)
                        {
                            if (!(r == r1 and c == c1) and !(r == r2 and c == c2))
                            {
                                allOptionsCopy[r][c] = unordered_set_difference(allOptions[r][c],allOptions[r][first]); //delete numbers in the pair from the rest of the row
                                if (!found and allOptionsCopy[r][c] != allOptions[r][c]) found = true; //if this had any effect set found to true
                            }
                        }
                    }
                }
            }
        }
    }
    allOptions = allOptionsCopy; //apply changes
    return found;
}

std::vector<std::vector<int>> Sudoku::get_puzzle()
{
    return puzzle;
}

void Sudoku::set_puzzle(std::vector<std::vector<int>> puzzle)
{
    solved = false; //because we don't know if the new puzzle is solved, set it to false.
    this->puzzle = puzzle;
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
            row.push_back((int)puzzleString[r * 9 + c] - '0'); //converts char to int and puts it in the row
        }
        newPuzzle.push_back(row);
    }
    puzzle = newPuzzle;
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
            std::vector<int> row = get_row(i);
            std::vector<int> col = get_col(i);
            std::vector<int> block = get_block(i);
            if (std::accumulate(row.begin(), row.end(), 0) != 45)
                return false;
            if (std::accumulate(col.begin(), col.end(), 0) != 45)
                return false;
            if (std::accumulate(block.begin(), block.end(), 0) != 45)
                return false;
        }
        solved = true;
    }
    return solved;
}

void Sudoku::solve()
{
    //first get all possibilities for each cell. This only happens once
    update_options();
    bool changed;
    do
    {
        changed = false;
        if (check_solved_cells())
        {
            changed = true;
            //print_puzzle(); //copy of puzzle for debugging
            continue; //if anything is found, go back to the beginning to check again
        }
        if (hidden_singles())
        {
            changed = true;
            continue;
        }
        //if (naked_pairs())
        //{
        //    changed = true;
        //    continue;
        //}

    } while (changed == true);
    if (!is_solved())
    {
        std::cout << "not solved using implemented checks. Resorting to backtracking";
        backtrack();
    }
}

Sudoku::Sudoku()
{
    initialize_options();
}

Sudoku::Sudoku(std::vector<std::vector<int>> puzzle)
{
    initialize_options();
    this->puzzle = puzzle;
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