#include <iostream>
#include <vector>
#include <unordered_set>
#include <cmath>
#include <numeric>
#include <algorithm>
#include "sudoku.h"
#include <functional>
#include <utility>

int Sudoku::block_number(int r, int c)
{
    return r / 3 * 3 + c / 3;
}

bool Sudoku::seen_by(int r1, int c1, int r2, int c2)
{
    return (r1 == r2 or c1 == c2 or block_number(r1, c1) == block_number(r2, c2));
}

std::vector<std::pair<int, int>> Sudoku::list_seen_by(int r, int c) //returns a list of squares seen by both squares
{
    std::vector<std::pair<int, int>> output;
    for (int i = 0; i < 9; i++)
    {
        if (i != r)
            output.push_back({i, c});
        if (i != c)
            output.push_back({r, i});
    }
    int r0 = r / 3 * 3;
    int c0 = c / 3 * 3;
    for (int i = r0; i < r0 + 3; i++)
    {
        for (int j = c0; j < c0 + 3; j++)
        {
            if (i != r and j != c)
                output.push_back({i, j});
        }
    }
    return output;
}

std::vector<std::pair<int, int>> Sudoku::list_seen_by(std::vector<std::pair<int, int>> checklist, int r, int c) //returns a list of squares within the given list the given square
{
    std::vector<std::pair<int, int>> output;
    for (std::pair<int, int> rc : checklist)
    {
        if (seen_by(rc.first, rc.second, r, c) and !(rc.first == r and rc.second == c))
            output.push_back(rc);
    }
    return output;
}

std::vector<std::pair<int, int>> Sudoku::list_seen_by(int r1, int c1, int r2, int c2) //returns a list of squares seen by both squares
{
    return list_seen_by(list_seen_by(r1, c1), r2, c2);
}

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
    initialized = true;
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

std::vector<std::unordered_set<int>> Sudoku::get_options_row_except(int r, int c1, int c2)
{
    if (c2 > c1)
    {
        c2--;
    }

    std::vector<std::unordered_set<int>> row = Sudoku::get_options_row(r);
    row.erase(row.begin() + c1);
    row.erase(row.begin() + c2);
    return row;
}

std::vector<std::unordered_set<int>> Sudoku::get_options_row_except(int r, int c1, int c2, int c3)
{
    if (c2 > c1)
    {
        c2--;
    }
    if (c3 > c1)
    {
        c3--;
    }
    if (c3 > c2)
    {
        c3--;
    }

    std::vector<std::unordered_set<int>> row = Sudoku::get_options_row(r);
    row.erase(row.begin() + c1);
    row.erase(row.begin() + c2);
    row.erase(row.begin() + c3);
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
    std::vector<std::unordered_set<int>> row = Sudoku::get_options_col(c);
    row.erase(row.begin() + r);
    return row;
}

std::vector<std::unordered_set<int>> Sudoku::get_options_col_except(int r1, int r2, int c)
{
    if (r2 > r1)
    {
        r2--;
    }
    std::vector<std::unordered_set<int>> col = get_options_col(c);
    col.erase(col.begin() + r1);
    col.erase(col.begin() + r2);
    return col;
}

std::vector<std::unordered_set<int>> Sudoku::get_options_col_except(int r1, int r2, int r3, int c)
{
    if (r2 > r1)
    {
        r2--;
    }
    if (r3 > r1)
    {
        r3--;
    }
    if (r3 > r2)
    {
        r3--;
    }
    std::vector<std::unordered_set<int>> col = get_options_col(c);
    col.erase(col.begin() + r1);
    col.erase(col.begin() + r2);
    col.erase(col.begin() + r3);
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

std::vector<std::unordered_set<int>> Sudoku::get_options_block_except(int r1, int c1, int r2, int c2)
{
    std::vector<std::unordered_set<int>> block = get_options_block(r1, c1);
    int i1 = r1 % 3 * 3 + c1 % 3;
    int i2 = r2 % 3 * 3 + c2 % 3;
    if (i2 > i1)
    {
        i2--;
    }
    block.erase(block.begin() + i1);
    block.erase(block.begin() + i2);
    return block;
}

std::vector<std::unordered_set<int>> Sudoku::get_options_block_except(int r1, int c1, int r2, int c2, int r3, int c3)
{
    std::vector<std::unordered_set<int>> block = get_options_block(r1, c1);
    int i1 = r1 % 3 * 3 + c1 % 3;
    int i2 = r2 % 3 * 3 + c2 % 3;
    int i3 = r3 % 3 * 3 + c3 % 3;
    if (i2 > i1)
    {
        i2--;
    }
    if (i3 > i1)
    {
        i3--;
    }
    if (i3 > i2)
    {
        i3--;
    }
    block.erase(block.begin() + i1);
    block.erase(block.begin() + i2);
    block.erase(block.begin() + i3);
    return block;
}

std::vector<std::vector<bool>> Sudoku::get_possible_locations(int n)
{
    std::vector<std::vector<bool>> output;
    for (int r = 0; r < 9; r++)
    {
        std::vector<bool> row;
        for (int c = 0; c < 9; c++)
        {
            row.push_back((puzzle[r][c] == 0 and allOptions[r][c].count(n) == 1));
        }
        output.push_back(row);
    }
    return output;
}

std::vector<bool> Sudoku::get_possible_locations_row(int n, int r)
{
    std::vector<bool> row;
    for (int c = 0; c < 9; c++)
    {
        row.push_back((puzzle[r][c] == 0 and allOptions[r][c].count(n) == 1));
    }
    return row;
}

std::vector<bool> Sudoku::get_possible_locations_col(int n, int c)
{
    std::vector<bool> col;
    for (int r = 0; r < 9; r++)
    {
        col.push_back((puzzle[r][c] == 0 and allOptions[r][c].count(n) == 1));
    }
    return col;
}

std::vector<bool> Sudoku::get_possible_locations_block(int n, int r, int c)
{
    std::vector<bool> block;
    r = r / 3 * 3; //this gives the top row for the block of the given r. by abusing that int floors numbers ex 5/3 = 1 *3 = 3;
    c = c / 3 * 3;
    for (int y = r; y < r + 3; y++)
    {
        for (int x = c; x < c + 3; x++)
        {
            block.push_back((puzzle[y][x] == 0 and allOptions[y][x].count(n) == 1));
        }
    }
    return block;
}

std::vector<std::vector<int>> Sudoku::get_options_count()
{
    std::vector<std::vector<int>> output;
    for (int r = 0; r < 9; r++)
    {
        std::vector<int> row;
        for (int c = 0; c < 9; c++)
        {
            row.push_back((allOptions[r][c].size()));
        }
        output.push_back(row);
    }
    return output;
}

bool Sudoku::backtrack()
{
    if (!initialized)
        update_options();
    int r = 10; //default values the loop can never reach
    int c = 10; //default values the loop can never reach
    int minsize = 10;
    for (int y = 0; y < 9 and minsize > 1; y++)
    {
        for (int x = 0; x < 9 and minsize > 1; x++)
        {
            if (puzzle[y][x] != 0)
                continue;
            int size = allOptions[y][x].size();
            if (size == 0)
                return false;
            else if (size == 1)
            {
                minsize = 1;
                r = y;
                c = x;
            }
            else if (size < minsize)
            {
                minsize = size;
                r = y;
                c = x;
            }
        }
    }
    if (r == 10)
    {
        return is_solved();
    }
    std::unordered_set<int> options = allOptions[r][c];
    for (int o : options)
    {
        puzzle[r][c] = o;
        update_options(r, c);
        if (backtrack() == true)
        {
            return true;
        }
        puzzle[r][c] = 0;
        update_options(r, c);
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

std::unordered_set<int> Sudoku::unordered_set_difference(std::unordered_set<int> left, std::vector<int> right)
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

std::unordered_set<int> Sudoku::unordered_set_intersection(std::unordered_set<int> left, std::unordered_set<int> right)
{
    std::unordered_set<int> result;
    for (auto item : left)
    {
        if (right.count(item) == 1)
        {
            result.emplace(item);
        }
    }
    return result;
}

std::unordered_set<int> Sudoku::unordered_set_3_way_intersection(std::unordered_set<int> left, std::unordered_set<int> middle, std::unordered_set<int> right)
{ //gets a set of items that are in at least 2 out of 3 sets
    std::unordered_set<int> result;
    for (auto item : left)
    {
        if (middle.count(item) == 1 or right.count(item) == 1)
        {
            result.emplace(item);
        }
    }
    for (auto item : middle)
    {
        if (left.count(item) == 1 or right.count(item) == 1)
        {
            result.emplace(item);
        }
    }
    for (auto item : right)
    {
        if (left.count(item) == 1 or middle.count(item) == 1)
        {
            result.emplace(item);
        }
    }
    return result;
}

template <typename t>
void Sudoku::transpose_matrix(std::vector<std::vector<t>> &input) //a simple function to switch X and Y in a 2d vector
{
    std::vector<std::vector<t>> output;
    int maxr = input.size();
    int maxc = input[0].size();
    for (int r = 0; r < maxr; r++)
    {
        std::vector<t> row;
        for (int c = 0; c < maxc; c++)
        {
            row.push_back(input[c][r]);
        }
        output.push_back(row);
    }
    input = output;
}

template <typename t>
std::vector<t> Sudoku::get_col(const std::vector<std::vector<t>> &input, int c) //a generic version of get_col
{
    std::vector<t> col;
    for (int r = 0; r < input.size(); r++)
    {
        col.push_back(input[r][c]);
    }
    return col;
}

template <typename t>
std::vector<t> Sudoku::get_block(const std::vector<std::vector<t>> &input, int r, int c) //a sort-of generic version of get_block (it is specific for 9*9 grids)
{
    std::vector<t> block;
    r = r / 3 * 3; //this gives the top row for the block of the given r. by abusing that int floors numbers ex 5/3 = 1 *3 = 3;
    c = c / 3 * 3;
    for (int y = r; y < r + 3; y++)
    {
        for (int x = c; x < c + 3; x++)
        {
            block.push_back(input[y][x]);
        }
    }
    return block;
}

template <typename t>
std::vector<t> Sudoku::get_block(const std::vector<std::vector<t>> &input, int b) //a sort-of generic version of get_block (it is specific for 9*9 grids)
{
    int r = b / 3 * 3; //see other get_block for explanation Simply said, block 0, 1 and 2 give 0, 3, 4 and 5 give 3, 6, 7 and 8 give 6
    int c = b % 3 * 3; //0, 3, 6 repeating.
    return get_block(input, r, c);
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
    for (int i = 0; i < 9; i++) //doing both rows and cols in one loop
    {
        std::vector<std::unordered_set<int>> optionsRow = get_options_row(i);
        for (int j = 0; j < 8; j++) //only have to check up to the second to last
        {
            if (optionsRow[j].size() == 2)
            {                                                                                                   //if there are two options exactly
                if (std::find(optionsRow.begin() + j + 1, optionsRow.end(), optionsRow[j]) != optionsRow.end()) //if you find a second set with the exact same options
                {
                    int first = j;                                                                                            //index of the first pair, to make it easier to read
                    int second = std::find(optionsRow.begin() + j + 1, optionsRow.end(), optionsRow[j]) - optionsRow.begin(); //index of the second
                    int r = i;                                                                                                //to make things easier to read, r for the row of the pair.
                    for (int c = 0; c < 9; c++)
                    {
                        if (c != first and c != second)
                        {
                            allOptionsCopy[r][c] = unordered_set_difference(allOptions[r][c], allOptions[r][first]); //delete numbers in the pair from the rest of the row
                            if (found == false and allOptionsCopy[r][c] != allOptions[r][c])
                            {
                                found = true; //if this had any effect set found to true
                                //std::cout << "found naked pair: r: " << i << ", c: " << first << " and " << second << std::endl;
                            }
                        }
                    }
                }
            }
        }
        std::vector<std::unordered_set<int>> optionsCol = get_options_col(i);
        for (int j = 0; j < 8; j++) //only have to check up to the second to last
        {
            if (optionsCol[j].size() == 2)
            {                                                                                                   //if there are two options exactly
                if (std::find(optionsCol.begin() + j + 1, optionsCol.end(), optionsCol[j]) != optionsCol.end()) //if you find a second set with the exact same options
                {
                    int first = j;                                                                                            //index of the first pair, to make it easier to read
                    int second = std::find(optionsCol.begin() + j + 1, optionsCol.end(), optionsCol[j]) - optionsCol.begin(); //index of the second
                    int c = i;                                                                                                //to make things easier to read, c for the column of the pair.
                    for (int r = 0; r < 9; r++)
                    {
                        if (r != first and r != second)
                        {
                            allOptionsCopy[r][c] = unordered_set_difference(allOptions[r][c], allOptions[first][c]); //delete numbers in the pair from the rest of the row
                            if (found == false and allOptionsCopy[r][c] != allOptions[r][c])
                            {
                                found = true; //if this had any effect set found to true
                                //std::cout << "found naked pair: c: " << i << ", r: " << first << " and " << second <<  std::endl;
                            }
                        }
                    }
                }
            }
        }
        std::vector<std::unordered_set<int>> optionsBlock = get_options_block(i);
        for (int j = 0; j < 8; j++) //only have to check up to the second to last
        {
            if (optionsBlock[j].size() == 2)
            {                                                                                                           //if there are two options exactly
                if (std::find(optionsBlock.begin() + j + 1, optionsBlock.end(), optionsBlock[j]) != optionsBlock.end()) //if you find a second set with the exact same options
                {
                    int first = j;                                                                                                    //index of the first pair, to make it easier to read
                    int second = std::find(optionsBlock.begin() + j + 1, optionsBlock.end(), optionsBlock[j]) - optionsBlock.begin(); //index of the second
                    int r0 = i / 3 * 3;
                    int c0 = i % 3 * 3;
                    int r1 = r0 + first / 3;
                    int c1 = c0 + first % 3;
                    int r2 = r0 + second / 3;
                    int c2 = c0 + second % 3;
                    for (int r = r0; r < r0 + 3; r++)
                    {
                        for (int c = c0; c < c0 + 3; c++)
                        {
                            if (!(r == r1 and c == c1) and !(r == r2 and c == c2))
                            {
                                allOptionsCopy[r][c] = unordered_set_difference(allOptions[r][c], allOptions[r1][c1]); //delete numbers in the pair from the rest of the row
                                if (found == false and allOptionsCopy[r][c] != allOptions[r][c])
                                    found = true; //if this had any effect set found to true
                            }
                        }
                    }
                }
            }
        }
    }

    if (found)
    { //because there's somehow false positives an extra check
        if (allOptions != allOptionsCopy)
        {
            allOptions = allOptionsCopy; //apply changes
            return true;
        }
    }
    return false;
}

bool Sudoku::naked_triples()
{
    std::vector<std::vector<std::unordered_set<int>>> allOptionsCopy = allOptions;
    bool found = false;
    for (int i = 0; i < 9; i++)
    {
        std::vector<std::unordered_set<int>> optionsRow = get_options_row(i);
        std::vector<std::unordered_set<int>> optionsCol = get_options_col(i);
        std::vector<std::unordered_set<int>> optionsBlock = get_options_block(i);
        std::unordered_set<int> ignore;
        for (int j = 0; j < 9; j++) //have to go to the last one this time
        {
            //start with rows
            if (optionsRow[j].size() == 3 and ignore.count(j) == 0) //easiest case, if there's a triple
            {
                int count = 1;
                std::vector<int> cs; //vector with the col numbers/x coordinates of the members of the triple;
                cs.push_back(j);
                std::vector<std::unordered_set<int>> subsets;
                std::vector<int> base;
                for (int n : optionsRow[j])
                    base.push_back(n); //moving the set into a vector because it's easier to work with individual values
                subsets.push_back(optionsRow[j]);
                subsets.push_back({base[0], base[1]});
                subsets.push_back({base[0], base[2]});
                subsets.push_back({base[1], base[2]});
                for (int k; k < 9; k++)
                {
                    if (k != j and ignore.count(k) == 0 and (std::find(subsets.begin(), subsets.end(), optionsRow[k]) != subsets.end()))
                    {
                        count++;
                        cs.push_back(k);
                    }
                }
                if (count == 3) //we found a naked triple!
                {
                    ignore.emplace(cs[0]); //we don't have to check these again this loop
                    ignore.emplace(cs[1]);
                    ignore.emplace(cs[2]);
                    int r = i; //just to make things a bit more readable
                    for (int c = 0; c < 9; c++)
                    {
                        if (std::find(cs.begin(), cs.end(), c) == cs.end()) //if the c coordinate is not in the cs vector
                        {
                            allOptionsCopy[r][c] = unordered_set_difference(allOptions[r][c], base);
                            if (found == false and allOptionsCopy[r][c] != allOptions[r][c]) //if there is any change
                            {
                                found = true;
                            }
                        }
                    }
                }
            }
            else if (optionsRow[j].size() == 2 and ignore.count(j) == 0) //for the harder 2*2*2 case
            {
                std::vector<int> s1, s2, s3; //the three sets of options. Using vectors so I can get the values separately
                int search1, search2;
                for (int o : optionsRow[j])
                    s1.push_back(o); //putting the first set in a vector
                search1 = s1[0];
                search2 = s1[1];
                std::vector<int> cs; //vector with the col numbers/x coordinates of the members of the triple;
                cs.push_back(j);
                for (int k = 0; k < 9; k++)
                {
                    if (k != j and optionsRow[k].size() == 2)
                    {
                        if (optionsRow[k].count(search1) == 1)
                        {
                            cs.push_back(k);
                            for (int o : optionsRow[k])
                                s2.push_back(o);
                            search1 = (search1 == s2[0]) ? s2[1] : s2[0]; //replace search1 with the unused value of this pair
                        }
                        else if (optionsRow[k].count(search2) == 1)
                        {
                            cs.push_back(k);
                            for (int o : optionsRow[k])
                                s2.push_back(o);
                            search2 = (search2 == s2[0]) ? s2[1] : s2[0]; //replace search1 with the unused value of this pair
                        }
                        if (cs.size() == 2)
                        {
                            for (int l = 0; l < 9; l++)
                            {
                                if (l != cs[0] and l != cs[1] and optionsRow[l].size() == 2)
                                {
                                    if (optionsRow[l].count(search1) == 1 and optionsRow[l].count(search2) == 1)
                                    {
                                        cs.push_back(l);
                                        for (int o : optionsRow[l])
                                            s3.push_back(o);
                                        break;
                                    }
                                }
                            }
                            break;
                        }
                    }
                }
                if (cs.size() == 3) //We found a naked triple!
                {
                    std::unordered_set<int> base = {s1[0], s1[1], s2[0], s2[1], s3[0], s3[1]}; //make a base set with all 3 values
                    ignore.emplace(cs[0]);                                                     //we don't have to check these again this loop
                    ignore.emplace(cs[1]);
                    ignore.emplace(cs[2]);
                    int r = i; //just to make things a bit more readable
                    for (int c = 0; c < 9; c++)
                    {
                        if (std::find(cs.begin(), cs.end(), c) == cs.end()) //if the c coordinate is not in the cs vector
                        {
                            allOptionsCopy[r][c] = unordered_set_difference(allOptions[r][c], base);
                            if (found == false and allOptionsCopy[r][c] != allOptions[r][c]) //if there is any change
                            {
                                found = true;
                            }
                        }
                    }
                }
            }
            ignore.clear(); //empty ignore to check cols
            //next up: cols
            if (optionsCol[j].size() == 3 and ignore.count(j) == 0) //easiest case, if there's a triple
            {
                int count = 1;
                std::vector<int> cs; //vector with the col numbers/x coordinates of the members of the triple;
                cs.push_back(j);
                std::vector<std::unordered_set<int>> subsets;
                std::vector<int> base;
                for (int n : optionsCol[j])
                    base.push_back(n); //moving the set into a vector because it's easier to work with individual values
                subsets.push_back(optionsCol[j]);
                subsets.push_back({base[0], base[1]});
                subsets.push_back({base[0], base[2]});
                subsets.push_back({base[1], base[2]});
                for (int k = 0; k < 9; k++)
                {
                    if (k != j and ignore.count(k) == 0 and (std::find(subsets.begin(), subsets.end(), optionsCol[k]) != subsets.end()))
                    {
                        count++;
                        cs.push_back(k);
                    }
                }
                if (count == 3) //we found a naked triple!
                {
                    ignore.emplace(cs[0]); //we don't have to check these again this loop
                    ignore.emplace(cs[1]);
                    ignore.emplace(cs[2]);
                    int c = i; //just to make things a bit more readable
                    for (int r = 0; r < 9; r++)
                    {
                        if (std::find(cs.begin(), cs.end(), r) == cs.end()) //if the c coordinate is not in the cs vector
                        {
                            allOptionsCopy[r][c] = unordered_set_difference(allOptions[r][c], base);
                            if (found == false and allOptionsCopy[r][c] != allOptions[r][c]) //if there is any change
                            {
                                found = true;
                            }
                        }
                    }
                }
            }
            else if (optionsCol[j].size() == 2 and ignore.count(j) == 0) //for the harder 2*2*2 case
            {
                std::vector<int> s1, s2, s3; //the three sets of options. Using vectors so I can get the values separately
                int search1, search2;
                for (int o : optionsCol[j])
                    s1.push_back(o); //putting the first set in a vector
                search1 = s1[0];
                search2 = s1[1];
                std::vector<int> cs; //vector with the col numbers/x coordinates of the members of the triple;
                cs.push_back(j);
                for (int k = 0; k < 9; k++)
                {
                    if (k != j and optionsCol[k].size() == 2)
                    {
                        if (optionsCol[k].count(search1) == 1)
                        {
                            cs.push_back(k);
                            for (int o : optionsCol[k])
                                s2.push_back(o);
                            search1 = (search1 == s2[0]) ? s2[1] : s2[0]; //replace search1 with the unused value of this pair
                        }
                        else if (optionsCol[k].count(search2) == 1)
                        {
                            cs.push_back(k);
                            for (int o : optionsCol[k])
                                s2.push_back(o);
                            search2 = (search2 == s2[0]) ? s2[1] : s2[0]; //replace search1 with the unused value of this pair
                        }
                        if (cs.size() == 2)
                        {
                            for (int l = 0; l < 9; l++)
                            {
                                if (l != cs[0] and l != cs[1] and optionsCol[l].size() == 2)
                                {
                                    if (optionsCol[l].count(search1) == 1 and optionsCol[l].count(search2) == 1)
                                    {
                                        cs.push_back(l);
                                        for (int o : optionsCol[l])
                                            s3.push_back(o);
                                        break;
                                    }
                                }
                            }
                            break;
                        }
                    }
                }
                if (cs.size() == 3) //We found a naked triple!
                {
                    std::unordered_set<int> base = {s1[0], s1[1], s2[0], s2[1], s3[0], s3[1]}; //make a base set with all 3 values
                    ignore.emplace(cs[0]);                                                     //we don't have to check these again this loop
                    ignore.emplace(cs[1]);
                    ignore.emplace(cs[2]);
                    int c = i; //just to make things a bit more readable
                    for (int r = 0; r < 9; r++)
                    {
                        if (std::find(cs.begin(), cs.end(), r) == cs.end()) //if the c coordinate is not in the cs vector
                        {
                            allOptionsCopy[r][c] = unordered_set_difference(allOptions[r][c], base);
                            if (allOptionsCopy[r][c] != allOptions[r][c]) //if there is any change
                            {
                                found = true;
                            }
                        }
                    }
                }
            }
            ignore.clear(); //empty ignore to check blocks
            //next up: blocks
            if (optionsBlock[j].size() == 3 and ignore.count(j) == 0) //easiest case, if there's a triple
            {
                int count = 1;
                std::vector<int> cs; //vector with the col numbers/x coordinates of the members of the triple;
                cs.push_back(j);
                std::vector<std::unordered_set<int>> subsets;
                std::vector<int> base;
                for (int n : optionsBlock[j])
                    base.push_back(n); //moving the set into a vector because it's easier to work with individual values
                subsets.push_back(optionsBlock[j]);
                subsets.push_back({base[0], base[1]});
                subsets.push_back({base[0], base[2]});
                subsets.push_back({base[1], base[2]});
                for (int k = 0; k < 9; k++)
                {
                    if (k != j and ignore.count(k) == 0 and (std::find(subsets.begin(), subsets.end(), optionsBlock[k]) != subsets.end()))
                    {
                        count++;
                        cs.push_back(k);
                    }
                }
                if (count == 3) //we found a naked triple!
                {
                    ignore.emplace(cs[0]); //we don't have to check these again this loop
                    ignore.emplace(cs[1]);
                    ignore.emplace(cs[2]);
                    int r0 = i / 3 * 3;
                    int c0 = i % 3 * 3;
                    int r1 = r0 + cs[0] / 3;
                    int c1 = c0 + cs[0] % 3;
                    int r2 = r0 + cs[1] / 3;
                    int c2 = c0 + cs[1] % 3;
                    int r3 = r0 + cs[2] / 3;
                    int c3 = c0 + cs[2] % 3;
                    for (int r = r0; r < r0 + 3; r++)
                    {
                        for (int c = c0; c < c0 + 3; c++)
                        {
                            if (!(r == r1 and c == c1) and !(r == r2 and c == c2) and !(r == r3 and c == c3))
                            {
                                allOptionsCopy[r][c] = unordered_set_difference(allOptions[r][c], base); //delete numbers in the pair from the rest of the row
                                if (found == false and allOptionsCopy[r][c] != allOptions[r][c])
                                    found = true; //if this had any effect set found to true
                            }
                        }
                    }
                }
            }
            else if (optionsBlock[j].size() == 2 and ignore.count(j) == 0) //for the harder 2*2*2 case
            {
                std::vector<int> s1, s2, s3; //the three sets of options. Using vectors so I can get the values separately
                int search1, search2;
                for (int o : optionsBlock[j])
                    s1.push_back(o); //putting the first set in a vector
                search1 = s1[0];
                search2 = s1[1];
                std::vector<int> cs; //vector with the col numbers/x coordinates of the members of the triple;
                cs.push_back(j);
                for (int k = 0; k < 9; k++)
                {
                    if (k != j and optionsBlock[k].size() == 2)
                    {
                        if (optionsBlock[k].count(search1) == 1)
                        {
                            cs.push_back(k);
                            for (int o : optionsBlock[k])
                                s2.push_back(o);
                            search1 = (search1 == s2[0]) ? s2[1] : s2[0]; //replace search1 with the unused value of this pair
                        }
                        else if (optionsBlock[k].count(search2) == 1)
                        {
                            cs.push_back(k);
                            for (int o : optionsBlock[k])
                                s2.push_back(o);
                            search2 = (search2 == s2[0]) ? s2[1] : s2[0]; //replace search1 with the unused value of this pair
                        }
                        if (cs.size() == 2)
                        {
                            for (int l = 0; l < 9; l++)
                            {
                                if (l != cs[0] and l != cs[1] and optionsBlock[l].size() == 2)
                                {
                                    if (optionsBlock[l].count(search1) == 1 and optionsBlock[l].count(search2) == 1)
                                    {
                                        cs.push_back(l);
                                        for (int o : optionsBlock[l])
                                            s3.push_back(o);
                                        break;
                                    }
                                }
                            }
                            break;
                        }
                    }
                }
                if (cs.size() == 3) //We found a naked triple!
                {
                    std::unordered_set<int> base = {s1[0], s1[1], s2[0], s2[1], s3[0], s3[1]}; //make a base set with all 3 values
                    ignore.emplace(cs[0]);                                                     //we don't have to check these again this loop
                    ignore.emplace(cs[1]);
                    ignore.emplace(cs[2]);
                    int r0 = i / 3 * 3;
                    int c0 = i % 3 * 3;
                    int r1 = r0 + cs[0] / 3;
                    int c1 = c0 + cs[0] % 3;
                    int r2 = r0 + cs[1] / 3;
                    int c2 = c0 + cs[1] % 3;
                    int r3 = r0 + cs[2] / 3;
                    int c3 = c0 + cs[2] % 3;
                    for (int r = r0; r < r0 + 3; r++)
                    {
                        for (int c = c0; c < c0 + 3; c++)
                        {
                            if (!(r == r1 and c == c1) and !(r == r2 and c == c2) and !(r == r3 and c == c3))
                            {
                                allOptionsCopy[r][c] = unordered_set_difference(allOptions[r][c], base); //delete numbers in the pair from the rest of the row
                                if (found == false and allOptionsCopy[r][c] != allOptions[r][c])
                                    found = true; //if this had any effect set found to true
                            }
                        }
                    }
                }
            }
        }
    }
    if (found)
    { //because there's somehow false positives an extra check
        if (allOptions != allOptionsCopy)
        {
            allOptions = allOptionsCopy; //apply changes
            return true;
        }
    }
    return false;
}

bool Sudoku::hidden_pairs()
{
    bool found = false;
    //trying a new programming method: the code explains the comments to the computer.
    std::vector<std::vector<std::unordered_set<int>>> allOptionsCopy = allOptions; //first make working copies of all options
    for (int i = 0; i < 9; i++)                                                    //iterate (i) over the numbers 0 to 9 (every row, collumn and block)
    {
        std::vector<std::unordered_set<int>> optionsRow = get_options_row(i); //get the row
        for (int j = 0; j < 8; j++)                                           //iterate (j) over the numbers 0 to 8 (if the last square is part of a pair it will be found earlier)
        {
            if (optionsRow[j].size() >= 2) //if the square has at least two options
            {
                for (int k = j + 1; k < 9; k++) //iterate (k) from the last iterator (j) up to 9
                {
                    if (optionsRow[j].size() > 2 or optionsRow[k].size() > 2) //if the first or second square has more than 2 options (otherwise it's a naked pair)
                    {
                        std::unordered_set<int> intersection = unordered_set_intersection(optionsRow[j], optionsRow[k]); //intersect the set from the square found in the last loop with the set from this loop
                        if (intersection.size() >= 2)                                                                    //if the intersection has at least 2 options
                        {
                            std::unordered_set<int> difference = unordered_set_difference(intersection, get_options_row_except(i, j, k)); //get the difference between the intersection of the two squares, and all options except for the two squares
                            if (difference.size() == 2)                                                                                   //if the result of the difference is exactly 2
                            {
                                allOptionsCopy[i][j] = difference; //replace the two sets of options with the result
                                allOptionsCopy[i][k] = difference;
                                found = true; //and set found to true
                                break;        //no need to iterate over k because we already found the pair for j
                            }
                        }
                    }
                }
            }
        }
        //now do the same thing for col
        std::vector<std::unordered_set<int>> optionsCol = get_options_col(i); //get the col
        for (int j = 0; j < 8; j++)                                           //iterate (j) over the numbers 0 to 8 (if the last square is part of a pair it will be found earlier)
        {
            if (optionsCol[j].size() >= 2) //if the square has at least two options
            {
                for (int k = j + 1; k < 9; k++) //iterate (k) from the last iterator (j) up to 9
                {
                    if (optionsCol[j].size() > 2 or optionsCol[k].size() > 2) //if the first or second square has more than 2 options (otherwise it's a naked pair)
                    {
                        std::unordered_set<int> intersection = unordered_set_intersection(optionsCol[j], optionsCol[k]); //intersect the set from the square found in the last loop with the set from this loop
                        if (intersection.size() >= 2)                                                                    //if the intersection has at least 2 options
                        {
                            std::unordered_set<int> difference = unordered_set_difference(intersection, get_options_col_except(j, k, i)); //get the difference between the intersection of the two squares, and all options except for the two squares
                            if (difference.size() == 2)                                                                                   //if the result of the difference is exactly 2
                            {
                                allOptionsCopy[j][i] = difference; //replace the two sets of options with the result
                                allOptionsCopy[k][i] = difference;
                                found = true; //and set found to true
                                break;        //no need to iterate over k because we already found the pair for j
                            }
                        }
                    }
                }
            }
        }
        //now do the same thing for block
        std::vector<std::unordered_set<int>> optionsBlock = get_options_block(i); //get the col
        for (int j = 0; j < 8; j++)                                               //iterate (j) over the numbers 0 to 8 (if the last square is part of a pair it will be found earlier)
        {
            if (optionsBlock[j].size() >= 2) //if the square has at least two options
            {
                for (int k = j + 1; k < 9; k++) //iterate (k) from the last iterator (j) up to 9
                {
                    if (optionsBlock[j].size() > 2 or optionsBlock[k].size() > 2) //if the first or second square has more than 2 options (otherwise it's a naked pair)
                    {
                        std::unordered_set<int> intersection = unordered_set_intersection(optionsBlock[j], optionsBlock[k]); //intersect the set from the square found in the last loop with the set from this loop
                        if (intersection.size() >= 2)                                                                        //if the intersection has at least 2 options
                        {
                            //this is different, get the coordinates of the squares
                            int r0 = i / 3 * 3; //first get the coords of the top left square of the block
                            int c0 = i % 3 * 3;
                            int r1 = r0 + j / 3;                                                                                                   //get the row of the first square
                            int c1 = c0 + j % 3;                                                                                                   //get the col of the first square
                            int r2 = r0 + k / 3;                                                                                                   //get the row of the second square
                            int c2 = c0 + k % 3;                                                                                                   //get the col of the second square
                            std::unordered_set<int> difference = unordered_set_difference(intersection, get_options_block_except(r0, c0, r1, c1)); //get the difference between the intersection of the two squares, and all options except for the two squares
                            if (difference.size() == 2)                                                                                            //if the result of the difference is exactly 2
                            {
                                allOptionsCopy[r1][c1] = difference; //replace the two sets of options with the result
                                allOptionsCopy[r2][c2] = difference;
                                found = true; //and set found to true
                                break;        //no need to iterate over k because we already found the pair for j
                            }
                        }
                    }
                }
            }
        }
    }
    if (found)
    {
        found = (allOptions != allOptionsCopy);
        allOptions = allOptionsCopy;
    }
    return found;
}

bool Sudoku::hidden_triples()
{
    bool found = false;
    std::vector<std::vector<std::unordered_set<int>>> allOptionsCopy = allOptions; //first make working copies of all options

    for (int i = 0; i < 9; i++) //iterate(i) from 0 to 9 (all rows/cols/blocks)
    {
        std::vector<std::unordered_set<int>> optionsRow = get_options_row(i); //get the row
        for (int j = 0; j < 7; j++)                                           //iterate(j) from 0 to 7 (8 and 9 will always be part of earlier triples)
        {
            if (optionsRow[j].size() >= 2) //if the square has at least two options
            {
                for (int k = j + 1; k < 8; k++) //iterate(k) from the last square(j) up to 8
                {
                    bool breakout = false;
                    if (optionsRow[k].size() >= 2 and unordered_set_intersection(optionsRow[j], optionsRow[k]).size() >= 1) //if the square has at least two options, and the intersection of the two contains at least one number
                    {
                        for (int l = k + 1; l < 9; l++) //iterate(l) from the last square(k) up to 9
                        {
                            if (optionsRow[l].size() >= 2 and unordered_set_intersection(optionsRow[j], optionsRow[l]).size() >= 1 and unordered_set_intersection(optionsRow[k], optionsRow[l]).size()) //if the square has at least two options, and the intersection of square j and l contains at least one number, and the intersection of k and l contains at least one number
                            {
                                std::unordered_set<int> intersection = unordered_set_3_way_intersection(optionsRow[j], optionsRow[k], optionsRow[l]); //get the 3 way intersection of square j, k and l
                                std::unordered_set<int> difference = unordered_set_difference(intersection, get_options_row_except(i, j, k, l));      //get the difference between above intersection, and the row except for j, k and l //note, make another get row except
                                if (difference.size() == 3)                                                                                           //if the difference has exactly 3 options
                                {
                                    if (unordered_set_intersection(optionsRow[j], difference).size() >= 2 and unordered_set_intersection(optionsRow[k], difference).size() >= 2 and unordered_set_intersection(optionsRow[l], difference).size() >= 2) //if the intersection between the difference and each square has at least 2 options
                                    {
                                        //we've got triples or hidden triples
                                        allOptionsCopy[i][j] = unordered_set_intersection(allOptions[i][j], difference); //replace each square with the intersection of the square and the above difference
                                        allOptionsCopy[i][k] = unordered_set_intersection(allOptions[i][k], difference);
                                        allOptionsCopy[i][l] = unordered_set_intersection(allOptions[i][l], difference);
                                        if (allOptionsCopy[i][j] != allOptions[i][j] or allOptionsCopy[i][k] != allOptions[i][k] or allOptionsCopy[i][l] != allOptions[i][l]) //if at least one of the squares changed
                                            found = true;
                                        breakout = true; //tell the above loop to break
                                        break;           //break
                                    }
                                }
                            }
                        }
                    }
                    if (breakout)
                        break; //if triples are found in the l loop, break
                }
            }
        }
        //repeat for cols
        std::vector<std::unordered_set<int>> optionsCol = get_options_col(i); //get the col
        for (int j = 0; j < 7; j++)                                           //iterate(j) from 0 to 7 (8 and 9 will always be part of earlier triples)
        {
            if (optionsCol[j].size() >= 2) //if the square has at least two options
            {
                for (int k = j + 1; k < 8; k++) //iterate(k) from the last square(j) up to 8
                {
                    bool breakout = false;
                    if (optionsCol[k].size() >= 2 and unordered_set_intersection(optionsCol[j], optionsCol[k]).size() >= 1) //if the square has at least two options, and the intersection of the two contains at least one number
                    {
                        for (int l = k + 1; l < 9; l++) //iterate(l) from the last square(k) up to 9
                        {
                            if (optionsCol[l].size() >= 2 and unordered_set_intersection(optionsCol[j], optionsCol[l]).size() >= 1 and unordered_set_intersection(optionsCol[k], optionsCol[l]).size()) //if the square has at least two options, and the intersection of square j and l contains at least one number, and the intersection of k and l contains at least one number
                            {
                                std::unordered_set<int> intersection = unordered_set_3_way_intersection(optionsCol[j], optionsCol[k], optionsCol[l]); //get the 3 way intersection of square j, k and l
                                std::unordered_set<int> difference = unordered_set_difference(intersection, get_options_col_except(j, k, l, i));      //get the difference between above intersection, and the col except for j, k and l
                                if (difference.size() == 3)                                                                                           //if the difference has exactly 3 options
                                {
                                    if (unordered_set_intersection(optionsCol[j], difference).size() >= 2 and unordered_set_intersection(optionsCol[k], difference).size() >= 2 and unordered_set_intersection(optionsCol[l], difference).size() >= 2) //if the intersection between the difference and each square has at least 2 options
                                    {
                                        //we've got triples or hidden triples
                                        allOptionsCopy[j][i] = unordered_set_intersection(allOptions[j][i], difference); //replace each square with the intersection of the square and the above difference
                                        allOptionsCopy[k][i] = unordered_set_intersection(allOptions[k][i], difference);
                                        allOptionsCopy[l][i] = unordered_set_intersection(allOptions[l][i], difference);
                                        if (allOptionsCopy[j][i] != allOptions[j][i] or allOptionsCopy[k][i] != allOptions[k][i] or allOptionsCopy[l][i] != allOptions[l][i]) //if at least one of the squares changed
                                            found = true;
                                        breakout = true; //tell the above loop to break
                                        break;           //break
                                    }
                                }
                            }
                        }
                    }
                    if (breakout)
                        break; //if triples are found in the l loop, break
                }
            }
        }
        //repeat for blocks
        std::vector<std::unordered_set<int>> optionsBlock = get_options_block(i); //get the block
        for (int j = 0; j < 7; j++)                                               //iterate(j) from 0 to 7 (8 and 9 will always be part of earlier triples)
        {
            if (optionsBlock[j].size() >= 2) //if the square has at least two options
            {
                for (int k = j + 1; k < 8; k++) //iterate(k) from the last square(j) up to 8
                {
                    bool breakout = false;
                    if (optionsBlock[k].size() >= 2 and unordered_set_intersection(optionsBlock[j], optionsBlock[k]).size() >= 1) //if the square has at least two options, and the intersection of the two contains at least one number
                    {
                        for (int l = k + 1; l < 9; l++) //iterate(l) from the last square(k) up to 9
                        {
                            if (optionsBlock[l].size() >= 2 and unordered_set_intersection(optionsBlock[j], optionsBlock[l]).size() >= 1 and unordered_set_intersection(optionsBlock[k], optionsBlock[l]).size()) //if the square has at least two options, and the intersection of square j and l contains at least one number, and the intersection of k and l contains at least one number
                            {
                                int r0 = i / 3 * 3;
                                int c0 = i % 3 * 3;
                                int r1 = r0 + j / 3;
                                int c1 = c0 + j % 3;
                                int r2 = r0 + k / 3;
                                int c2 = c0 + k % 3;
                                int r3 = r0 + l / 3;
                                int c3 = c0 + l % 3;
                                std::unordered_set<int> intersection = unordered_set_3_way_intersection(optionsBlock[j], optionsBlock[k], optionsBlock[l]);    //get the 3 way intersection of square j, k and l
                                std::unordered_set<int> difference = unordered_set_difference(intersection, get_options_block_except(r1, c1, r2, c2, r3, c3)); //get the difference between above intersection, and the block except for j, k and l
                                if (difference.size() == 3)                                                                                                    //if the difference has exactly 3 options
                                {
                                    if (unordered_set_intersection(optionsBlock[j], difference).size() >= 2 and unordered_set_intersection(optionsBlock[k], difference).size() >= 2 and unordered_set_intersection(optionsBlock[l], difference).size() >= 2) //if the intersection between the difference and each square has at least 2 options
                                    {
                                        //we've got triples or hidden triples
                                        allOptionsCopy[r1][c1] = unordered_set_intersection(allOptions[r1][c1], difference); //replace each square with the intersection of the square and the above difference
                                        allOptionsCopy[r2][c2] = unordered_set_intersection(allOptions[r2][c2], difference);
                                        allOptionsCopy[r3][c3] = unordered_set_intersection(allOptions[r3][c3], difference);
                                        if (allOptionsCopy[r1][c1] != allOptions[r1][c1] or allOptionsCopy[r2][c2] != allOptions[r2][c2] or allOptionsCopy[r3][c3] != allOptions[r3][c3]) //if at least one of the squares changed
                                            found = true;
                                        breakout = true; //tell the above loop to break
                                        break;           //break
                                    }
                                }
                            }
                        }
                    }
                    if (breakout)
                        break; //if triples are found in the l loop, break
                }
            }
        }
    }

    if (found)
    {
        found = (allOptions != allOptionsCopy);
        allOptions = allOptionsCopy;
    }
    return found;
}

bool Sudoku::pointing_pairs()
{
    bool found = false;
    std::vector<std::vector<std::unordered_set<int>>> allOptionsCopy = allOptions; //first make working copies of all options
    for (int i = 0; i < 9; i++)                                                    //iterate(i) from 0 to 9 (all blocks)
    {
        std::vector<std::unordered_set<int>> optionsBlock = get_options_block(i); //get the block
        for (int j = 0; j < 8; j++)                                               //iterate(j) from 0 to 8 (No need to check the last square because it will always be tested with an earlier square)
        {
            int r0 = i / 3 * 3; //calculate r and c for the square.
            int c0 = i % 3 * 3;
            int r = r0 + j / 3;
            int c = c0 + j % 3;
            if (puzzle[r][c] == 0) //if it isn't solved
            {
                for (int o : optionsBlock[j]) //iterate through every option
                {
                    //if the option exist in at least one other square in the same row within the block (No need to check the last square in the row, going to use some maths magic)
                    if (j % 3 != 2 and (optionsBlock[j + 1].count(o) == 1 or optionsBlock[j / 3 * 3 + 2].count(o) == 1)) //say you are testing square 4. 4%3 = 1, 4+1 == 5, 4/3 = 1, 1*3 = 3, 3+2 = 5
                    {
                        bool ispair = true;         //create bool ispair to remember if the option has been found. default true
                        for (int k = 0; k < 9; k++) //iterate(k) through every square within the block
                        {
                            if (k / 3 != j / 3 and optionsBlock[k].count(o) == 1) //if the square is not in the same row as square j and it contains the number we're checking
                            {
                                ispair = false; //set bool to false
                                break;          //no more need to test further.
                            }
                        }
                        if (ispair) //if bool ispair is still true
                        {
                            for (int k = 0; k < 9; k++) //iterate(k) through every square in the row
                            {
                                if (k / 3 != c / 3) //if it's not in the same block as the square
                                {
                                    if (allOptionsCopy[r][k].erase(o) == 1) //remove the number and check if there are actual changes
                                        found = true;                       //if so, found = true;
                                }
                            }
                        }
                    }
                    //if the option exist in at least one other square in the same col within the block (No need to check the last square in the col, going to use some maths magic)
                    if (j / 3 != 2 and (optionsBlock[j + 3].count(o) == 1 or optionsBlock[j % 3 + 6].count(o) == 1)) //say you are testing square 4. 4/3 = 1, 4+3 == 7, 4%3 = 1, 1+6=7
                    {
                        bool ispair = true;         //create bool ispair to remember if the option has been found. default true
                        for (int k = 0; k < 9; k++) //iterate(k) through every square within the block
                        {
                            if (k % 3 != j % 3 and optionsBlock[k].count(o) == 1) //if the square is not in the same col as square j and it contains the number we're checking
                            {
                                ispair = false; //set bool to false
                                break;          //no more need to test further.
                            }
                        }
                        if (ispair) //if bool ispair is still true
                        {
                            for (int k = 0; k < 9; k++) //iterate(k) through every square in the col
                            {
                                if (k / 3 != r / 3) //if it's not in the same block as the square
                                {
                                    if (allOptionsCopy[k][c].erase(o) == 1) //remove the number and check if there are actual changes
                                        found = true;                       //if so, found = true;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    if (found)
    {
        found = (allOptions != allOptionsCopy);
        allOptions = allOptionsCopy;
    }
    return found;
}

bool Sudoku::box_line_reduction()
{
    bool found = false;
    std::vector<std::vector<std::unordered_set<int>>> allOptionsCopy = allOptions; //first make working copies of all options

    for (int i = 0; i < 9 and found == false; i++) //iterate(i) from 0 to 9 (all rows and columns)
    {
        //first rows
        std::vector<std::unordered_set<int>> optionsRow = get_options_row(i); //get the row
        for (int j = 0; j < 9; j++)                                           //iterate(j) from 0 to 9 (all squares)
        {
            if (puzzle[i][j] == 0) //if the square is not solved
            {
                std::unordered_set<int> options = optionsRow[j]; //copy the options into a seperate variable
                for (int k = 0; k < 9; k++)                      //iterate(k) from 0 to 9 (all squares)
                {
                    if (k < j / 3 * 3 or k >= j / 3 * 3 + 3) //if the square is not in the same block as square j
                    {
                        options = unordered_set_difference(options, optionsRow[k]); //get the difference between the options variable and k, and save it in the same variable
                        if (options.size() == 0)
                            break; //if the options variable is empty, break
                    }
                }
                if (options.size() == 0)
                    continue;       //if the options variable is empty, go to next iteration
                int r0 = i / 3 * 3; //get the row and column of the top left square in the block
                int c0 = j / 3 * 3;
                for (int r = r0; r < r0 + 3; r++) //iterate (r) through all rows in the block
                {
                    if (r != i) //if the row is not row i
                    {
                        for (int c = c0; c < c0 + 3; c++) //iterate (c) though all cows in the block
                        {
                            allOptionsCopy[r][c] = unordered_set_difference(allOptionsCopy[r][c], options); //get the difference between the square, and the remaining options. And save it back into the square
                            if (allOptionsCopy[r][c] != allOptions[r][c])
                                found = true; //if anything changed, set found to true
                        }
                    }
                }
            }
        }
        //next up: cols
        std::vector<std::unordered_set<int>> optionsCol = get_options_col(i); //get the col
        for (int j = 0; j < 9 and found == false; j++)                        //iterate(j) from 0 to 9 (all squares)
        {
            if (puzzle[j][i] == 0) //if the square is not solved
            {
                std::unordered_set<int> options = optionsCol[j]; //copy the options into a seperate variable
                for (int k = 0; k < 9; k++)                      //iterate(k) from 0 to 9 (all squares)
                {
                    if (k < j / 3 * 3 or k >= j / 3 * 3 + 3) //if the square is not in the same block as square j
                    {
                        options = unordered_set_difference(options, optionsCol[k]); //get the difference between the options variable and k, and save it in the same variable
                        if (options.size() == 0)
                            break; //if the options variable is empty, break
                    }
                }
                if (options.size() == 0)
                    continue;       //if the options variable is empty, go to next iteration
                int r0 = j / 3 * 3; //get the row and column of the top left square in the block
                int c0 = i / 3 * 3;
                for (int c = c0; c < c0 + 3; c++) //iterate (r) through all cols in the block
                {
                    if (c != i) //if the row is not row i
                    {
                        for (int r = r0; r < r0 + 3; r++) //iterate (c) though all cows in the block
                        {
                            allOptionsCopy[r][c] = unordered_set_difference(allOptionsCopy[r][c], options); //get the difference between the square, and the remaining options. And save it back into the square
                            if (allOptionsCopy[r][c] != allOptions[r][c])
                                found = true; //if anything changed, set found to true
                        }
                    }
                }
            }
        }
    }

    if (found)
    {
        found = (allOptions != allOptionsCopy);
        allOptions = allOptionsCopy;
    }
    return found;
}

bool Sudoku::x_wing()
{
    bool found = false;
    std::vector<std::vector<std::unordered_set<int>>> allOptionsCopy = allOptions; //first make working copies of all options
    for (int n = 1; n < 10; n++)                                                   //going from 1 through 9, because we're looking for numbers, not rows or cols
    {
        std::vector<std::vector<bool>> locationRows = get_possible_locations(n); //get the list of possible locations for number i
        std::vector<std::vector<bool>> locationCols = locationRows;              //copy it into a seperate value and flip it to easily check cols
        transpose_matrix(locationCols);

        for (int j = 0; j < 8; j++) //iterate 0 to 8, 9 is unnecessary because it will be tested in earlier iterations
        {
            //first check rows
            if (std::count(locationRows[j].begin(), locationRows[j].end(), true) == 2)
            {
                int c1 = std::find(locationRows[j].begin(), locationRows[j].end(), true) - locationRows[j].begin();          //find index of first true
                int c2 = std::find(locationRows[j].begin() + c1 + 1, locationRows[j].end(), true) - locationRows[j].begin(); //find index of second true
                for (int k = j + 1; k < 9; k++)
                {
                    if (locationRows[k] == locationRows[j]) //this is enough to check if there are open spots at the exact same cols
                    {
                        for (int r = 0; r < 9; r++)
                        {
                            if (r != j and r != k)
                            {
                                if (allOptionsCopy[r][c1].erase(n) == 1 or allOptionsCopy[r][c2].erase(n) == 1)
                                    found = true;
                            }
                        }
                        break;
                    }
                }
            }
            //next up, cols
            if (std::count(locationCols[j].begin(), locationCols[j].end(), true) == 2)
            {
                int r1 = std::find(locationCols[j].begin(), locationCols[j].end(), true) - locationCols[j].begin();          //find index of first true
                int r2 = std::find(locationCols[j].begin() + r1 + 1, locationCols[j].end(), true) - locationCols[j].begin(); //find index of second true
                for (int k = j + 1; k < 9; k++)
                {
                    if (locationCols[k] == locationCols[j]) //this is enough to check if there are open spots at the exact same cols
                    {
                        for (int c = 0; c < 9; c++)
                        {
                            if (c != j and c != k)
                            {
                                if (allOptionsCopy[r1][c].erase(n) == 1 or allOptionsCopy[r2][c].erase(n) == 1)
                                    found = true;
                            }
                        }
                        break;
                    }
                }
            }
        }
    }
    if (found)
    {
        found = (allOptions != allOptionsCopy);
        allOptions = allOptionsCopy;
    }
    return found;
}

bool Sudoku::simple_colouring()
{
    bool found = false;
    //std::vector<std::vector<std::unordered_set<int>>> allOptionsCopy = allOptions; //because there is no comparison, no copy is necessary
    for (int n = 1; n < 10; n++) //going from 1 through 9, because we're looking for numbers, not rows or cols
    {
        std::vector<std::vector<bool>> locations = get_possible_locations(n); //get the list of possible locations for number i
        std::vector<std::vector<bool>> locationCols = locations;              //transposed version to save some calculations later on.
        transpose_matrix(locationCols);
        std::vector<std::vector<int>> chain;
        int chainLength = 0;
        std::function<void(int, int, int)> find_chain;
        find_chain = [&](int r, int c, int colour) { //I don't want to make a function I only use once, but I need recursion so I'm making a lambda
            chain[r][c] = colour;
            chainLength++;
            std::vector<bool> locationRow = locations[r];
            std::vector<bool> locationCol = locationCols[c];
            std::vector<bool> locationBlock = get_block(locations, r, c);
            if (count(locationRow.begin(), locationRow.end(), true) == 2)
            {
                int c2 = find(locationRow.begin(), locationRow.end(), true) - locationRow.begin();
                if (c2 == c)
                    c2 = find(locationRow.begin() + c + 1, locationRow.end(), true) - locationRow.begin();
                if (chain[r][c2] == 0)
                    find_chain(r, c2, (colour == 1) ? 2 : 1);
            }
            if (count(locationCol.begin(), locationCol.end(), true) == 2)
            {
                int r2 = find(locationCol.begin(), locationCol.end(), true) - locationCol.begin();
                if (r2 == r)
                    r2 = find(locationCol.begin() + r + 1, locationCol.end(), true) - locationCol.begin();
                if (chain[r2][c] == 0)
                    find_chain(r2, c, (colour == 1) ? 2 : 1);
            }
            if (count(locationBlock.begin(), locationBlock.end(), true) == 2)
            {
                int r0 = r / 3 * 3; //get the row and column of the top left square in the block
                int c0 = c / 3 * 3;
                int b = (r - r0) * 3 + c - c0; //get the index of the current square;
                int b2 = find(locationBlock.begin(), locationBlock.end(), true) - locationBlock.begin();
                if (b2 == b)
                    b2 = find(locationBlock.begin() + b + 1, locationBlock.end(), true) - locationBlock.begin();
                int r2 = r0 + b2 / 3; //get the row and column of the second square
                int c2 = c0 + b2 % 3;
                if (chain[r2][c2] == 0)
                    find_chain(r2, c2, (colour == 1) ? 2 : 1);
            }
        };
        for (int i = 0; i < 9; i++)
        {
            int r = -1;
            int c = -1;
            std::vector<bool> locationRow = locations[i];
            std::vector<bool> locationCol = locationCols[i];
            if (count(locationRow.begin(), locationRow.end(), true) == 2)
            {
                r = i;
                c = find(locationRow.begin(), locationRow.end(), true) - locationRow.begin();
            }
            else if (count(locationCol.begin(), locationCol.end(), true) == 2)
            {
                c = i;
                r = find(locationCol.begin(), locationCol.end(), true) - locationCol.begin();
            }
            else
            {
                std::vector<bool> locationBlock = get_block(locations, i);
                if (count(locationBlock.begin(), locationBlock.end(), true) == 2)
                {
                    int bi = find(locationBlock.begin(), locationBlock.end(), true) - locationBlock.begin();
                    int r = i / 3 * 3 + bi / 3;
                    int c = i % 3 * 3 + bi % 3;
                }
            }
            if (r != -1 and (chain.size() == 0 or chain[r][c] == 0))
            {
                bool changed = false;
                chainLength = 0;
                chain = {{0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}};
                find_chain(r, c, 1);
                if (chainLength > 2)
                {
                    std::vector<std::vector<int>> chainCols = chain;
                    transpose_matrix(chainCols);
                    for (int j = 0; j < 9; j++)
                    {
                        for (int k = 0; k < 9; k++) //first test rule 4
                        {
                            if (locations[j][k] == true and chain[j][k] == 0)
                            {
                                std::vector<int> chainRow = chain[j];
                                std::vector<int> chainCol = chainCols[k];
                                std::vector<int> chainBlock = get_block(chain, j, k);
                                if ((find(chainRow.begin(), chainRow.end(), 1) != chainRow.end() or find(chainCol.begin(), chainCol.end(), 1) != chainCol.end() or find(chainBlock.begin(), chainBlock.end(), 1) != chainBlock.end()) and (find(chainRow.begin(), chainRow.end(), 2) != chainRow.end() or find(chainCol.begin(), chainCol.end(), 2) != chainCol.end() or find(chainBlock.begin(), chainBlock.end(), 2) != chainBlock.end())) //if the square sees both a 1 and a 2
                                {
                                    allOptions[j][k].erase(n);
                                    found = changed = true;
                                }
                            }
                        }
                        //then test rule 2
                        int remove = -1;
                        std::vector<int> chainRow = chain[j];
                        std::vector<int> chainCol = chainCols[j];
                        std::vector<int> chainBlock = get_block(chain, j);
                        if (count(chainRow.begin(), chainRow.end(), 1) >= 2 or count(chainCol.begin(), chainCol.end(), 1) >= 2 or count(chainBlock.begin(), chainBlock.end(), 1) >= 2) //two of the same colours see each other
                        {
                            remove = 1;
                        }
                        else if (count(chainRow.begin(), chainRow.end(), 2) >= 2 or count(chainCol.begin(), chainCol.end(), 2) >= 2 or count(chainBlock.begin(), chainBlock.end(), 2) >= 2) //two of the same colours see each other
                        {
                            remove = 2;
                        }
                        if (remove != -1)
                        {
                            for (int l = 0; l < 9; l++)
                            {
                                for (int m = 0; m < 9; m++)
                                {
                                    if (chain[l][m] == remove)
                                        allOptions[l][m].erase(n);
                                }
                            }
                            found = changed = true;

                            break;
                        }
                    }
                    if (changed)
                    {
                        locations = get_possible_locations(n); //refresh locations
                        locationCols = locations;
                        transpose_matrix(locationCols);
                    }
                }
            }
        }
    }
    //allOptions = allOptionsCopy;

    return found;
}

bool Sudoku::y_wing()
{
    bool found = false;
    std::vector<std::vector<std::unordered_set<int>>> allOptionsCopy = allOptions; //first make working copies of all options

    for (int r = 0; r < 9; r++)
    {
        for (int c = 0; c < 9; c++)
        {
            if (allOptions[r][c].size() == 2)
            {
                std::vector<int> options;
                options.insert(options.end(), allOptions[r][c].begin(), allOptions[r][c].end()); //casting set to vector so i can access both individually
                int n1 = options[0];
                int n2 = options[1];
                int n3 = 0;  //initialize to meaningless value
                int r2 = 10; //initialize to impossible value
                int c2 = 10; //initialize to impossible value
                int r3 = 10; //initialize to impossible value
                int c3 = 10; //initialize to impossible value
                int r4 = 10;
                int c4 = 10;
                std::unordered_set<int> remove;
                // first check square y wings (rows and colums)
                for (c2 = 0; c2 < 9; c2++)
                {
                    if (c2 != c and allOptions[r][c2].size() == 2)
                    {
                        if (allOptions[r][c2].count(n1) == 1 and allOptions[r][c2].count(n2) == 0)
                        {
                            r2 = r;
                            options.clear();
                            options.insert(options.end(), allOptions[r][c2].begin(), allOptions[r][c2].end());
                            if (options[0] == n1)
                            {
                                n3 = options[1];
                            }
                            else
                            {
                                n3 = options[0];
                            }

                            break; //since r2 and c2 were already initialized they were saved.
                        }
                        else if (allOptions[r][c2].count(n2) == 1 and allOptions[r][c2].count(n1) == 0)
                        {
                            r2 = r;
                            std::swap(n1, n2);
                            options.clear();
                            options.insert(options.end(), allOptions[r][c2].begin(), allOptions[r][c2].end());
                            if (options[0] == n1)
                            {
                                n3 = options[1];
                            }
                            else
                            {
                                n3 = options[0];
                            }
                            break;
                        }
                    }
                }
                if (r2 != 10) //we found a candidate in the same row
                {
                    std::unordered_set<int> needle = {n2, n3}; //we know exactly what we're looking for
                    std::vector<std::unordered_set<int>> col1 = get_options_col(c);
                    std::vector<std::unordered_set<int>> col2 = get_options_col(c2);
                    if (std::find(col1.begin(), col1.end(), needle) != col1.end())
                    {
                        c3 = c;
                        c4 = c2;
                        r4 = r3 = std::find(col1.begin(), col1.end(), needle) - col1.begin();
                        remove = unordered_set_intersection(needle, col2[r2]);
                    }
                    else if (std::find(col2.begin(), col2.end(), needle) != col2.end())
                    {
                        c3 = c2;
                        c4 = c;
                        r4 = r3 = std::find(col2.begin(), col2.end(), needle) - col2.begin();
                        remove = unordered_set_intersection(needle, col1[r]);
                    }
                    if (r4 != 10) // we found an y wing
                    {
                        if (allOptionsCopy[r4][c4].erase(*remove.begin()) == 1)
                            found = true;
                    }
                }
                //next check with blocks
                n3 = 0;  //reset to meaningless value
                r2 = 10; //reset to impossible value
                c2 = 10; //reset to impossible value
                r3 = 10; //reset to impossible value
                c3 = 10; //reset to impossible value
                r4 = 10;
                c4 = 10;

                std::vector<std::unordered_set<int>> optionsBlock = get_options_block(r, c);
                int b1 = r % 3 * 3 + c % 3;
                int b2 = 10; //initialize value to impossible option
                for (int b = 0; b < 9; b++)
                {
                    if (b != b1 and optionsBlock[b].size() == 2)
                    {
                        if (optionsBlock[b].count(n1) == 1 and optionsBlock[b].count(n2) == 0)
                        {
                            b2 = b;
                            options.clear();
                            options.insert(options.end(), optionsBlock[b].begin(), optionsBlock[b].end());
                            if (options[0] == n1)
                            {
                                n3 = options[1];
                            }
                            else
                            {
                                n3 = options[0];
                            }

                            break;
                        }
                        else if (optionsBlock[b].count(n2) == 1 and optionsBlock[b].count(n1) == 0)
                        {
                            b2 = b;
                            std::swap(n1, n2);
                            options.clear();
                            options.insert(options.end(), optionsBlock[b].begin(), optionsBlock[b].end());
                            if (options[0] == n1)
                            {
                                n3 = options[1];
                            }
                            else
                            {
                                n3 = options[0];
                            }
                            break;
                        }
                    }
                }
                if (b2 != 10)
                {
                    c3 = c4 = r3 = r4 = 10; //resetting
                    int r0 = r / 3 * 3;
                    int c0 = c / 3 * 3;
                    r2 = r0 + b2 / 3;
                    c2 = c0 + b2 % 3;
                    std::unordered_set<int> needle = {n2, n3}; //we know exactly what we're looking for
                    //check row in relation to block
                    if (r != r2)
                    {
                        std::vector<std::unordered_set<int>> row1 = get_options_row(r);
                        std::vector<std::unordered_set<int>> row2 = get_options_row(r2);
                        if (std::find(row1.begin(), row1.end(), needle) != row1.end())
                        {
                            r3 = r;
                            r4 = r2;
                            c3 = std::find(row1.begin(), row1.end(), needle) - row1.begin();
                            remove = unordered_set_intersection(needle, row2[c2]);
                        }
                        else if (std::find(row2.begin(), row2.end(), needle) != row2.end())
                        {
                            r3 = r2;
                            r4 = r;
                            c3 = std::find(row2.begin(), row2.end(), needle) - row2.begin();
                            remove = unordered_set_intersection(needle, row1[c]);
                        }
                        if (r4 != 10 and !(r / 3 == r2 / 3 and r2 / 3 == r3 / 3 and c / 3 == c2 / 3 and c2 / 3 == c3 / 3))
                        {
                            for (c4 = c3 / 3 * 3; c4 < c3 / 3 * 3 + 3; c4++)
                            {
                                if (allOptionsCopy[r4][c4].erase(*remove.begin()) == 1)
                                    found = true;
                            }
                        }
                    }
                    //check col in relation to block
                    c3 = c4 = r3 = r4 = 10; //resetting in case we've also found a y wing in the row
                    if (c != c2)
                    {
                        std::vector<std::unordered_set<int>> col1 = get_options_col(c);
                        std::vector<std::unordered_set<int>> col2 = get_options_col(c2);
                        if (std::find(col1.begin(), col1.end(), needle) != col1.end())
                        {
                            c3 = c;
                            c4 = c2;
                            r3 = std::find(col1.begin(), col1.end(), needle) - col1.begin();
                            remove = unordered_set_intersection(needle, col2[r2]);
                        }
                        else if (std::find(col2.begin(), col2.end(), needle) != col2.end())
                        {
                            c3 = c2;
                            c4 = c;
                            r3 = std::find(col2.begin(), col2.end(), needle) - col2.begin();
                            remove = unordered_set_intersection(needle, col1[r]);
                        }
                        if (c4 != 10 and !(r / 3 == r2 / 3 and r2 / 3 == r3 / 3 and c / 3 == c2 / 3 and c2 / 3 == c3 / 3)) //if they're all int the same block also do nothing
                        {
                            for (r4 = r3 / 3 * 3; r4 < r3 / 3 * 3 + 3; r4++)
                            {
                                if (allOptionsCopy[r4][c4].erase(*remove.begin()) == 1)
                                    found = true;
                            }
                        }
                    }
                }
            }
        }
    }

    if (found)
    {
        found = (allOptions != allOptionsCopy);
        allOptions = allOptionsCopy;
    }
    return found;
}

bool Sudoku::swordfish()
{
    bool found = false;
    std::vector<std::vector<std::unordered_set<int>>> allOptionsCopy = allOptions; //first make working copies of all options
    for (int n = 1; n < 10; n++)
    {
        std::vector<std::vector<bool>> locations = get_possible_locations(n); //get the list of possible locations for number i

        int r1, r2, r3;
        int c1, c2, c3;
        r1 = r2 = r3 = 10;
        c1 = c2 = c3 = 10;
        bool foundCase = false;
        //let's first test rows.
        for (int i = 0; i < 7 and !foundCase; i++)
        {
            if (count(locations[i].begin(), locations[i].end(), true) == 2 or count(locations[i].begin(), locations[i].end(), true) == 3)
            {
                r1 = i;
                c1 = std::find(locations[i].begin(), locations[i].end(), true) - locations[i].begin();
                c2 = std::find(locations[i].begin() + c1 + 1, locations[i].end(), true) - locations[i].begin();
                if (count(locations[i].begin(), locations[i].end(), true) == 3)
                    c3 = std::find(locations[i].begin() + c2 + 1, locations[i].end(), true) - locations[i].begin();
                else
                    c3 = 10; //just in case c3 was already set by a different case.
                for (int j = i + 1; j < 8 and !foundCase; j++)
                {
                    bool foundSecond = false;
                    if (count(locations[j].begin(), locations[j].end(), true) == 2)
                    {
                        if (c3 != 10)
                        {
                            if ((locations[j][c1] and locations[j][c2]) or (locations[j][c1] and locations[j][c3]) or (locations[j][c2] and locations[j][c3]))
                            {
                                foundSecond = true;
                            }
                        }
                        else
                        {
                            if ((locations[j][c1] and !locations[j][c2]) or (locations[j][c2] and !locations[j][c1]))
                            {
                                foundSecond = true;
                                c3 = std::find(locations[j].begin(), locations[j].end(), true) - locations[j].begin();
                                while (c3 == c1 or c3 == c2) //I don't want to do two seperate but exactly the same if statements, so I use a while.
                                    c3 = std::find(locations[j].begin() + c3 + 1, locations[j].end(), true) - locations[j].begin();
                            }
                        }
                    }
                    else if (count(locations[j].begin(), locations[j].end(), true) == 3)
                    {
                        if (locations[j][c1] and locations[j][c2] and (c3 == 10 or locations[j][c3]))
                        {
                            foundSecond = true;
                            if (c3 == 10)
                            {
                                c3 = std::find(locations[j].begin(), locations[j].end(), true) - locations[j].begin();
                                while (c3 == c1 or c3 == c2) //I don't want to do two seperate but exactly the same if statements, so I use a while.
                                    c3 = std::find(locations[j].begin() + c3 + 1, locations[j].end(), true) - locations[j].begin();
                            }
                        }
                    }
                    if (foundSecond)
                    {
                        r2 = j;
                        for (int k = j + 1; k < 9 and !foundCase; k++)
                        {
                            if (count(locations[k].begin(), locations[k].end(), true) == 2)
                            {
                                if ((locations[k][c1] and locations[k][c2]) or (locations[k][c1] and locations[k][c3]) or (locations[k][c2] and locations[k][c3]))
                                {
                                    foundCase = true;
                                    r3 = k;
                                }
                            }
                            else if (count(locations[k].begin(), locations[k].end(), true) == 3)
                            {
                                if (locations[k][c1] and locations[k][c2] and locations[k][c3])
                                {
                                    foundCase = true;
                                    r3 = k;
                                }
                            }
                        }
                    }
                }
            }
        }
        if (foundCase)
        {
            //we've found a case. Let's deal with it.
            for (int i = 0; i < 9; i++)
            {
                if (i != r1 and i != r2 and i != r3)
                {
                    if (allOptionsCopy[i][c1].erase(n) == 1 or allOptionsCopy[i][c2].erase(n) == 1 or allOptionsCopy[i][c3].erase(n) == 1)
                        found = true;
                }
            }

            continue; //no need to check cols for this number
        }
        //time to check cols
        std::vector<std::vector<bool>> locationCols = locations; //transposed version to save some calculations later on.
        transpose_matrix(locationCols);
        r1 = r2 = r3 = 10;
        c1 = c2 = c3 = 10;
        for (int i = 0; i < 7 and !foundCase; i++)
        {
            if (count(locationCols[i].begin(), locationCols[i].end(), true) == 2 or count(locationCols[i].begin(), locationCols[i].end(), true) == 3)
            {
                c1 = i;
                r1 = std::find(locationCols[i].begin(), locationCols[i].end(), true) - locationCols[i].begin();
                r2 = std::find(locationCols[i].begin() + r1 + 1, locationCols[i].end(), true) - locationCols[i].begin();
                if (count(locationCols[i].begin(), locationCols[i].end(), true) == 3)
                    r3 = std::find(locationCols[i].begin() + r2 + 1, locationCols[i].end(), true) - locationCols[i].begin();
                else
                    r3 = 10; //just in case r3 was already set by a different case.
                for (int j = i + 1; j < 8 and !foundCase; j++)
                {
                    bool foundSecond = false;
                    if (count(locationCols[j].begin(), locationCols[j].end(), true) == 2)
                    {
                        if (r3 != 10)
                        {
                            if ((locationCols[j][r1] and locationCols[j][r2]) or (locationCols[j][r1] and locationCols[j][r3]) or (locationCols[j][r2] and locationCols[j][r3]))
                            {
                                foundSecond = true;
                            }
                        }
                        else
                        {
                            if ((locationCols[j][r1] and !locationCols[j][r2]) or (locationCols[j][r2] and !locationCols[j][r1]))
                            {
                                foundSecond = true;
                                r3 = std::find(locationCols[j].begin(), locationCols[j].end(), true) - locationCols[j].begin();
                                while (r3 == r1 or r3 == r2) //I don't want to do two seperate but exactly the same if statements, so I use a while.
                                    r3 = std::find(locationCols[j].begin() + r3 + 1, locationCols[j].end(), true) - locationCols[j].begin();
                            }
                        }
                    }
                    else if (count(locationCols[j].begin(), locationCols[j].end(), true) == 3)
                    {
                        if (locationCols[j][r1] and locationCols[j][r2] and (r3 == 10 or locationCols[j][r3]))
                        {
                            foundSecond = true;
                            if (r3 == 10)
                            {
                                r3 = std::find(locationCols[j].begin(), locationCols[j].end(), true) - locationCols[j].begin();
                                while (r3 == r1 or r3 == r2) //I don't want to do two seperate but exactly the same if statements, so I use a while.
                                    r3 = std::find(locationCols[j].begin() + r3 + 1, locationCols[j].end(), true) - locationCols[j].begin();
                            }
                        }
                    }
                    if (foundSecond)
                    {
                        c2 = j;
                        for (int k = j + 1; k < 9 and !foundCase; k++)
                        {
                            if (count(locationCols[k].begin(), locationCols[k].end(), true) == 2)
                            {
                                if ((locationCols[k][r1] and locationCols[k][r2]) or (locationCols[k][r1] and locationCols[k][r3]) or (locationCols[k][r2] and locationCols[k][r3]))
                                {
                                    foundCase = true;
                                    c3 = k;
                                }
                            }
                            else if (count(locationCols[k].begin(), locationCols[k].end(), true) == 3)
                            {
                                if (locationCols[k][r1] and locationCols[k][r2] and locationCols[k][r3])
                                {
                                    foundCase = true;
                                    c3 = k;
                                }
                            }
                        }
                    }
                }
            }
        }
        if (foundCase)
        {
            //we've found a case. Let's deal with it.
            for (int i = 0; i < 9; i++)
            {
                if (i != c1 and i != c2 and i != c3)
                {
                    if (allOptionsCopy[r1][i].erase(n) == 1 or allOptionsCopy[r2][i].erase(n) == 1 or allOptionsCopy[r3][i].erase(n) == 1)
                        found = true;
                }
            }
        }
    }

    if (found)
        allOptions = allOptionsCopy;
    return found;
}

bool Sudoku::xyz_wing()
{
    bool found = false;
    std::vector<std::vector<std::unordered_set<int>>> allOptionsCopy = allOptions; //first make working copies of all options

    for (int r = 0; r < 9; r++)
    {
        for (int c = 0; c < 9; c++)
        {
            if (allOptions[r][c].size() == 3)
            {
                int r1 = r;
                int c1 = c;
                int r0 = r1 / 3 * 3;
                int c0 = c / 3 * 3;
                int r2, r3, c2, c3;
                r2 = r3 = c2 = c3 = 10;
                int s2, s3;
                s2 = s3 = 10;
                std::vector<int> xyz;
                xyz.insert(xyz.end(), allOptions[r][c].begin(), allOptions[r][c].end()); //casting options to vector for ease of use
                std::unordered_set<int> xy = {xyz[0], xyz[1]};
                std::unordered_set<int> xz = {xyz[0], xyz[2]};
                std::unordered_set<int> yz = {xyz[1], xyz[2]};
                std::vector<std::unordered_set<int>> subsets = {xy, xz, yz};
                std::vector<std::unordered_set<int>> optionsRow = get_options_row(r1);
                std::vector<std::unordered_set<int>> optionsCol = get_options_col(c1);
                std::vector<std::unordered_set<int>> optionsBlock = get_options_block(r1, c1);
                for (int s = 0; s < 3; s++)
                {
                    if (find(optionsRow.begin(), optionsRow.end(), subsets[s]) != optionsRow.end())
                    {
                        if (s2 == 10)
                        {
                            s2 = s;
                            r2 = r;
                            c2 = find(optionsRow.begin(), optionsRow.end(), subsets[s]) - optionsRow.begin();
                        }
                        else
                        {
                            s3 = s;
                            r3 = r;
                            c3 = find(optionsRow.begin(), optionsRow.end(), subsets[s]) - optionsRow.begin();
                        }
                        break;
                    }
                }
                for (int s = 0; s < 3; s++)
                {
                    if (find(optionsCol.begin(), optionsCol.end(), subsets[s]) != optionsCol.end())
                    {
                        if (s2 == 10)
                        {
                            s2 = s;
                            c2 = c;
                            r2 = find(optionsCol.begin(), optionsCol.end(), subsets[s]) - optionsCol.begin();
                            break;
                        }
                        else if (s2 != s)
                        {
                            s3 = s;
                            c3 = c;
                            r3 = find(optionsCol.begin(), optionsCol.end(), subsets[s]) - optionsCol.begin();
                            break;
                        }
                    }
                }
                for (int s = 0; s < 3; s++)
                {
                    if (find(optionsBlock.begin(), optionsBlock.end(), subsets[s]) != optionsBlock.end())
                    {
                        if (s2 == 10)
                        {
                            s2 = s;
                            int b2 = find(optionsBlock.begin(), optionsBlock.end(), subsets[s]) - optionsBlock.begin();
                            r2 = r0 + b2 / 3;
                            c3 = c0 + b2 % 3;
                            break;
                        }
                        else if (s2 != s)
                        {
                            s3 = s;
                            int b3 = find(optionsBlock.begin(), optionsBlock.end(), subsets[s]) - optionsBlock.begin();
                            r3 = r0 + b3 / 3;
                            c3 = c0 + b3 % 3;
                            break;
                        }
                    }
                }
                if (s3 != 10 and s2 != s3)
                {
                    std::unordered_set<int> intersection = unordered_set_intersection(subsets[s2], subsets[s3]);
                    int n = *intersection.begin();
                    for (int i = 0; i < 9; i++)
                    {
                        if (i != c1 and i != c2 and i != c3 and seen_by(r1, i, r2, c2) and seen_by(r1, i, r3, c3))
                        {
                            if (allOptionsCopy[r1][i].erase(n) == 1)
                                found = true;
                        }
                        if (i != r1 and i != r2 and i != r3 and seen_by(i, c1, r2, c2) and seen_by(i, c1, r3, c3))
                        {
                            if (allOptionsCopy[i][c1].erase(n) == 1)
                                found = true;
                        }
                    }
                    for (int i = r0; i < r0 + 3; i++)
                    {
                        for (int j = c0; j < c0 + 3; j++)
                        {
                            if (!(i == r1 and j == c1) and !(i == r2 and j == c2) and !(i == r3 and j == c3) and seen_by(i, j, r2, c2) and seen_by(i, j, r3, c3))
                            {
                                if (allOptionsCopy[i][j].erase(n) == 1)
                                    found = true;
                            }
                        }
                    }
                }
                //we're doing the same thing again with colums first because sometimes two xyz wings are possible with the same pivot
                r2 = c2 = r3 = c3 = s2 = s3 = 10;
                for (int s = 0; s < 3; s++)
                {
                    if (find(optionsCol.begin(), optionsCol.end(), subsets[s]) != optionsCol.end())
                    {
                        if (s2 == 10)
                        {
                            s2 = s;
                            c2 = c;
                            r2 = find(optionsCol.begin(), optionsCol.end(), subsets[s]) - optionsCol.begin();
                            break;
                        }
                        else
                        {
                            s3 = s;
                            c3 = c;
                            r3 = find(optionsCol.begin(), optionsCol.end(), subsets[s]) - optionsCol.begin();
                            break;
                        }
                    }
                }
                for (int s = 0; s < 3; s++)
                {
                    if (find(optionsRow.begin(), optionsRow.end(), subsets[s]) != optionsRow.end())
                    {
                        if (s2 == 10)
                        {
                            s2 = s;
                            r2 = r;
                            c2 = find(optionsRow.begin(), optionsRow.end(), subsets[s]) - optionsRow.begin();
                            break;
                        }
                        else if (s2 != s)
                        {
                            s3 = s;
                            r3 = r;
                            c3 = find(optionsRow.begin(), optionsRow.end(), subsets[s]) - optionsRow.begin();
                            break;
                        }
                    }
                }
                for (int s = 0; s < 3; s++)
                {
                    if (find(optionsBlock.begin(), optionsBlock.end(), subsets[s]) != optionsBlock.end())
                    {
                        if (s2 == 10)
                        {
                            s2 = s;
                            int b2 = find(optionsBlock.begin(), optionsBlock.end(), subsets[s]) - optionsBlock.begin();
                            r2 = r0 + b2 / 3;
                            c3 = c0 + b2 % 3;
                            break;
                        }
                        else if (s2 != s)
                        {
                            s3 = s;
                            int b3 = find(optionsBlock.begin(), optionsBlock.end(), subsets[s]) - optionsBlock.begin();
                            r3 = r0 + b3 / 3;
                            c3 = c0 + b3 % 3;
                            break;
                        }
                    }
                }
                if (s3 != 10 and s2 != s3)
                {
                    std::unordered_set<int> intersection = unordered_set_intersection(subsets[s2], subsets[s3]);
                    int n = *intersection.begin();
                    for (int i = 0; i < 9; i++)
                    {
                        if (i != c1 and i != c2 and i != c3 and seen_by(r1, i, r2, c2) and seen_by(r1, i, r3, c3))
                        {
                            if (allOptionsCopy[r1][i].erase(n) == 1)
                                found = true;
                        }
                        if (i != r1 and i != r2 and i != r3 and seen_by(i, c1, r2, c2) and seen_by(i, c1, r3, c3))
                        {
                            if (allOptionsCopy[i][c1].erase(n) == 1)
                                found = true;
                        }
                    }
                    for (int i = r0; i < r0 + 3; i++)
                    {
                        for (int j = c0; j < c0 + 3; j++)
                        {
                            if (!(i == r1 and j == c1) and !(i == r2 and j == c2) and !(i == r3 and j == c3) and seen_by(i, j, r2, c2) and seen_by(i, j, r3, c3))
                            {
                                if (allOptionsCopy[i][j].erase(n) == 1)
                                    found = true;
                            }
                        }
                    }
                }
            }
        }
    }

    if (found)
    {
        found = (allOptions != allOptionsCopy);
        allOptions = allOptionsCopy;
    }
    return found;
}

bool Sudoku::x_cycles()
{
    bool found = false;
    std::vector<std::vector<std::unordered_set<int>>> allOptionsCopy = allOptions; //first make working copies of all options
    std::vector<std::vector<int>> chain = {{0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}};
    for (int n = 1; n < 10; n++)
    {
        std::vector<std::vector<bool>> locations = get_possible_locations(n);
        std::vector<std::vector<bool>> locationCols = locations;
        transpose_matrix(locationCols);
        std::vector<std::pair<int, int>> tested;
        int startr, startc, chainLength, dr, dc;
        bool ds = false;
        std::function<bool(int, int, int, bool, bool)> find_chain;
        find_chain = [&](int r, int c, int colour, bool strong, bool allow_disc) {
            chain[r][c] = colour;
            tested.push_back({r, c});
            chainLength += 1;
            if (strong) //first try finding strong links. This is the easy part.
            {
                if (std::count(locations[r].begin(), locations[r].end(), true) == 2)
                {
                    int r2 = r;
                    int c2 = std::find(locations[r].begin(), locations[r].end(), true) - locations[r].begin();
                    if (c2 == c)
                        c2 = std::find(locations[r].begin() + c + 1, locations[r].end(), true) - locations[r].begin();
                    if ((chain[r2][c2] == 0 and find_chain(r2, c2, (colour == 1) ? 2 : 1, false, allow_disc)))
                    {
                        return true;
                    }
                }
                if (std::count(locationCols[c].begin(), locationCols[c].end(), true) == 2)
                {
                    int c2 = c;
                    int r2 = std::find(locationCols[c].begin(), locationCols[c].end(), true) - locationCols[c].begin();
                    if (r2 == r)
                        r2 = std::find(locationCols[c].begin() + r + 1, locationCols[c].end(), true) - locationCols[c].begin();
                    if ((chain[r2][c2] == 0 and find_chain(r2, c2, (colour == 1) ? 2 : 1, false, allow_disc)))
                    {
                        return true;
                    }
                }
                std::vector<bool> locationBlock = get_block(locations, r, c);
                if (std::count(locationBlock.begin(), locationBlock.end(), true) == 2)
                {
                    int b = r % 3 * 3 + c % 3;
                    int r0 = r / 3 * 3;
                    int c0 = c / 3 * 3;
                    int b2 = std::find(locationBlock.begin(), locationBlock.end(), true) - locationBlock.begin();
                    if (b2 == b)
                        b2 = std::find(locationBlock.begin() + b + 1, locationBlock.end(), true) - locationBlock.begin();
                    int r2 = r0 + b2 / 3;
                    int c2 = c0 + b2 % 3;
                    if ((chain[r2][c2] == 0 and find_chain(r2, c2, (colour == 1) ? 2 : 1, false, allow_disc)))
                    {
                        return true;
                    }
                }
                if (dr != 10 or allow_disc == false)
                {
                    chain[r][c] = 0;
                    chainLength -= 1;
                    return false;
                }
                else
                {
                    dr = r;
                    dc = c;
                    ds = false;
                    chain[r][c] = 3;                //changing the color to something neutral
                    colour = (colour == 1) ? 2 : 1; //flipping the color so the next color fits the chain.
                }
            }
            //then we try weak links
            if (strong == false and chainLength > 2 and (r == startr or c == startc or block_number(r, c) == block_number(startr, startc)))
                return true;
            if (std::count(locations[r].begin(), locations[r].end(), true) > 2)
            {
                bool done = false;
                int r2 = r;
                int next = 0;
                while (!done)
                {
                    int c2 = std::find(locations[r].begin() + next, locations[r].end(), true) - locations[r].begin();
                    if (c2 == c)
                        next = c2 + 1;
                    else if (c2 == locations[r].end() - locations[r].begin())
                        done = true;
                    else
                    {
                        if ((c2 == startc and r2 == startr and chainLength > 2) or (chain[r2][c2] == 0 and find_chain(r2, c2, (colour == 1) ? 2 : 1, true, allow_disc)))
                        {
                            return true;
                        }
                        else
                        {
                            next = c2 + 1;
                            if (next == locations[r].end() - locations[r].begin())
                                done = true;
                        }
                    }
                }
            }
            if (std::count(locationCols[c].begin(), locationCols[c].end(), true) > 2)
            {
                bool done = false;
                int c2 = c;
                int next = 0;
                while (!done)
                {
                    int r2 = std::find(locationCols[c].begin() + next, locationCols[c].end(), true) - locationCols[c].begin();
                    if (r2 == r)
                        next = r2 + 1;
                    else if (r2 == locationCols[c].end() - locationCols[c].begin())
                        done = true;
                    else
                    {
                        if ((c2 == startc and r2 == startr and chainLength > 2) or (chain[r2][c2] == 0 and find_chain(r2, c2, (colour == 1) ? 2 : 1, true, allow_disc)))
                        {
                            return true;
                        }
                        else
                        {
                            next = r2 + 1;
                            if (next == locationCols[c].end() - locationCols[c].begin())
                                done = true;
                        }
                    }
                }
            }
            std::vector<bool> locationBlock = get_block(locations, r, c);
            if (std::count(locationBlock.begin(), locationBlock.end(), true) > 2)
            {
                bool done = false;
                int next = 0;
                int b = r % 3 * 3 + c % 3;
                int r0 = r / 3 * 3;
                int c0 = c / 3 * 3;
                while (!done)
                {
                    int b2 = std::find(locationBlock.begin() + next, locationBlock.end(), true) - locationBlock.begin();
                    if (b2 == b)
                        next = b2 + 1;
                    else if (b2 == locationBlock.end() - locationBlock.begin())
                        done = true;
                    else
                    {
                        int r2 = r0 + b2 / 3;
                        int c2 = c0 + b2 % 3;
                        if ((c2 == startc and r2 == startr and chainLength > 2) or (chain[r2][c2] == 0 and find_chain(r2, c2, (colour == 1) ? 2 : 1, true, allow_disc)))
                        {
                            return true;
                        }
                        else
                        {
                            next = b2 + 1;
                            if (next == locationBlock.end() - locationBlock.begin())
                                done = true;
                        }
                    }
                }
            }
            if (dr == 10 and chainLength > 2 and allow_disc == true and (r == startr or c == startc or block_number(r, c) == block_number(startr, startc)))
            {
                dr = startr;
                dc = startc;
                ds = true;
                return true;
            }

            chain[r][c] = 0;
            chainLength--;
            if (dr == r and dc == c)
                dr = dc = 10;
            return false;
        };
        for (int i = 0; i < 9; i++)
        {
            for (int j = 0; j < 9; j++)
            {
                if (locations[i][j] == true)
                {
                    tested.clear();
                    bool foundCycle = false;
                    if (std::count(locations[i].begin(), locations[i].end(), true) == 2)
                    {
                        chain = {{0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}};
                        chainLength = 0;
                        dr = dc = 10;

                        startr = i;
                        startc = j;
                        std::vector<std::pair<int, int>> testedBackup = tested;
                        tested.clear();
                        if (find_chain(startr, startc, 1, true, false))
                        {
                            foundCycle = true;
                        }
                    }
                    if (!foundCycle and std::count(locationCols[j].begin(), locationCols[j].end(), true) == 2)
                    {
                        chain = {{0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}};
                        chainLength = 0;
                        dr = dc = 10;
                        startr = i;
                        startc = j;
                        std::vector<std::pair<int, int>> testedBackup = tested;
                        tested.clear();
                        if (find_chain(startr, startc, 1, true, false))
                        {
                            foundCycle = true;
                        }
                    }
                    std::vector<bool> locationBlock = get_block(locations, i, j);
                    if (!foundCycle and std::count(locationBlock.begin(), locationBlock.end(), true) == 2)
                    {
                        chain = {{0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}};
                        chainLength = 0;
                        dr = dc = 10;
                        startr = i;
                        startc = j;
                        tested.clear();
                        std::vector<std::pair<int, int>> testedBackup = tested;
                        if (find_chain(startr, startc, 1, true, false))
                        {
                            foundCycle = true;
                        }
                    }
                    if (!foundCycle and std::count(locations[i].begin(), locations[i].end(), true) == 2)
                    {
                        chain = {{0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}};
                        chainLength = 0;
                        dr = dc = 10;

                        startr = i;
                        startc = j;
                        std::vector<std::pair<int, int>> testedBackup = tested;
                        tested.clear();
                        if (find_chain(startr, startc, 1, true, true))
                        {
                            foundCycle = true;
                        }
                    }
                    if (!foundCycle and std::count(locationCols[j].begin(), locationCols[j].end(), true) == 2)
                    {
                        chain = {{0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}};
                        chainLength = 0;
                        dr = dc = 10;
                        startr = i;
                        startc = j;
                        std::vector<std::pair<int, int>> testedBackup = tested;
                        tested.clear();
                        if (find_chain(startr, startc, 1, true, true))
                        {
                            foundCycle = true;
                        }
                    }
                    if (!foundCycle and std::count(locationBlock.begin(), locationBlock.end(), true) == 2)
                    {
                        chain = {{0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}};
                        chainLength = 0;
                        dr = dc = 10;
                        startr = i;
                        startc = j;
                        tested.clear();
                        std::vector<std::pair<int, int>> testedBackup = tested;
                        if (find_chain(startr, startc, 1, true, true))
                        {
                            foundCycle = true;
                        }
                    }
                    if (foundCycle)
                    {
                        if (chainLength % 2 == 0) //we have a continous loop
                        {
                            std::vector<std::vector<int>> chainCols = chain;
                            transpose_matrix(chainCols);
                            for (int k = 0; k < 9; k++) //we check every row to see if it contains both a 1 and a 2
                            {
                                int c1 = std::find(chain[k].begin(), chain[k].end(), 1) - chain[k].begin();
                                int c2 = std::find(chain[k].begin(), chain[k].end(), 2) - chain[k].begin();
                                if (c1 != 9 and c2 != 9) //we found a row that contains both 1 and 2
                                {
                                    int r = k;
                                    for (int c = 0; c < 9; c++)
                                    {
                                        if (c != c1 and c != c2)
                                        {
                                            if (allOptionsCopy[r][c].erase(n))
                                                found = true;
                                        }
                                    }
                                }
                                int r1 = std::find(chainCols[k].begin(), chainCols[k].end(), 1) - chainCols[k].begin();
                                int r2 = std::find(chainCols[k].begin(), chainCols[k].end(), 2) - chainCols[k].begin();
                                if (r1 != 9 and r2 != 9) //we found a row that contains both 1 and 2
                                {
                                    int c = k;
                                    for (int r = 0; r < 9; r++)
                                    {
                                        if (r != r1 and r != r2)
                                        {
                                            if (allOptionsCopy[r][c].erase(n))
                                                found = true;
                                        }
                                    }
                                }
                                std::vector<int> chainBlock = get_block(chain, k);
                                int b1 = std::find(chainBlock.begin(), chainBlock.end(), 1) - chainBlock.begin();
                                int b2 = std::find(chainBlock.begin(), chainBlock.end(), 2) - chainBlock.begin();
                                if (b1 != 9 and b2 != 9) //we found a row that contains both 1 and 2
                                {
                                    int r0 = k / 3 * 3;
                                    int c0 = k % 3 * 3;
                                    int r1 = r0 + b1 / 3;
                                    int c1 = c0 + b1 % 3;
                                    int r2 = r0 + b2 / 3;
                                    int c2 = c0 + b2 % 3;
                                    for (int r = r0; r < r0 + 3; r++)
                                    {
                                        for (int c = c0; c < c0 + 3; c++)
                                        {
                                            if (!(r == r1 and c == c1) and !(r == r2 and c == c2))
                                            {
                                                if (allOptionsCopy[r][c].erase(n))
                                                    found = true;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                        else if (dr != 10 and ds == true) //we have case 2
                        {
                            allOptionsCopy[dr][dc] = {n};
                            found = true;
                        }
                        else //we have case 3
                        {
                            allOptionsCopy[dr][dc].erase(n);
                            found = true;
                        }
                    }
                }
            }
        }
    }

    if (found)
    {
        found = (allOptions != allOptionsCopy);
        allOptions = allOptionsCopy;
    }
    return found;
}

bool Sudoku::xy_chain()
{
    bool found = false;
    std::vector<std::vector<std::unordered_set<int>>> allOptionsCopy = allOptions; //first make working copies of all options

    std::vector<std::vector<int>> optionsCount = get_options_count();
    std::vector<std::vector<int>> optionsCountCols = optionsCount;
    transpose_matrix(optionsCountCols);
    std::vector<std::vector<bool>> chain = {{0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}};
    int chainLength = 0;
    std::function<std::vector<std::pair<int, int>>(int, int, int, int, std::vector<std::pair<int, int>>)> find_chain;
    find_chain = [&](int n, int r, int c, int gn, std::vector<std::pair<int, int>> onchain) {
        std::vector<std::pair<int, int>> goals;
        chain[r][c] = true;
        onchain.push_back(std::make_pair(r, c));
        if (onchain.size() > 4 and n == gn)
        {
            goals.push_back(std::make_pair(r, c));
        }
        std::vector<std::pair<int, int>> seen = list_seen_by(r, c);
        for (std::pair<int, int> rc : seen)
        {
            if (optionsCount[rc.first][rc.second] == 2 and chain[rc.first][rc.second] == 0 and find(onchain.begin(), onchain.end(), rc) == onchain.end() and allOptions[rc.first][rc.second].count(n) == 1)
            {
                std::vector<int> options;
                options.insert(options.end(), allOptions[rc.first][rc.second].begin(), allOptions[rc.first][rc.second].end());
                int nextN = (options[0] == n) ? options[1] : options[0];
                std::vector<std::pair<int, int>> exits = find_chain(nextN, rc.first, rc.second, gn, onchain);
                if (exits.size() > 0)
                    goals.insert(goals.end(), exits.begin(), exits.end());
            }
        }

        //chain[r][c] = false;
        return goals;
    };

    for (int r = 0; r < 9 and found == false; r++)
    {
        for (int c = 0; c < 9 and found == false; c++)
        {
            if (optionsCount[r][c] == 2)
            {
                std::vector<int> options;
                options.insert(options.end(), allOptions[r][c].begin(), allOptions[r][c].end());
                int n1 = options[0];
                int n2 = options[1];
                std::vector<std::pair<int, int>> remove1, remove2;
                chain = {{0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}};
                chainLength = 0;
                std::vector<std::pair<int, int>> goals1 = find_chain(n2, r, c, n1, {});
                sort(goals1.begin(), goals1.end());
                goals1.erase(unique(goals1.begin(), goals1.end()), goals1.end());
                if (goals1.size() > 0)
                {
                    for (std::pair<int, int> rc : goals1)
                    {
                        std::vector<std::pair<int, int>> remove = list_seen_by(r, c, rc.first, rc.second);
                        remove1.insert(remove1.end(), remove.begin(), remove.end());
                    }
                    sort(remove1.begin(), remove1.end());
                    remove1.erase(unique(remove1.begin(), remove1.end()), remove1.end());
                }
                chain = {{0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}};
                chainLength = 0;
                std::vector<std::pair<int, int>> goals2 = find_chain(n1, r, c, n2, {});
                sort(goals2.begin(), goals2.end());
                goals2.erase(unique(goals2.begin(), goals2.end()), goals2.end());
                if (goals2.size() > 0)
                {
                    for (std::pair<int, int> rc : goals2)
                    {
                        std::vector<std::pair<int, int>> remove = list_seen_by(r, c, rc.first, rc.second);
                        remove2.insert(remove2.end(), remove.begin(), remove.end());
                    }
                    sort(remove2.begin(), remove2.end());
                    remove2.erase(unique(remove2.begin(), remove2.end()), remove2.end());
                }
                for (std::pair<int, int> rm : remove1)
                {
                    if (!(rm.first == r and rm.second == c) and (find(goals1.begin(), goals1.end(), rm) == goals1.end()))
                    {
                        if (allOptionsCopy[rm.first][rm.second].erase(n1))
                            found = true;
                    }
                }

                for (std::pair<int, int> rm : remove2)
                {
                    if (!(rm.first == r and rm.second == c) and (find(goals2.begin(), goals2.end(), rm) == goals2.end()))
                    {
                        if (allOptionsCopy[rm.first][rm.second].erase(n2))
                            found = true;
                    }
                }
            }
        }
    }

    if (found)
    {
        found = (allOptions != allOptionsCopy);
        allOptions = allOptionsCopy;
    }
    return found;
}

bool Sudoku::three_d_medusa()
{
    bool found = false;
    std::vector<std::vector<std::unordered_set<int>>> allOptionsCopy = allOptions;
    std::vector<std::vector<std::vector<std::unordered_set<int>>>> chain; //in order chain[color][row][col]
    std::vector<std::vector<std::pair<int, int>>> inChain = {{}, {}};
    std::unordered_set<int> tested;
    int remove = -1;
    int chainLength = 0;
    std::function<void(int, int, int, int)> find_chain;
    find_chain = [&](int r, int c, int n, int colour) { //I don't want to make a function I only use once, but I need recursion so I'm making a lambda
        chain[colour][r][c].insert(n);
        if (chain[colour][r][c].size() > 1)
            remove = colour; //if there is more than 1 item in the set, the colour is to be removed (rule1)
        chainLength++;
        inChain[colour].push_back(std::make_pair(r, c));
        tested.insert(r * 9 + c);
        int colour2 = (colour == 0) ? 1 : 0;
        if (allOptions[r][c].size() == 2)
        {
            std::vector<int> options;
            options.insert(options.end(), allOptions[r][c].begin(), allOptions[r][c].end());
            int n2 = (options[0] == n) ? options[1] : options[0];
            if (chain[colour2][r][c].count(n2) == 0 and chain[colour][r][c].count(n2) == 0)
                find_chain(r, c, n2, colour2);
        }
        std::vector<bool> locationsRow = get_possible_locations_row(n, r);

        if (count(locationsRow.begin(), locationsRow.end(), true) == 2)
        {
            int c2 = find(locationsRow.begin(), locationsRow.end(), true) - locationsRow.begin();
            if (c2 == c)
                c2 = find(locationsRow.begin() + c + 1, locationsRow.end(), true) - locationsRow.begin();
            if (chain[colour2][r][c2].count(n) == 0 and chain[colour][r][c2].count(n) == 0)
                find_chain(r, c2, n, colour2);
        }
        std::vector<bool> locationsCol = get_possible_locations_col(n, c);
        if (count(locationsCol.begin(), locationsCol.end(), true) == 2)
        {
            int r2 = find(locationsCol.begin(), locationsCol.end(), true) - locationsCol.begin();
            if (r2 == r)
                r2 = find(locationsCol.begin() + r + 1, locationsCol.end(), true) - locationsCol.begin();
            if (chain[colour2][r2][c].count(n) == 0 and chain[colour][r2][c].count(n) == 0)
                find_chain(r2, c, n, colour2);
        }
        std::vector<bool> locationsBlock = get_possible_locations_block(n, r, c);
        if (count(locationsBlock.begin(), locationsBlock.end(), true) == 2)
        {
            int b1 = r / 3 * 3 + c % 3;
            int b2 = find(locationsBlock.begin(), locationsBlock.end(), true) - locationsBlock.begin();
            if (b2 == b1)
                b2 = find(locationsBlock.begin() + b1 + 1, locationsBlock.end(), true) - locationsBlock.begin();
            int r0 = r / 3 * 3;
            int c0 = c / 3 * 3;
            int r2 = r0 + b2 / 3;
            int c2 = c0 + b2 % 3;
            if (chain[colour2][r2][c2].count(n) == 0 and chain[colour][r2][c2].count(n) == 0)
                find_chain(r2, c2, n, colour2);
        }
    };
    for (int n = 1; n < 10; n++) //going from 1 through 9, because we're looking for numbers, not rows or cols
    {
        std::vector<std::vector<bool>> locations = get_possible_locations(n); //get the list of possible locations for number i
        std::vector<std::vector<bool>> locationCols = locations;              //transposed version to save some calculations later on.
        transpose_matrix(locationCols);
        chain = {{{{}, {}, {}, {}, {}, {}, {}, {}, {}}, {{}, {}, {}, {}, {}, {}, {}, {}, {}}, {{}, {}, {}, {}, {}, {}, {}, {}, {}}, {{}, {}, {}, {}, {}, {}, {}, {}, {}}, {{}, {}, {}, {}, {}, {}, {}, {}, {}}, {{}, {}, {}, {}, {}, {}, {}, {}, {}}, {{}, {}, {}, {}, {}, {}, {}, {}, {}}, {{}, {}, {}, {}, {}, {}, {}, {}, {}}, {{}, {}, {}, {}, {}, {}, {}, {}, {}}}, {{{}, {}, {}, {}, {}, {}, {}, {}, {}}, {{}, {}, {}, {}, {}, {}, {}, {}, {}}, {{}, {}, {}, {}, {}, {}, {}, {}, {}}, {{}, {}, {}, {}, {}, {}, {}, {}, {}}, {{}, {}, {}, {}, {}, {}, {}, {}, {}}, {{}, {}, {}, {}, {}, {}, {}, {}, {}}, {{}, {}, {}, {}, {}, {}, {}, {}, {}}, {{}, {}, {}, {}, {}, {}, {}, {}, {}}, {{}, {}, {}, {}, {}, {}, {}, {}, {}}}};
        chainLength = 0;
        remove = -1;
        inChain[0].clear();
        inChain[1].clear();
        for (int i = 0; i < 9; i++)
        {
            bool foundPair = false;
            std::vector<bool> locationsBlock = get_block(locations, i);
            if (count(locations[i].begin(), locations[i].end(), true) == 2)
            {
                int r = i;
                int c = find(locations[i].begin(), locations[i].end(), true) - locations[i].begin();
                if (tested.count(r * 9 + c) == 0)
                {
                    chain = {{{{}, {}, {}, {}, {}, {}, {}, {}, {}}, {{}, {}, {}, {}, {}, {}, {}, {}, {}}, {{}, {}, {}, {}, {}, {}, {}, {}, {}}, {{}, {}, {}, {}, {}, {}, {}, {}, {}}, {{}, {}, {}, {}, {}, {}, {}, {}, {}}, {{}, {}, {}, {}, {}, {}, {}, {}, {}}, {{}, {}, {}, {}, {}, {}, {}, {}, {}}, {{}, {}, {}, {}, {}, {}, {}, {}, {}}, {{}, {}, {}, {}, {}, {}, {}, {}, {}}}, {{{}, {}, {}, {}, {}, {}, {}, {}, {}}, {{}, {}, {}, {}, {}, {}, {}, {}, {}}, {{}, {}, {}, {}, {}, {}, {}, {}, {}}, {{}, {}, {}, {}, {}, {}, {}, {}, {}}, {{}, {}, {}, {}, {}, {}, {}, {}, {}}, {{}, {}, {}, {}, {}, {}, {}, {}, {}}, {{}, {}, {}, {}, {}, {}, {}, {}, {}}, {{}, {}, {}, {}, {}, {}, {}, {}, {}}, {{}, {}, {}, {}, {}, {}, {}, {}, {}}}};
                    chainLength = 0;
                    remove = -1;
                    inChain[0].clear();
                    inChain[1].clear();
                    find_chain(r, c, n, 0);
                    if (chainLength > 3)
                        foundPair = true;
                }
            }
            else if (count(locationCols[i].begin(), locationCols[i].end(), true) == 2)
            {
                int c = i;
                int r = find(locationCols[i].begin(), locationCols[i].end(), true) - locationCols[i].begin();
                if (tested.count(r * 9 + c) == 0)
                {
                    chain = {{{{}, {}, {}, {}, {}, {}, {}, {}, {}}, {{}, {}, {}, {}, {}, {}, {}, {}, {}}, {{}, {}, {}, {}, {}, {}, {}, {}, {}}, {{}, {}, {}, {}, {}, {}, {}, {}, {}}, {{}, {}, {}, {}, {}, {}, {}, {}, {}}, {{}, {}, {}, {}, {}, {}, {}, {}, {}}, {{}, {}, {}, {}, {}, {}, {}, {}, {}}, {{}, {}, {}, {}, {}, {}, {}, {}, {}}, {{}, {}, {}, {}, {}, {}, {}, {}, {}}}, {{{}, {}, {}, {}, {}, {}, {}, {}, {}}, {{}, {}, {}, {}, {}, {}, {}, {}, {}}, {{}, {}, {}, {}, {}, {}, {}, {}, {}}, {{}, {}, {}, {}, {}, {}, {}, {}, {}}, {{}, {}, {}, {}, {}, {}, {}, {}, {}}, {{}, {}, {}, {}, {}, {}, {}, {}, {}}, {{}, {}, {}, {}, {}, {}, {}, {}, {}}, {{}, {}, {}, {}, {}, {}, {}, {}, {}}, {{}, {}, {}, {}, {}, {}, {}, {}, {}}}};
                    chainLength = 0;
                    remove = -1;
                    inChain[0].clear();
                    inChain[1].clear();
                    find_chain(r, c, n, 0);
                    if (chainLength > 3)
                        foundPair = true;
                }
            }
            else if (count(locationsBlock.begin(), locationsBlock.end(), true) == 2)
            {
                int bn = i;
                int b = find(locationsBlock.begin(), locationsBlock.end(), true) - locationsBlock.begin();
                int r0 = bn / 3 * 3;
                int c0 = bn % 3 * 3;
                int r = r0 + b / 3;
                int c = c0 + b % 3;
                if (tested.count(r * 9 + c) == 0)
                {
                    chain = {{{{}, {}, {}, {}, {}, {}, {}, {}, {}}, {{}, {}, {}, {}, {}, {}, {}, {}, {}}, {{}, {}, {}, {}, {}, {}, {}, {}, {}}, {{}, {}, {}, {}, {}, {}, {}, {}, {}}, {{}, {}, {}, {}, {}, {}, {}, {}, {}}, {{}, {}, {}, {}, {}, {}, {}, {}, {}}, {{}, {}, {}, {}, {}, {}, {}, {}, {}}, {{}, {}, {}, {}, {}, {}, {}, {}, {}}, {{}, {}, {}, {}, {}, {}, {}, {}, {}}}, {{{}, {}, {}, {}, {}, {}, {}, {}, {}}, {{}, {}, {}, {}, {}, {}, {}, {}, {}}, {{}, {}, {}, {}, {}, {}, {}, {}, {}}, {{}, {}, {}, {}, {}, {}, {}, {}, {}}, {{}, {}, {}, {}, {}, {}, {}, {}, {}}, {{}, {}, {}, {}, {}, {}, {}, {}, {}}, {{}, {}, {}, {}, {}, {}, {}, {}, {}}, {{}, {}, {}, {}, {}, {}, {}, {}, {}}, {{}, {}, {}, {}, {}, {}, {}, {}, {}}}};
                    chainLength = 0;
                    remove = -1;
                    inChain[0].clear();
                    inChain[1].clear();
                    find_chain(r, c, n, 0);
                    if (chainLength > 3)
                        foundPair = true;
                }
            }
            if (foundPair)
            {
                if (remove != -1) //let's start with rule 1
                {
                    int keep = (remove == 0) ? 1 : 0;
                    for (std::pair<int, int> rc : inChain[remove])
                    {
                        for (int rm : chain[remove][rc.first][rc.second])
                        {
                            if (allOptionsCopy[rc.first][rc.second].erase(rm))
                                found = true;
                        }
                    }
                    for (std::pair<int, int> rc : inChain[keep])
                    {
                        allOptions[rc.first][rc.second] = {*chain[keep][rc.first][rc.second].begin()};
                    }
                }
                else //I can't test rule 2 in one if statement, so I have to do that with nested else if;
                {
                    //we now know every square has at most one number per color, so let's remove the sets;
                    std::vector<std::vector<std::vector<bool>>> inChain2d = {{{0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}}, {{0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}}};
                    std::vector<std::vector<std::vector<int>>> chainNumbers = {{{0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}}, {{0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}}};
                    for (std::pair<int, int> rc : inChain[0])
                    {
                        inChain2d[0][rc.first][rc.second] = true;
                        chainNumbers[0][rc.first][rc.second] = *chain[0][rc.first][rc.second].begin();
                    }
                    for (std::pair<int, int> rc : inChain[1])
                    {
                        inChain2d[1][rc.first][rc.second] = true;
                        chainNumbers[1][rc.first][rc.second] = *chain[1][rc.first][rc.second].begin();
                    }
                    std::vector<std::vector<std::vector<bool>>> inChain2dCols = inChain2d;
                    std::vector<std::vector<std::vector<int>>> chainNumbersCols = chainNumbers;
                    transpose_matrix(inChain2dCols[0]);
                    transpose_matrix(inChain2dCols[1]);
                    transpose_matrix(chainNumbersCols[0]);
                    transpose_matrix(chainNumbersCols[1]);
                    for (std::pair<int, int> rc : inChain[0])
                    {
                        std::vector<std::vector<int>> chainNumbersBlock = {get_block(chainNumbers[0], rc.first, rc.second), get_block(chainNumbers[1], rc.first, rc.second)};
                        int rn = chainNumbers[0][rc.first][rc.second];
                        if (count(chainNumbers[0][rc.first].begin(), chainNumbers[0][rc.first].end(), rn) > 1 or count(chainNumbersCols[0][rc.second].begin(), chainNumbersCols[0][rc.second].end(), rn) > 1 or count(chainNumbersBlock[0].begin(), chainNumbersBlock[0].end(), rn) > 1)
                        {
                            remove = 0;
                        }
                    }
                    for (std::pair<int, int> rc : inChain[1])
                    {
                        std::vector<std::vector<int>> chainNumbersBlock = {get_block(chainNumbers[0], rc.first, rc.second), get_block(chainNumbers[1], rc.first, rc.second)};
                        int rn = chainNumbers[1][rc.first][rc.second];
                        if (count(chainNumbers[1][rc.first].begin(), chainNumbers[1][rc.first].end(), rn) > 1 or count(chainNumbersCols[1][rc.second].begin(), chainNumbersCols[1][rc.second].end(), rn) > 1 or count(chainNumbersBlock[1].begin(), chainNumbersBlock[1].end(), rn) > 1)
                        {
                            remove = 1;
                        }
                    }
                    if (remove != -1) //we found a colour to remove with rule 2
                    {
                        int keep = (remove == 0) ? 1 : 0;
                        for (std::pair<int, int> rc : inChain[remove])
                        {

                            if (allOptionsCopy[rc.first][rc.second].erase(chainNumbers[remove][rc.first][rc.second]))
                                found = true;
                        }
                        for (std::pair<int, int> rc : inChain[keep])
                        {
                            allOptionsCopy[rc.first][rc.second] = {chainNumbers[keep][rc.first][rc.second]};
                        }
                    }
                    else // rule 3, 4, 5 and 6;
                    {
                        for (std::pair<int, int> rc : inChain[0]) //rule 3;
                        {
                            if (inChain2d[1][rc.first][rc.second] == true) //if a square is both in chain 1 and 2;
                            {
                                if (allOptions[rc.first][rc.second].size() > 2) //if there is a 3rd number it can be removed.
                                {
                                    allOptionsCopy[rc.first][rc.second] = {chainNumbers[0][rc.first][rc.second], chainNumbers[1][rc.first][rc.second]};
                                    found = true;
                                }
                            }
                        }
                        for (int r = 0; r < 9; r++) //for rule 4 we go over every single square.
                        {
                            for (int c = 0; c < 9; c++)
                            {
                                if (puzzle[r][c] == 0 and inChain2d[0][r][c] == false and inChain2d[1][r][c] == false) //we're only checking numbers not in the chain
                                {
                                    std::vector<std::vector<int>> chainNumberBlocks = {get_block(chainNumbers[0], r, c), get_block(chainNumbers[1], r, c)};
                                    for (int rn : allOptions[r][c]) //if one of the options is also seen in the same row, col or block, with both colors, remove it.
                                    {
                                        if ((find(chainNumbers[0][r].begin(), chainNumbers[0][r].end(), rn) != chainNumbers[0][r].end() or find(chainNumbersCols[0][c].begin(), chainNumbersCols[0][c].end(), rn) != chainNumbersCols[0][c].end() or find(chainNumberBlocks[0].begin(), chainNumberBlocks[0].end(), rn) != chainNumberBlocks[0].end()) and (find(chainNumbers[1][r].begin(), chainNumbers[1][r].end(), rn) != chainNumbers[1][r].end() or find(chainNumbersCols[1][c].begin(), chainNumbersCols[1][c].end(), rn) != chainNumbersCols[1][c].end() or find(chainNumberBlocks[1].begin(), chainNumberBlocks[1].end(), rn) != chainNumberBlocks[1].end()))
                                        {
                                            allOptionsCopy[r][c].erase(rn);
                                            found = true;
                                        }
                                    }
                                }
                            }
                        }
                        for (std::pair<int, int> rc : inChain[0]) //for rule 5 we go over every square that's in one color but not the other. Starting with color 0
                        {
                            if (inChain2d[1][rc.first][rc.second] == false)
                            {
                                std::vector<std::vector<int>> chainNumberBlocks = {get_block(chainNumbers[0], rc.first, rc.second), get_block(chainNumbers[1], rc.first, rc.second)};
                                for (int rn : allOptions[rc.first][rc.second])
                                {
                                    if (chainNumbers[0][rc.first][rc.second] != rn)
                                    {
                                        if (find(chainNumbers[1][rc.first].begin(), chainNumbers[1][rc.first].end(), rn) != chainNumbers[1][rc.first].end() or find(chainNumbersCols[1][rc.second].begin(), chainNumbersCols[1][rc.second].end(), rn) != chainNumbersCols[1][rc.second].end() or find(chainNumberBlocks[1].begin(), chainNumberBlocks[1].end(), rn) != chainNumberBlocks[1].end())
                                        {
                                            allOptionsCopy[rc.first][rc.second].erase(rn);
                                            found = true;
                                        }
                                    }
                                }
                            }
                        }
                        for (std::pair<int, int> rc : inChain[1]) //for rule 5 we go over every square that's in one color but not the other. color 1 next
                        {
                            if (inChain2d[0][rc.first][rc.second] == false)
                            {
                                std::vector<std::vector<int>> chainNumberBlocks = {get_block(chainNumbers[0], rc.first, rc.second), get_block(chainNumbers[1], rc.first, rc.second)};
                                for (int rn : allOptions[rc.first][rc.second])
                                {
                                    if (chainNumbers[1][rc.first][rc.second] != rn)
                                    {
                                        if (find(chainNumbers[0][rc.first].begin(), chainNumbers[0][rc.first].end(), rn) != chainNumbers[0][rc.first].end() or find(chainNumbersCols[0][rc.second].begin(), chainNumbersCols[0][rc.second].end(), rn) != chainNumbersCols[0][rc.second].end() or find(chainNumberBlocks[0].begin(), chainNumberBlocks[0].end(), rn) != chainNumberBlocks[0].end())
                                        {
                                            allOptionsCopy[rc.first][rc.second].erase(rn);
                                            found = true;
                                        }
                                    }
                                }
                            }
                        }
                        for (int r = 0; r < 9 and remove == -1; r++) //for rule 6 we go over every single square again. (todo, combine 4 and 6 to save time)
                        {
                            for (int c = 0; c < 9 and remove == -1; c++)
                            {
                                if (puzzle[r][c] == 0 and inChain2d[0][r][c] == false and inChain2d[1][r][c] == false) //we're only checking numbers not in the chain
                                {
                                    std::vector<std::vector<int>> chainNumberBlocks = {get_block(chainNumbers[0], r, c), get_block(chainNumbers[1], r, c)};
                                    bool notEmpty = false;
                                    for (int rn : allOptions[r][c])
                                    {
                                        if (find(chainNumbers[0][r].begin(), chainNumbers[0][r].end(), rn) == chainNumbers[0][r].end() and find(chainNumbersCols[0][c].begin(), chainNumbersCols[0][c].end(), rn) == chainNumbersCols[0][c].end() and find(chainNumberBlocks[0].begin(), chainNumberBlocks[0].end(), rn) == chainNumberBlocks[0].end())
                                        {
                                            notEmpty = true;
                                            break;
                                        }
                                    }
                                    if (notEmpty == false)
                                    {
                                        remove = 0;
                                    }
                                    else
                                    {
                                        notEmpty = false;
                                        for (int rn : allOptions[r][c])
                                        {
                                            if (find(chainNumbers[1][r].begin(), chainNumbers[1][r].end(), rn) == chainNumbers[1][r].end() and find(chainNumbersCols[1][c].begin(), chainNumbersCols[1][c].end(), rn) == chainNumbersCols[1][c].end() and find(chainNumberBlocks[1].begin(), chainNumberBlocks[1].end(), rn) == chainNumberBlocks[1].end())
                                            {
                                                notEmpty = true;
                                                break;
                                            }
                                        }
                                        if (notEmpty == false)
                                        {
                                            remove = 1;
                                        }
                                    }
                                    if (remove != -1)
                                    {
                                        int keep = (remove == 0) ? 1 : 0;
                                        for (std::pair<int, int> rc : inChain[remove])
                                        {

                                            if (allOptionsCopy[rc.first][rc.second].erase(chainNumbers[remove][rc.first][rc.second]))
                                                found = true;
                                        }
                                        for (std::pair<int, int> rc : inChain[keep])
                                        {
                                            allOptionsCopy[rc.first][rc.second] = {chainNumbers[keep][rc.first][rc.second]};
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    if (found)
    {
        found = (allOptions != allOptionsCopy);
        allOptions = allOptionsCopy;
    }
    return found;
}

bool Sudoku::jellyfish()
{
    bool found = false;
    std::vector<std::vector<std::unordered_set<int>>> allOptionsCopy = allOptions;

    for (int n = 1; n < 10; n++)
    {
        std::vector<std::vector<bool>> locations = get_possible_locations(n);
        int r1, r2, r3, r4, c1, c2, c3, c4;
        r1 = r2 = r3 = r4 = c1 = c2 = c3 = c4 = 10;
        std::vector<int> rs = {10, 10, 10, 10};
        std::vector<int> cs = {10, 10, 10, 10};
        for (int i = 0; i < 6 and r4 == 10; i++)
        {
            int numLocations = count(locations[i].begin(), locations[i].end(), true);
            if (numLocations >= 2 and numLocations <= 4)
            {
                bool noJellyfish = false;
                std::vector<int> csCopy1 = cs;
                for (int m = 0; m < 9; m++)
                {
                    if (locations[i][m] == true)
                    {
                        if (csCopy1[0] == 10 or csCopy1[0] == m)
                            csCopy1[0] = m;
                        else if (csCopy1[1] == 10 or csCopy1[1] == m)
                            csCopy1[1] = m;
                        else if (csCopy1[2] == 10 or csCopy1[2] == m)
                            csCopy1[2] = m;
                        else if (csCopy1[3] == 10 or csCopy1[3] == m)
                            csCopy1[3] = m;
                        else
                        {
                            noJellyfish = true;
                            break;
                        }
                    }
                }
                if (noJellyfish == false)
                {
                    r1 = i;
                    for (int j = i + 1; j < 7 and r4 == 10; j++)
                    {
                        numLocations = count(locations[j].begin(), locations[j].end(), true);
                        if (numLocations >= 2 and numLocations <= 4)
                        {
                            noJellyfish = false;
                            std::vector<int> csCopy2 = csCopy1;
                            for (int m = 0; m < 9; m++)
                            {
                                if (locations[j][m] == true)
                                {
                                    if (csCopy2[0] == 10 or csCopy2[0] == m)
                                        csCopy2[0] = m;
                                    else if (csCopy2[1] == 10 or csCopy2[1] == m)
                                        csCopy2[1] = m;
                                    else if (csCopy2[2] == 10 or csCopy2[2] == m)
                                        csCopy2[2] = m;
                                    else if (csCopy2[3] == 10 or csCopy2[3] == m)
                                        csCopy2[3] = m;
                                    else
                                    {
                                        noJellyfish = true;
                                        break;
                                    }
                                }
                            }
                            if (noJellyfish == false)
                            {
                                r2 = j;
                                for (int k = j + 1; k < 8 and r4 == 10; k++)
                                {
                                    numLocations = count(locations[k].begin(), locations[k].end(), true);
                                    if (numLocations >= 2 and numLocations <= 4)
                                    {
                                        noJellyfish = false;
                                        std::vector<int> csCopy3 = csCopy2;
                                        for (int m = 0; m < 9; m++)
                                        {
                                            if (locations[k][m] == true)
                                            {
                                                if (csCopy3[0] == 10 or csCopy3[0] == m)
                                                    csCopy3[0] = m;
                                                else if (csCopy3[1] == 10 or csCopy3[1] == m)
                                                    csCopy3[1] = m;
                                                else if (csCopy3[2] == 10 or csCopy3[2] == m)
                                                    csCopy3[2] = m;
                                                else if (csCopy3[3] == 10 or csCopy3[3] == m)
                                                    csCopy3[3] = m;
                                                else
                                                {
                                                    noJellyfish = true;
                                                    break;
                                                }
                                            }
                                        }
                                        if (noJellyfish == false and csCopy3[3] != 10)
                                        {
                                            r3 = k;
                                            for (int l = k + 1; l < 9 and r4 == 10; l++)
                                            {
                                                numLocations = count(locations[l].begin(), locations[l].end(), true);
                                                if (numLocations >= 2 and numLocations <= 4)
                                                {
                                                    noJellyfish = false;
                                                    std::vector<int> csCopy4 = csCopy3;
                                                    for (int m = 0; m < 9; m++)
                                                    {
                                                        if (locations[l][m] == true)
                                                        {
                                                            if (csCopy4[0] == 10 or csCopy4[0] == m)
                                                                csCopy4[0] = m;
                                                            else if (csCopy4[1] == 10 or csCopy4[1] == m)
                                                                csCopy4[1] = m;
                                                            else if (csCopy4[2] == 10 or csCopy4[2] == m)
                                                                csCopy4[2] = m;
                                                            else if (csCopy4[3] == 10 or csCopy4[3] == m)
                                                                csCopy4[3] = m;
                                                            else
                                                            {
                                                                noJellyfish = true;
                                                                break;
                                                            }
                                                        }
                                                    }
                                                    if (noJellyfish == false)
                                                    {
                                                        r4 = l;
                                                        c1 = csCopy4[0];
                                                        c2 = csCopy4[1];
                                                        c3 = csCopy4[2];
                                                        c4 = csCopy4[3];
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        if (r4 != 10) //We found a jellyfish!
        {
            for (int i = 0; i < 9; i++)
            {
                if (i != r1 and i != r2 and i != r3 and i != r4)
                {
                    if (allOptionsCopy[i][c1].erase(n) == 1)
                        found = true;
                    if (allOptionsCopy[i][c2].erase(n) == 1)
                        found = true;
                    if (allOptionsCopy[i][c3].erase(n) == 1)
                        found = true;
                    if (allOptionsCopy[i][c4].erase(n) == 1)
                        found = true;
                }
            }
        }
        else
        {
            std::vector<std::vector<bool>> locationCols = locations;
            transpose_matrix(locationCols); //todo, change everything to work with cols
            r1 = r2 = r3 = r4 = c1 = c2 = c3 = c4 = 10;
            rs = {10, 10, 10, 10};
            cs = {10, 10, 10, 10};
            for (int i = 0; i < 6 and c4 == 10; i++)
            {
                int numLocations = count(locationCols[i].begin(), locationCols[i].end(), true);
                if (numLocations >= 2 and numLocations <= 4)
                {
                    bool noJellyfish = false;
                    std::vector<int> rsCopy1 = rs;
                    for (int m = 0; m < 9; m++)
                    {
                        if (locationCols[i][m] == true)
                        {
                            if (rsCopy1[0] == 10 or rsCopy1[0] == m)
                                rsCopy1[0] = m;
                            else if (rsCopy1[1] == 10 or rsCopy1[1] == m)
                                rsCopy1[1] = m;
                            else if (rsCopy1[2] == 10 or rsCopy1[2] == m)
                                rsCopy1[2] = m;
                            else if (rsCopy1[3] == 10 or rsCopy1[3] == m)
                                rsCopy1[3] = m;
                            else
                            {
                                noJellyfish = true;
                                break;
                            }
                        }
                    }
                    if (noJellyfish == false)
                    {
                        c1 = i;
                        for (int j = i + 1; j < 7 and c4 == 10; j++)
                        {
                            numLocations = count(locationCols[j].begin(), locationCols[j].end(), true);
                            if (numLocations >= 2 and numLocations <= 4)
                            {
                                noJellyfish = false;
                                std::vector<int> rsCopy2 = rsCopy1;
                                for (int m = 0; m < 9; m++)
                                {
                                    if (locationCols[j][m] == true)
                                    {
                                        if (rsCopy2[0] == 10 or rsCopy2[0] == m)
                                            rsCopy2[0] = m;
                                        else if (rsCopy2[1] == 10 or rsCopy2[1] == m)
                                            rsCopy2[1] = m;
                                        else if (rsCopy2[2] == 10 or rsCopy2[2] == m)
                                            rsCopy2[2] = m;
                                        else if (rsCopy2[3] == 10 or rsCopy2[3] == m)
                                            rsCopy2[3] = m;
                                        else
                                        {
                                            noJellyfish = true;
                                            break;
                                        }
                                    }
                                }
                                if (noJellyfish == false)
                                {
                                    c2 = j;
                                    for (int k = j + 1; k < 8 and c4 == 10; k++)
                                    {
                                        numLocations = count(locationCols[k].begin(), locationCols[k].end(), true);
                                        if (numLocations >= 2 and numLocations <= 4)
                                        {
                                            noJellyfish = false;
                                            std::vector<int> rsCopy3 = rsCopy2;
                                            for (int m = 0; m < 9; m++)
                                            {
                                                if (locationCols[k][m] == true)
                                                {
                                                    if (rsCopy3[0] == 10 or rsCopy3[0] == m)
                                                        rsCopy3[0] = m;
                                                    else if (rsCopy3[1] == 10 or rsCopy3[1] == m)
                                                        rsCopy3[1] = m;
                                                    else if (rsCopy3[2] == 10 or rsCopy3[2] == m)
                                                        rsCopy3[2] = m;
                                                    else if (rsCopy3[3] == 10 or rsCopy3[3] == m)
                                                        rsCopy3[3] = m;
                                                    else
                                                    {
                                                        noJellyfish = true;
                                                        break;
                                                    }
                                                }
                                            }
                                            if (noJellyfish == false and rsCopy3[3] != 10)
                                            {
                                                c3 = k;
                                                for (int l = k + 1; l < 9 and c4 == 10; l++)
                                                {
                                                    numLocations = count(locationCols[l].begin(), locationCols[l].end(), true);
                                                    if (numLocations >= 2 and numLocations <= 4)
                                                    {
                                                        noJellyfish = false;
                                                        std::vector<int> rsCopy4 = rsCopy3;
                                                        for (int m = 0; m < 9; m++)
                                                        {
                                                            if (locationCols[l][m] == true)
                                                            {
                                                                if (rsCopy4[0] == 10 or rsCopy4[0] == m)
                                                                    rsCopy4[0] = m;
                                                                else if (rsCopy4[1] == 10 or rsCopy4[1] == m)
                                                                    rsCopy4[1] = m;
                                                                else if (rsCopy4[2] == 10 or rsCopy4[2] == m)
                                                                    rsCopy4[2] = m;
                                                                else if (rsCopy4[3] == 10 or rsCopy4[3] == m)
                                                                    rsCopy4[3] = m;
                                                                else
                                                                {
                                                                    noJellyfish = true;
                                                                    break;
                                                                }
                                                            }
                                                        }
                                                        if (noJellyfish == false)
                                                        {
                                                            c4 = l;
                                                            r1 = rsCopy4[0];
                                                            r2 = rsCopy4[1];
                                                            r3 = rsCopy4[2];
                                                            r4 = rsCopy4[3];
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            if (c4 != 10) //We found a jellyfish!
            {
                for (int i = 0; i < 9; i++)
                {
                    if (i != c1 and i != c2 and i != c3 and i != c4)
                    {
                        if (allOptionsCopy[r1][i].erase(n) == 1)
                            found = true;
                        if (allOptionsCopy[r2][i].erase(n) == 1)
                            found = true;
                        if (allOptionsCopy[r3][i].erase(n) == 1)
                            found = true;
                        if (allOptionsCopy[r4][i].erase(n) == 1)
                            found = true;
                    }
                }
            }
        }
    }

    if (found)
    {
        found = (allOptions != allOptionsCopy);
        allOptions = allOptionsCopy;
    }
    return found;
}

bool Sudoku::unique_rectangles()
{
    bool found = false;
    std::vector<std::vector<std::unordered_set<int>>> allOptionsCopy = allOptions;
    std::vector<std::vector<int>> optionsCount = get_options_count();
    for (int r = 0; r < 9; r++)
    {
        for (int c = 0; c < 9; c++)
        {
            if (optionsCount[r][c] == 2)
            {
                int r1, r2, r3, r4, c1, c2, c3, c4, n1, n2;
                r2 = r3 = r4 = c2 = c3 = c4 = 10;
                n1 = n2 = 0;
                r1 = r;
                c1 = c;
                std::vector<std::unordered_set<int>> optionsRow = get_options_row(r);
                std::vector<std::unordered_set<int>> optionsCol = get_options_col(c);
                bool diagonal = false;
                if (count(optionsRow.begin(), optionsRow.end(), allOptions[r][c]) == 2) //Let's check in the rows first. This should find any version except 2c
                {
                    std::vector<int> options;
                    options.insert(options.end(), allOptions[r][c].begin(), allOptions[r][c].end());
                    n1 = options[0];
                    n2 = options[1];
                    r2 = r1;
                    c2 = find(optionsRow.begin(), optionsRow.end(), allOptions[r][c]) - optionsRow.begin();
                    if (c2 == c1)
                        c2 = find(optionsRow.begin() + c1 + 1, optionsRow.end(), allOptions[r][c]) - optionsRow.begin();

                    for (int i = 0; i < 9 and c4 == 10; i++)
                    {
                        if (i != r1)
                        {
                            if (allOptions[i][c1].count(n1) == 1 and allOptions[i][c1].count(n2) == 1 and allOptions[i][c2].count(n1) == 1 and allOptions[i][c2].count(n2) == 1)
                            {
                                if (block_number(r1, c1) == block_number(r2, c2) or block_number(r1, c1) == block_number(i, c1))
                                {
                                    r3 = r4 = i;
                                    c3 = c1;
                                    c4 = c2;
                                }
                            }
                        }
                    }
                }
                if (r4 == 10) //we haven't found a square yet. Let's check collums
                {
                    if (count(optionsCol.begin(), optionsCol.end(), allOptions[r][c]) == 2) //Let's check in the rows first. This should find any version except 2c
                    {
                        std::vector<int> options;
                        options.insert(options.end(), allOptions[r][c].begin(), allOptions[r][c].end());
                        n1 = options[0];
                        n2 = options[1];
                        c2 = c1;
                        r2 = find(optionsCol.begin(), optionsCol.end(), allOptions[r][c]) - optionsCol.begin();
                        if (r2 == r1)
                            r2 = find(optionsCol.begin() + r1 + 1, optionsCol.end(), allOptions[r][c]) - optionsCol.begin();

                        for (int i = 0; i < 9 and c4 == 10; i++)
                        {
                            if (i != c1)
                            {
                                if (allOptions[r1][i].count(n1) == 1 and allOptions[r1][i].count(n2) == 1 and allOptions[r2][i].count(n1) == 1 and allOptions[r2][i].count(n2) == 1)
                                {
                                    if (block_number(r1, c1) == block_number(r2, c2) or block_number(r1, c1) == block_number(r1, i))
                                    {
                                        c3 = c4 = i;
                                        r3 = r1;
                                        r4 = r2;
                                    }
                                }
                            }
                        }
                    }
                }
                if (r4 == 10) //we haven't found a square yet. We have to check diagonals
                {
                    int r0 = r1 / 3 * 3;
                    int c0 = c1 / 3 * 3;
                    int tr1, tr2, tc1, tc2;
                    if (r1 == r0)
                    {
                        tr1 = r0 + 1;
                        tr2 = r0 + 2;
                    }
                    else if (r1 == r0 + 1)
                    {
                        tr1 = r0;
                        tr2 = r0 + 2;
                    }
                    else if (r1 == r0 + 2)
                    {
                        tr1 = r0;
                        tr2 = r0 + 1;
                    }
                    if (c1 == c0)
                    {
                        tc1 = c0 + 1;
                        tc2 = c0 + 2;
                    }
                    else if (c1 == c0 + 1)
                    {
                        tc1 = c0;
                        tc2 = c0 + 2;
                    }
                    else if (c1 == c0 + 2)
                    {
                        tc1 = c0;
                        tc2 = c0 + 1;
                    }
                    std::vector<std::unordered_set<int>> tOptionsRow1 = get_options_row(tr1);
                    std::vector<std::unordered_set<int>> tOptionsRow2 = get_options_row(tr2);
                    std::vector<std::unordered_set<int>> tOptionsCol1 = get_options_col(tc1);
                    std::vector<std::unordered_set<int>> tOptionsCol2 = get_options_col(tc2);
                    if (find(tOptionsRow1.begin(), tOptionsRow1.end(), allOptions[r1][c1]) != tOptionsRow1.end())
                    {
                        int tc = find(tOptionsRow1.begin(), tOptionsRow1.end(), allOptions[r1][c1]) - tOptionsRow1.begin();
                        if (tc == c1)
                            tc = find(tOptionsRow1.begin() + tc + 1, tOptionsRow1.end(), allOptions[r1][c1]) - tOptionsRow1.begin();
                        if (tc < 9 and allOptions[r1][tc].count(n1) == 1 and allOptions[r1][tc].count(n2) == 1 and allOptions[tr1][c1].count(n1) == 1 and allOptions[tr1][c1].count(n2) == 1)
                        {
                            r4 = tr1;
                            c4 = tc;
                            r2 = r1;
                            c2 = c4;
                            r3 = r4;
                            c3 = c1;
                            diagonal = true;
                        }
                    }
                    if (r4 == 10 and find(tOptionsRow2.begin(), tOptionsRow2.end(), allOptions[r1][c1]) != tOptionsRow2.end())
                    {
                        int tc = find(tOptionsRow2.begin(), tOptionsRow2.end(), allOptions[r1][c1]) - tOptionsRow2.begin();
                        if (tc == c1)
                            tc = find(tOptionsRow2.begin() + tc + 1, tOptionsRow2.end(), allOptions[r1][c1]) - tOptionsRow2.begin();
                        if (tc < 9 and allOptions[r1][tc].count(n1) == 1 and allOptions[r1][tc].count(n2) == 1 and allOptions[tr2][c1].count(n1) == 1 and allOptions[tr2][c1].count(n2) == 1)
                        {
                            r4 = tr2;
                            c4 = tc;
                            r2 = r1;
                            c2 = c4;
                            r3 = r4;
                            c3 = c1;
                            diagonal = true;
                        }
                    }
                    if (r4 == 10 and find(tOptionsCol1.begin(), tOptionsCol1.end(), allOptions[r1][c1]) != tOptionsCol1.end())
                    {
                        int tr = find(tOptionsCol1.begin(), tOptionsCol1.end(), allOptions[r1][c1]) - tOptionsCol1.begin();
                        if (tr == r1)
                            tr = find(tOptionsCol1.begin() + tr + 1, tOptionsCol1.end(), allOptions[r1][c1]) - tOptionsCol1.begin();
                        if (tr < 9 and allOptions[tr][c1].count(n1) == 1 and allOptions[tr][c1].count(n2) == 1 and allOptions[r1][tc1].count(n1) == 1 and allOptions[r1][tc1].count(n2) == 1)
                        {
                            r4 = tr;
                            c4 = tc1;
                            r2 = r1;
                            c2 = c4;
                            r3 = r4;
                            c3 = c1;
                            diagonal = true;
                        }
                    }
                    if (r4 == 10 and find(tOptionsCol2.begin(), tOptionsCol2.end(), allOptions[r1][c1]) != tOptionsCol2.end())
                    {
                        int tr = find(tOptionsCol2.begin(), tOptionsCol2.end(), allOptions[r1][c1]) - tOptionsCol2.begin();
                        if (tr == r1)
                            tr = find(tOptionsCol2.begin() + tr + 1, tOptionsCol2.end(), allOptions[r1][c1]) - tOptionsCol2.begin();
                        if (tr < 9 and allOptions[tr][c1].count(n1) == 1 and allOptions[tr][c1].count(n2) == 1 and allOptions[r1][tc2].count(n1) == 1 and allOptions[r1][tc2].count(n2) == 1)
                        {
                            r4 = tr;
                            c4 = tc2;
                            r2 = r1;
                            c2 = c4;
                            r3 = r4;
                            c3 = c1;
                            diagonal = true;
                        }
                    }
                }
                if (r4 != 10) //We found one!
                {
                    if ((allOptions[r1][c1] == allOptions[r2][c2]) and (allOptions[r1][c1] == allOptions[r3][c3] xor allOptions[r1][c1] == allOptions[r4][c4]))
                    { //We found type 1
                        if (allOptions[r3][c3] != allOptions[r1][c1])
                        {
                            allOptionsCopy[r3][c3].erase(n1);
                            allOptionsCopy[r3][c3].erase(n2);
                            found = true;
                        }
                        else
                        {
                            allOptionsCopy[r4][c4].erase(n1);
                            allOptionsCopy[r4][c4].erase(n2);
                            found = true;
                        }
                    }
                    else if (allOptions[r1][c1] == allOptions[r2][c2] and allOptions[r3][c3] == allOptions[r4][c4] and allOptions[r1][c1] != allOptions[r3][c3] and allOptions[r3][c3].size() == 3)
                    { //we found type 2 a or b
                        std::unordered_set<int> remove = unordered_set_difference(allOptions[r3][c3], allOptions[r1][c1]);
                        int rm = *remove.begin();
                        std::vector<std::pair<int, int>> seen = list_seen_by(r3, c3, r4, c4);
                        for (std::pair<int, int> rc : seen)
                        {
                            if (allOptionsCopy[rc.first][rc.second].erase(rm))
                                found = true;
                        }
                    }
                    else if (diagonal and allOptions[r2][c2] == allOptions[r3][c3] and allOptions[r3][c3].size() == 3)
                    { //type 2 c
                        std::unordered_set<int> remove = unordered_set_difference(allOptions[r3][c3], allOptions[r1][c1]);
                        int rm = *remove.begin();
                        std::vector<std::pair<int, int>> seen = list_seen_by(r2, c2, r3, c3);
                        for (std::pair<int, int> rc : seen)
                        {
                            if (allOptionsCopy[rc.first][rc.second].erase(rm))
                                found = true;
                        }
                    }
                    else if (allOptions[r4][c4] != allOptions[r3][c3] and allOptions[r4][c4].size() == 3 and allOptions[r3][c3].size() == 3 and block_number(r4, c4) != block_number(r3, c3))
                    { //type 3a
                        std::unordered_set<int> rm1 = unordered_set_difference(allOptions[r3][c3], allOptions[r1][c1]);
                        std::unordered_set<int> rm2 = unordered_set_difference(allOptions[r4][c4], allOptions[r1][c1]);
                        int n3 = *rm1.begin();
                        int n4 = *rm2.begin();
                        std::unordered_set<int> needle = {n3, n4};
                        int nc1 = 10;
                        int nr1 = 10;
                        if (r3 == r4)
                        {
                            nc1 == find(allOptions[r3].begin(), allOptions[r3].end(), needle) - allOptions[r3].begin();
                            if (nc1 < 9)
                                nr1 = r3;
                        }
                        else if (c3 == c4)
                        {
                            optionsCol = get_options_col(c3);
                            nr1 == find(optionsCol.begin(), optionsCol.end(), needle) - optionsCol.begin();
                            if (nr1 < 9)
                                nc1 = r3;
                        }
                        if (nr1 < 9 and nc1 < 9)
                        {
                            std::vector<std::pair<int, int>> seen = list_seen_by(list_seen_by(r2, c2, r3, c3), nr1, nc1);
                            for (std::pair<int, int> rc : seen)
                            {
                                if (allOptionsCopy[rc.first][rc.second].erase(n3))
                                    found = true;
                                if (allOptionsCopy[rc.first][rc.second].erase(n4))
                                    found = true;
                            }
                        }
                    }
                    else if (allOptions[r4][c4] != allOptions[r3][c3] and allOptions[r4][c4].size() == 3 and allOptions[r3][c3].size() == 3 and block_number(r4, c4) == block_number(r3, c3))
                    { //type 3a
                        std::unordered_set<int> rm1 = unordered_set_difference(allOptions[r3][c3], allOptions[r1][c1]);
                        std::unordered_set<int> rm2 = unordered_set_difference(allOptions[r4][c4], allOptions[r1][c1]);
                        int n3 = *rm1.begin();
                        int n4 = *rm2.begin();
                        std::unordered_set<int> needle = {n3, n4};
                        int nc1 = 10;
                        int nr1 = 10;
                        int nr2 = 10;
                        int nc2 = 10;
                        if (r3 == r4)
                        {
                            nc1 == find(allOptions[r3].begin(), allOptions[r3].end(), needle) - allOptions[r3].begin();
                            if (nc1 < 9)
                                nr1 = r3;
                        }
                        else if (c3 == c4)
                        {
                            optionsCol = get_options_col(c3);
                            nr1 == find(optionsCol.begin(), optionsCol.end(), needle) - optionsCol.begin();
                            if (nr1 < 9)
                                nc1 = r3;
                        }
                        std::vector<std::unordered_set<int>> optionsBlock = get_options_block(r3, c3);
                        int b2 = find(optionsBlock.begin(), optionsBlock.end(), needle) - optionsBlock.begin();
                        if (b2 < 9)
                        {
                            int r0 = r3 / 3 * 3;
                            int c0 = c3 / 3 * 3;
                            int nr2 = r0 + b2 / 3;
                            int nc2 = c0 + b2 % 3;
                        }
                        if (nr1 < 9 and nc1 < 9)
                        {
                            std::vector<std::pair<int, int>> seen = list_seen_by(list_seen_by(r2, c2, r3, c3), nr1, nc1);
                            for (std::pair<int, int> rc : seen)
                            {
                                if (allOptionsCopy[rc.first][rc.second].erase(n3))
                                    found = true;
                                if (allOptionsCopy[rc.first][rc.second].erase(n4))
                                    found = true;
                            }
                        }
                        if (nr2 < 9 and nc2 < 9)
                        {
                            std::vector<std::pair<int, int>> seen = list_seen_by(list_seen_by(r2, c2, r3, c3), nr2, nc2);
                            for (std::pair<int, int> rc : seen)
                            {
                                if (allOptionsCopy[rc.first][rc.second].erase(n3))
                                    found = true;
                                if (allOptionsCopy[rc.first][rc.second].erase(n4))
                                    found = true;
                            }
                        }
                    }
                    else if ((allOptions[r4][c4].size() == 4 and allOptions[r3][c3].size() == 3) or (allOptions[r4][c4].size() == 4 and allOptions[r3][c3].size() == 4) or allOptions[r4][c4].size() == 3 and allOptions[r3][c3].size() == 4) //Todo: Type 3/3b with Triple Pseudo-Cells
                    {
                        std::unordered_set<int> merged = allOptions[r4][c4];
                        merged.insert(allOptions[r3][c3].begin(), allOptions[r3][c3].end());
                        if (merged.size() == 4)
                        {
                            std::unordered_set<int> needle = unordered_set_intersection(merged, allOptions[r1][c1]);
                            int n3 = *needle.begin();
                            needle.erase(n3);
                            int n4 = *needle.begin();
                            needle = {n3, n4};
                            int n5 = 0;
                            int r5, r6, c5, c6;
                            r5 = r6 = c5 = c6 = 10;
                            if (r3 == r4) //they're on the same row
                            {
                                std::vector<bool> locationsRowN3 = get_possible_locations_row(n3, r3);
                                std::vector<bool> locationsRowN4 = get_possible_locations_row(n4, r3);
                                for (int pc5 = 0; pc5 < 9 and n5 == 0; pc5++)
                                {
                                    if (pc5 != c4 and pc5 != c3 and (locationsRowN3[pc5] == true or locationsRowN4[pc5] == true))
                                    {
                                        if (unordered_set_difference(allOptions[r3][pc5], needle).size() == 1)
                                        {
                                            std::unordered_set<int> pn5s = unordered_set_difference(allOptions[r3][pc5], needle);
                                            int pn5 = *pn5s.begin();
                                            std::vector<bool> locationsRowN5 = get_possible_locations_row(pn5, r4);
                                            if (count(locationsRowN5.begin(), locationsRowN5.end(), true) == 2)
                                            {
                                                int pc6 = find(locationsRowN5.begin() + pc5 + 1, locationsRowN5.end(), true) - locationsRowN5.begin();
                                                if (pc6 < 9 and unordered_set_difference(allOptions[r3][pc6], needle).size() == 1)
                                                {
                                                    r5 = r6 = r4;
                                                    c5 = pc5;
                                                    c6 = pc6;
                                                    n5 = pn5;
                                                }
                                            }
                                        }
                                    }
                                }
                                if (c5 != 10) //I think we found something
                                {
                                    for (int c7 = 0; c7++; c7 < 9)
                                    {
                                        if (c7 != c3 and c7 != c4 and c7 != c5 and c7 != c6)
                                        {
                                            if (allOptionsCopy[r4][c7].erase(n3))
                                                found = true;
                                            if (allOptionsCopy[r4][c7].erase(n4))
                                                found = true;
                                        }
                                    }
                                }
                            }
                            else //they're on the same col
                            {
                                std::vector<bool> locationsColN3 = get_possible_locations_col(n3, c3);
                                std::vector<bool> locationsColN4 = get_possible_locations_col(n4, c3);
                                for (int pr5 = 0; pr5 < 9 and n5 == 0; pr5++)
                                {
                                    if (pr5 != r4 and pr5 != r3 and (locationsColN3[pr5] == true or locationsColN4[pr5] == true))
                                    {
                                        if (unordered_set_difference(allOptions[pr5][c3], needle).size() == 1)
                                        {
                                            std::unordered_set<int> pn5s = unordered_set_difference(allOptions[pr5][c3], needle);
                                            int pn5 = *pn5s.begin();
                                            std::vector<bool> locationsColN5 = get_possible_locations_col(pn5, c4);
                                            if (count(locationsColN5.begin(), locationsColN5.end(), true) == 2)
                                            {
                                                int pr6 = find(locationsColN5.begin() + pr5 + 1, locationsColN5.end(), true) - locationsColN5.begin();
                                                if (pr6 < 9 and unordered_set_difference(allOptions[pr6][c4], needle).size() == 1)
                                                {
                                                    c5 = c6 = c4;
                                                    r5 = pr5;
                                                    r6 = pr6;
                                                    n5 = pn5;
                                                }
                                            }
                                        }
                                    }
                                }
                                if (r5 != 10) //I think we found something
                                {
                                    for (int r7 = 0; r7++; r7 < 9)
                                    {
                                        if (r7 != r3 and r7 != r4 and r7 != r5 and r7 != r6)
                                        {
                                            if (allOptionsCopy[r7][c4].erase(n3))
                                                found = true;
                                            if (allOptionsCopy[r7][c4].erase(n4))
                                                found = true;
                                        }
                                    }
                                }
                            }
                        }
                    }
                    //finally the variations of case 4.
                    if (block_number(r3, c3) == block_number(r4, c4) and allOptions[r3][c3].size() > 2 and allOptions[r4][c3].size() > 2)
                    {
                        std::vector<bool> locationsBlockn1 = get_possible_locations_block(n1, r3, c3);
                        std::vector<bool> locationsBlockn2 = get_possible_locations_block(n2, r3, c3);
                        if (count(locationsBlockn1.begin(), locationsBlockn1.end(), true) == 2)
                        { //n1 has to be in the two squares, therefore n2 can be removed
                            if (allOptionsCopy[r3][c3].erase(n2))
                                found = true;
                            if (allOptionsCopy[r4][c4].erase(n2))
                                found = true;
                        }
                        else if (count(locationsBlockn2.begin(), locationsBlockn2.end(), true) == 2)
                        { //n2 has to be in the two squares, therefore n1 can be removed
                            if (allOptionsCopy[r3][c3].erase(n1))
                                found = true;
                            if (allOptionsCopy[r4][c4].erase(n1))
                                found = true;
                        }
                    }
                    if (r3 == r4 and allOptions[r3][c3].size() > 2 and allOptions[r4][c3].size() > 2)
                    {
                        std::vector<bool> locationsRown1 = get_possible_locations_row(n1, r3);
                        std::vector<bool> locationsRown2 = get_possible_locations_row(n2, r3);
                        if (count(locationsRown1.begin(), locationsRown1.end(), true) == 2)
                        { //n1 has to be in the two squares, therefore n2 can be removed
                            if (allOptionsCopy[r3][c3].erase(n2))
                                found = true;
                            if (allOptionsCopy[r4][c4].erase(n2))
                                found = true;
                        }
                        else if (count(locationsRown2.begin(), locationsRown2.end(), true) == 2)
                        { //n2 has to be in the two squares, therefore n1 can be removed
                            if (allOptionsCopy[r3][c3].erase(n1))
                                found = true;
                            if (allOptionsCopy[r4][c4].erase(n1))
                                found = true;
                        }
                    }
                    else if (c3 == c4 and allOptions[r3][c3].size() > 2 and allOptions[r4][c3].size() > 2)
                    {
                        std::vector<bool> locationsColn1 = get_possible_locations_col(n1, c3);
                        std::vector<bool> locationsColn2 = get_possible_locations_col(n2, c3);
                        if (count(locationsColn1.begin(), locationsColn1.end(), true) == 2)
                        { //n1 has to be in the two squares, therefore n2 can be removed
                            if (allOptionsCopy[r3][c3].erase(n2))
                                found = true;
                            if (allOptionsCopy[r4][c4].erase(n2))
                                found = true;
                        }
                        else if (count(locationsColn2.begin(), locationsColn2.end(), true) == 2)
                        { //n2 has to be in the two squares, therefore n1 can be removed
                            if (allOptionsCopy[r3][c3].erase(n1))
                                found = true;
                            if (allOptionsCopy[r4][c4].erase(n1))
                                found = true;
                        }
                    }
                }
            }
        }
    }

    if (found)
    {
        found = (allOptions != allOptionsCopy);
        allOptions = allOptionsCopy;
    }
    return found;
}

bool Sudoku::hidden_unique_rectangles()
{
    bool found = false;
    std::vector<std::vector<std::unordered_set<int>>> allOptionsCopy = allOptions;

    for (int r = 0; r < 9 and found == false; r++)
    {
        for (int c = 0; c < 9 and found == false; c++)
        {
            if (allOptions[r][c].size() == 2)
            {
                std::vector<int> options;
                options.insert(options.end(), allOptions[r][c].begin(), allOptions[r][c].end());
                int n1 = options[0];
                int n2 = options[1];
                int r1 = r;
                int c1 = c;
                int r2, r3, r4, c2, c3, c4;
                r2 = r3 = r4 = c2 = c3 = c4 = 10;
                for (int i = 0; i < 9 and r4 == 10; i++)
                {

                    if (i != c and allOptions[r][i].count(n1) == 1 and allOptions[r][i].count(n2) == 1) // we found another square that has both n1 and n2
                    {
                        r2 = r;
                        c2 = i;
                        for (int j = 0; j < 9 and r4 == 10; j++)
                        {
                            if (j != r and (block_number(r1, c1) == block_number(r2, c2) or block_number(r1, c1) == block_number(j, c1)) and allOptions[j][c1].count(n1) == 1 and allOptions[j][c1].count(n2) == 1 and allOptions[j][c2].count(n1) == 1 and allOptions[j][c2].count(n2) == 1)
                            { //we found a square
                                r3 = j;
                                r4 = j;
                                c3 = c1;
                                c4 = c2;
                            }
                        }
                    }
                }
                if (r4 != 10 and (block_number(r1, c1) == block_number(r2, c2) or block_number(r1, c1) == block_number(r3, c3)))
                {
                    if (allOptions[r2][c2] != allOptions[r1][c1] and allOptions[r3][c3] != allOptions[r1][c1] and allOptions[r4][c4] != allOptions[r1][c1])
                    { //r1,c1 is the only pair. Type 1.
                        std::vector<bool> locationsRow = get_possible_locations_row(n1, r4);
                        std::vector<bool> locationsCol = get_possible_locations_col(n1, c4);
                        if (count(locationsRow.begin(), locationsRow.end(), true) == 2 and count(locationsCol.begin(), locationsCol.end(), true) == 2)
                        { //there is a strong link for N1. N2 can be removed.
                            allOptionsCopy[r4][c4].erase(n2);
                            found = true;
                        }
                        else
                        {
                            locationsRow = get_possible_locations_row(n2, r4);
                            locationsCol = get_possible_locations_col(n2, c4);
                            if (count(locationsRow.begin(), locationsRow.end(), true) == 2 and count(locationsCol.begin(), locationsCol.end(), true) == 2)
                            { //there is a strong link for N2. N1 can be removed.
                                allOptionsCopy[r4][c4].erase(n1);
                                found = true;
                            }
                        }
                    }
                    else if (allOptions[r1][c1] == allOptions[r2][c2])
                    { //if the "floor pair" is in the same column.
                        std::vector<bool> locationsCol = get_possible_locations_col(n1, c1);
                        if (count(locationsCol.begin(), locationsCol.end(), true) == 2)
                        { //hard link between r1,c1 and r3,c3 on n1. remove n2 from r4,c4
                            allOptionsCopy[r4][c4].erase(n2);
                            found = true;
                        }
                        else
                        {
                            locationsCol = get_possible_locations_col(n2, c1);
                            if (count(locationsCol.begin(), locationsCol.end(), true) == 2)
                            { //hard link between r1,c1 and r3,c3 on n2. remove n2 from r4,c4
                                allOptionsCopy[r4][c4].erase(n1);
                                found = true;
                            }
                            else
                            {
                                locationsCol = get_possible_locations_col(n1, c2);
                                if (count(locationsCol.begin(), locationsCol.end(), true) == 2)
                                { //hard link between r2,c2 and r4,c4 on n1. remove n2 from r3,c3
                                    allOptionsCopy[r3][c3].erase(n2);
                                    found = true;
                                }
                                else
                                {
                                    locationsCol = get_possible_locations_col(n2, c2);
                                    if (count(locationsCol.begin(), locationsCol.end(), true) == 2)
                                    { //hard link between r2,c2 and r4,c4 on n2. remove n2 from r4,c4
                                        allOptionsCopy[r3][c3].erase(n1);
                                        found = true;
                                    }
                                }
                            }
                        }
                    }
                    else if (allOptions[r1][c1] == allOptions[r3][c3])
                    { //if the "floor pair" is in the same row.
                        std::vector<bool> locationsRow = get_possible_locations_row(n1, r1);
                        if (count(locationsRow.begin(), locationsRow.end(), true) == 2)
                        { //hard link between r1,c1 and r2,c2 on n1. remove n2 from r4,c4
                            allOptionsCopy[r4][c4].erase(n2);
                            found = true;
                        }
                        else
                        {
                            locationsRow = get_possible_locations_row(n2, r1);
                            if (count(locationsRow.begin(), locationsRow.end(), true) == 2)
                            { //hard link between r1,c1 and r2,c2 on n2. remove n1 from r4,c4
                                allOptionsCopy[r4][c4].erase(n1);
                                found = true;
                            }
                            else
                            {
                                locationsRow = get_possible_locations_row(n1, r2);
                                if (count(locationsRow.begin(), locationsRow.end(), true) == 2)
                                { //hard link between r3,c3 and r4,c4 on n1. remove n2 from r2,c2
                                    allOptionsCopy[r2][c2].erase(n2);
                                    found = true;
                                }
                                else
                                {
                                    locationsRow = get_possible_locations_row(n2, r2);
                                    if (count(locationsRow.begin(), locationsRow.end(), true) == 2)
                                    { //hard link between r3,c3 and r4,c4 on n2. remove n2 from r2,c2
                                        allOptionsCopy[r2][c2].erase(n1);
                                        found = true;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    if (found)
    {
        found = (allOptions != allOptionsCopy);
        allOptions = allOptionsCopy;
    }
    return found;
}

bool Sudoku::alternating_inference_chains()
{ //todo: implement groups
    bool found = false;

    std::vector<std::vector<std::unordered_set<int>>> allOptionsCopy = allOptions;
    std::vector<std::vector<std::vector<int>>> chains;
    int chainLength;
    bool strongStart;
    bool strongEnd;
    std::function<bool(int, int, int, int, int, int, int)> find_chain; //r, c == coordinates currently tested, n is the number used to get there. gr, gc, gn is the starting place and number. n and gn can be 0 if not yet decided colour is 0 or 1, or 2 if not yet decided.
    find_chain = [&](int r, int c, int n, int gr, int gc, int gn, int colour) {
        std::vector<int> options;
        options.insert(options.end(), allOptions[r][c].begin(), allOptions[r][c].end());
        int optionSize = options.size();
        if (colour == 2 and optionSize == 2)
        { //if this is the first square in the chain and there are only two options
            chains[0][r][c] = options[0];
            chainLength++;
            strongStart = true;
            strongEnd = false;
            if (find_chain(r, c, options[1], gr, gc, options[0], 1))
                return true;
            else
            {
                chains[0][r][c] = options[1];
                if (find_chain(r, c, options[0], gr, gc, options[1], 1))
                    return true;
                else
                {
                    chains[0][r][c] = 0;
                    chainLength--;
                    strongStart = false;
                    strongEnd = true;
                }
            }
        }

        if (colour == 2)
        {
            for (int curn : options)
            {
                std::vector<bool> locationsRow = get_possible_locations_row(curn, r);
                if (count(locationsRow.begin(), locationsRow.end(), true) == 2)
                {
                    int nextr = r;
                    int nextc = find(locationsRow.begin(), locationsRow.end(), true) - locationsRow.begin();
                    if (nextc == c)
                        nextc = find(locationsRow.begin() + nextc + 1, locationsRow.end(), true) - locationsRow.begin();
                    if (nextc < 9 and chains[0][nextr][nextc] == 0 and chains[1][nextr][nextc] == 0)
                    {
                        chains[0][r][c] = curn;
                        strongStart = true;
                        strongEnd = false;
                        chainLength++;
                        if (find_chain(nextr, nextc, curn, gr, gc, curn, 1))
                            return true;
                        else
                        {
                            chains[0][r][c] = 0;
                            strongStart = false;
                            strongEnd = true;
                            chainLength--;
                        }
                    }
                }
                std::vector<bool> locationsCol = get_possible_locations_col(curn, c);
                if (count(locationsCol.begin(), locationsCol.end(), true) == 2)
                {
                    int nextc = c;
                    int nextr = find(locationsCol.begin(), locationsCol.end(), true) - locationsCol.begin();
                    if (nextr == r)
                        nextr = find(locationsCol.begin() + r + 1, locationsCol.end(), true) - locationsCol.begin();
                    if (chains[0][nextr][nextc] == 0 and chains[1][nextr][nextc] == 0)
                    {
                        chains[0][r][c] = curn;
                        chainLength++;
                        strongStart = true;
                        if (find_chain(nextr, nextc, curn, gr, gc, curn, 1))
                            return true;
                        else
                        {
                            chains[0][r][c] = 0;
                            strongStart = false;
                            strongEnd = true;
                            chainLength--;
                        }
                    }
                }
                std::vector<bool> locationsBlock = get_possible_locations_block(curn, r, c);
                if (count(locationsBlock.begin(), locationsBlock.end(), true) == 2)
                {
                    int b = r % 3 * 3 + c % 3;
                    int nextb = find(locationsBlock.begin(), locationsBlock.end(), true) - locationsBlock.begin();
                    if (nextb == b)
                        nextb = find(locationsBlock.begin() + b + 1, locationsBlock.end(), true) - locationsBlock.begin();
                    int r0 = r / 3 * 3;
                    int c0 = c / 3 * 3;
                    int nextr = r0 + nextb / 3;
                    int nextc = c0 + nextb % 3;
                    if (chains[0][nextr][nextc] == 0 and chains[1][nextr][nextc] == 0)
                    {
                        chains[0][r][c] = curn;
                        strongStart = true;
                        chainLength++;
                        if (find_chain(nextr, nextc, curn, gr, gc, curn, 1))
                            return true;
                        else
                        {
                            chains[0][r][c] = 0;
                            strongStart = false;
                            strongEnd = true;
                            chainLength--;
                        }
                    }
                }
            }
            for (int curn : options)
            {
                std::vector<bool> locationsRow = get_possible_locations_row(curn, r);
                if (count(locationsRow.begin(), locationsRow.end(), true) > 2)
                {
                    for (int nextc = 0; nextc < 9; nextc++)
                    {
                        if (nextc != c and locationsRow[nextc] == true)
                        {
                            chains[1][r][c] = curn;
                            chainLength++;
                            if (find_chain(r, nextc, curn, gr, gc, curn, 0))
                                return true;
                            else
                            {
                                chains[1][r][c] = 0;
                                chainLength--;
                            }
                        }
                    }
                }
                std::vector<bool> locationsCol = get_possible_locations_col(curn, c);
                if (count(locationsCol.begin(), locationsCol.end(), true) > 2)
                {
                    for (int nextr = 0; nextr < 9; nextr++)
                    {
                        if (nextr != r and locationsCol[nextr] == true)
                        {
                            chains[1][r][c] = curn;
                            chainLength++;
                            if (find_chain(nextr, c, curn, gr, gc, curn, 0))
                                return true;
                            else
                            {
                                chains[1][r][c] = 0;
                                chainLength--;
                            }
                        }
                    }
                }
                std::vector<bool> locationsBlock = get_possible_locations_block(curn, r, c);
                if (count(locationsBlock.begin(), locationsBlock.end(), true) > 2)
                {
                    int b = r % 3 * 3 + c % 3;
                    int r0 = r / 3 * 3;
                    int c0 = c / 3 * 3;
                    for (int nextb = 0; nextb < 9; nextb++)
                    {
                        if (nextb != b and locationsBlock[nextb] == true)
                        {
                            int nextr = r0 + nextb / 3;
                            int nextc = c0 + nextb % 3;
                            chains[1][r][c] = curn;
                            chainLength++;
                            if ((r != nextr and c != nextc) and find_chain(nextr, nextc, curn, gr, gc, curn, 0))
                                return true;
                            else
                            {
                                chains[1][r][c] = 0;
                                chainLength--;
                            }
                        }
                    }
                }
            }
        }

        if (colour == 0 and options.size() == 2 and chains[1][r][c] == 0)
        { //if this is a strong link and there are only two options
            chains[0][r][c] = n;
            chainLength++;
            int nextN = (options[0] == n) ? options[1] : options[0];
            if (find_chain(r, c, nextN, gr, gc, gn, 1))
                return true;
            else
            {
                chains[0][r][c] = 0;
                chainLength--;
            }
        }

        /*if (colour == 0 and options.size() == 3 and chains[1][r][c] == 0)
        { //if this is a strong link and there are 3 options: look for almost locked set.

            std::unordered_set<int> needle = allOptions[r][c];
            needle.erase(n);
            options.clear();
            options.insert(options.end(), needle.begin(), needle.end());
            int c2 = find(allOptions[r].begin(), allOptions[r].end(), needle) - allOptions[r].begin();
            if (c2 <= 8 and chains[0][r][c2] == 0 and chains[1][r][c2] == 0) //we found an als in the row
            {
                std::vector<bool> locationsRow = get_possible_locations_row(options[0], r);
                if (count(locationsRow.begin(), locationsRow.end(), true) > 2)
                {
                    for (int i = 0; i < 9; i++)
                    {
                        if (locationsRow[i] == true and i != c and i != c2 and chains[0][r][i] == 0 and chains[1][r][i] == 0)
                        {
                            chains[0][r][c] = n;
                            chains[1][r][c] = 10 + options[0];
                            chains[1][r][c2] = 10 + options[0];
                            chainLength += 2;
                            if (find_chain(r, i, options[0], gr, gc, gn, 0))
                                return true;
                            else
                            {
                                chains[0][r][c] = 0;
                                chains[1][r][c] = 0;
                                chains[1][r][c2] = 0;
                                chainLength -= 2;
                            }
                        }
                    }
                }
                locationsRow = get_possible_locations_row(options[1], r);
                if (count(locationsRow.begin(), locationsRow.end(), true) > 2)
                {
                    for (int i = 0; i < 9; i++)
                    {
                        if (locationsRow[i] == true and i != c and i != c2 and chains[0][r][i] == 0 and chains[1][r][i] == 0)
                        {
                            chains[0][r][c] = n;
                            chains[1][r][c] = 10 + options[1];
                            chains[1][r][c2] = 10 + options[1];
                            chainLength += 2;
                            if (find_chain(r, i, options[1], gr, gc, gn, 0))
                                return true;
                            else
                            {
                                chains[0][r][c] = 0;
                                chains[1][r][c] = 0;
                                chains[1][r][c2] = 0;
                                chainLength -= 2;
                            }
                        }
                    }
                }
            }
            std::vector<std::unordered_set<int>> optionsCol = get_options_col(c);
            int r2 = find(optionsCol.begin(), optionsCol.end(), needle) - optionsCol.begin();
            if (r2 <= 8 and chains[0][r2][c] == 0 and chains[1][r2][c] == 0)
            {
                std::vector<bool> locationsCol = get_possible_locations_col(options[0], c);
                if (count(locationsCol.begin(), locationsCol.end(), true) > 2)
                {
                    for (int i = 0; i < 9; i++)
                    {
                        if (locationsCol[i] == true and i != r and i != r2 and chains[0][i][c] == 0 and chains[1][i][c] == 0)
                        {
                            chains[0][r][c] = n;
                            chains[1][r][c] = 10 + options[0];
                            chains[1][r2][c] = 10 + options[0];
                            chainLength += 2;
                            if (find_chain(i, c, options[0], gr, gc, gn, 0))
                                return true;
                            else
                            {
                                chains[0][r][c] = 0;
                                chains[1][r][c] = 0;
                                chains[1][r2][c] = 0;
                                chainLength -= 2;
                            }
                        }
                    }
                }
                locationsCol = get_possible_locations_col(options[1], c);
                if (count(locationsCol.begin(), locationsCol.end(), true) > 2)
                {
                    for (int i = 0; i < 9; i++)
                    {
                        if (locationsCol[i] == true and i != r and i != r2 and chains[0][i][c] == 0 and chains[1][i][c] == 0)
                        {
                            chains[0][r][c] = n;
                            chains[1][r][c] = 10 + options[1];
                            chains[1][r2][c] = 10 + options[1];
                            chainLength += 2;
                            if (find_chain(i, c, options[1], gr, gc, gn, 0))
                                return true;
                            else
                            {
                                chains[0][r][c] = 0;
                                chains[1][r][c] = 0;
                                chains[1][r2][c] = 0;
                                chainLength -= 2;
                            }
                        }
                    }
                }
            }
        }
*/
        if (colour == 0)
        { //strong link
            std::vector<bool> locationsRow = get_possible_locations_row(n, r);
            if (count(locationsRow.begin(), locationsRow.end(), true) == 2)
            {
                int nextr = r;
                int nextc = find(locationsRow.begin(), locationsRow.end(), true) - locationsRow.begin();
                if (nextc == c)
                    nextc = find(locationsRow.begin() + c + 1, locationsRow.end(), true) - locationsRow.begin();
                if (chains[0][nextr][nextc] == 0 and chains[1][nextr][nextc] == 0)
                {
                    chains[0][r][c] = n;
                    chainLength++;
                    if (find_chain(nextr, nextc, n, gr, gc, gn, 1))
                        return true;
                    else
                    {
                        chains[0][r][c] = 0;
                        chainLength--;
                    }
                }
            }
            std::vector<bool> locationsCol = get_possible_locations_col(n, c);
            if (count(locationsCol.begin(), locationsCol.end(), true) == 2)
            {
                int nextc = c;
                int nextr = find(locationsCol.begin(), locationsCol.end(), true) - locationsCol.begin();
                if (nextr == r)
                    nextr = find(locationsCol.begin() + r + 1, locationsCol.end(), true) - locationsCol.begin();
                if (chains[0][nextr][nextc] == 0 and chains[1][nextr][nextc] == 0)
                {
                    chains[0][r][c] = n;
                    chainLength++;
                    if (find_chain(nextr, nextc, n, gr, gc, gn, 1))
                        return true;
                    else
                    {
                        chains[0][r][c] = 0;
                        chainLength--;
                    }
                }
            }
            std::vector<bool> locationsBlock = get_possible_locations_block(n, r, c);
            if (count(locationsBlock.begin(), locationsBlock.end(), true) == 2)
            {
                int b = r % 3 * 3 + c % 3;
                int nextb = find(locationsBlock.begin(), locationsBlock.end(), true) - locationsBlock.begin();
                if (nextb == b)
                    nextb = find(locationsBlock.begin() + b + 1, locationsBlock.end(), true) - locationsBlock.begin();
                int r0 = r / 3 * 3;
                int c0 = c / 3 * 3;
                int nextr = r0 + nextb / 3;
                int nextc = c0 + nextb % 3;
                if (chains[0][nextr][nextc] == 0 and chains[1][nextr][nextc] == 0)
                {
                    chains[0][r][c] = n;
                    chainLength++;
                    if (find_chain(nextr, nextc, n, gr, gc, gn, 1))
                        return true;
                    else
                    {
                        chains[0][r][c] = 0;
                        chainLength--;
                    }
                }
            }
        }

        if (colour == 1 and chains[0][r][c] == 0)
        { //weak link, looking for other numbers in the same square
            for (int nextn : options)
            {
                if (nextn != n)
                {
                    chains[1][r][c] = n;
                    chainLength++;
                    if (find_chain(r, c, nextn, gr, gc, gn, 0))
                        return true;
                    else
                    {
                        chains[1][r][c] = 0;
                        chainLength--;
                    }
                }
            }
        }
        if (colour == 1)
        { //weak link
            std::vector<bool> locationsRow = get_possible_locations_row(n, r);
            if (count(locationsRow.begin(), locationsRow.end(), true) > 2)
            {
                for (int nextc = 0; nextc < 9; nextc++)
                {
                    if (nextc != c and locationsRow[nextc] == true and chains[0][r][nextc] == 0 and chains[1][r][nextc] == 0)
                    {
                        chains[1][r][c] = n;
                        chainLength++;
                        if (find_chain(r, nextc, n, gr, gc, gn, 0))
                            return true;
                        else
                        {
                            chains[1][r][c] = 0;
                            chainLength--;
                        }
                    }
                }
            }
            std::vector<bool> locationsCol = get_possible_locations_col(n, c);
            if (count(locationsCol.begin(), locationsCol.end(), true) > 2)
            {
                for (int nextr = 0; nextr < 9; nextr++)
                {
                    if (nextr != r and locationsCol[nextr] == true and chains[0][nextr][c] == 0 and chains[1][nextr][c] == 0)
                    {
                        chains[1][r][c] = n;
                        chainLength++;
                        if (find_chain(nextr, c, n, gr, gc, gn, 0))
                            return true;
                        else
                        {
                            chains[1][r][c] = 0;
                            chainLength--;
                        }
                    }
                }
            }
            std::vector<bool> locationsBlock = get_possible_locations_block(n, r, c);
            if (count(locationsBlock.begin(), locationsBlock.end(), true) > 2)
            {
                int b = r % 3 * 3 + c % 3;
                int r0 = r / 3 * 3;
                int c0 = c / 3 * 3;
                for (int nextb = 0; nextb < 9; nextb++)
                {
                    if (nextb != b and locationsBlock[nextb] == true)
                    {
                        int nextr = r0 + nextb / 3;
                        int nextc = c0 + nextb % 3;
                        chains[1][r][c] = n;
                        chainLength++;
                        if ((r != nextr and c != nextc) and chains[0][nextr][nextc] == 0 and chains[1][nextr][nextc] == 0 and find_chain(nextr, nextc, n, gr, gc, gn, 0))
                            return true;
                        else
                        {
                            chains[1][r][c] = 0;
                            chainLength--;
                        }
                    }
                }
            }
        }

        if ((chainLength > 4 and (r == gr or c == gc or block_number(r, c) == block_number(gr, gc)) and n == gn))
        {
            if (r == gr)
            {
                std::vector<bool> locationsRow = get_possible_locations_row(n, r);
                if (count(locationsRow.begin(), locationsRow.end(), true) == 2 and colour == 0)
                {
                    chainLength++;
                    chains[colour][r][c] = n;
                    return true;
                }
                else if (count(locationsRow.begin(), locationsRow.end(), true) > 2 and colour == 1)
                {
                    chainLength++;
                    chains[colour][r][c] = n;
                    return true;
                }
            }
            if (c == gc)
            {
                std::vector<bool> locationsCol = get_possible_locations_col(n, c);
                if (count(locationsCol.begin(), locationsCol.end(), true) == 2 and colour == 0)
                {
                    chainLength++;
                    chains[colour][r][c] = n;
                    return true;
                }
                else if (count(locationsCol.begin(), locationsCol.end(), true) > 2 and colour == 1)
                {
                    chainLength++;
                    chains[colour][r][c] = n;
                    return true;
                }
            }
            if (block_number(r, c) == block_number(gr, gc))
            {
                std::vector<bool> locationsBlock = get_possible_locations_block(n, r, c);
                if (count(locationsBlock.begin(), locationsBlock.end(), true) == 2 and colour == 0)
                {
                    chainLength++;
                    chains[colour][r][c] = n;
                    return true;
                }
                else if (count(locationsBlock.begin(), locationsBlock.end(), true) > 2 and colour == 1)
                {
                    chainLength++;
                    chains[colour][r][c] = n;
                    return true;
                }
            }
        }
        //if ((chainLength > 4 and (r == gr or c == gc or (block_number(r, c) == block_number(gr, gc))) and allOptions[gr][gc].count(n) == 1 and chains[(colour==0)?1:0][gr][gc]==0))
        //{
        //    chainLength += 2;
        //    chains[colour][r][c] = n;
        //    chains[(colour==0)?1:0][gr][gc] = n;
        //    return true;
        //}

        return false;
    };

    for (int r = 0; r < 9 and found == false; r++)
    {
        for (int c = 0; c < 9 and found == false; c++)
        {
            if (puzzle[r][c] == 0)
            {
                chains = {{{0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}}, {{0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}}};
                chainLength = 0;
                strongStart = false;
                if (find_chain(r, c, 0, r, c, 0, 2))
                {
                    if (chainLength % 2 == 0) //if it's a nice loop
                    {
                        for (int n = 1; n < 10; n++) //checking every number separately
                        {
                            std::vector<std::pair<int, int>> rclist0;
                            std::vector<std::pair<int, int>> rclist1;
                            for (int y = 0; y < 9; y++)
                            {
                                for (int x = 0; x < 9; x++)
                                {
                                    if (chains[0][y][x] == n)
                                    {
                                        rclist0.push_back(std::make_pair(y, x));
                                    }
                                    else if (chains[1][y][x] == n)
                                    {
                                        rclist1.push_back(std::make_pair(y, x));
                                    }
                                }
                            }
                            for (std::pair<int, int> rc0 : rclist0)
                            {
                                for (std::pair<int, int> rc1 : rclist1)
                                {
                                    std::vector<std::pair<int, int>> rmlist = list_seen_by(rc0.first, rc0.second, rc1.first, rc1.second);
                                    for (std::pair<int, int> rm : rmlist)
                                    {
                                        if (chains[0][rm.first][rm.second] == 0 and chains[1][rm.first][rm.second] == 0)
                                        {
                                            if (allOptionsCopy[rm.first][rm.second].erase(n))
                                                found = true;
                                        }
                                    }
                                }
                            }
                        }
                        for (int y = 0; y < 9; y++)
                        {
                            for (int x = 0; x < 9; x++)
                            {
                                if (chains[0][y][x] != 0 and chains[1][y][x] != 0)
                                {
                                    allOptionsCopy[y][x] = {chains[0][y][x]%10, chains[1][y][x]%10};
                                    if (allOptions[y][x] != allOptionsCopy[y][x])
                                        found = true;
                                }
                            }
                        }
                    }
                    else if (strongStart)
                    {
                        allOptionsCopy[r][c] = {chains[0][r][c]};
                        found = true;
                    }
                    else if (!strongStart)
                    {
                        allOptionsCopy[r][c].erase(chains[1][r][c]);
                        found = true;
                    }
                }
            }
        }
    }

    if (found)
    {
        found = (allOptions != allOptionsCopy);
        allOptions = allOptionsCopy;
    }
    return found;
}

std::vector<std::vector<int>> Sudoku::get_puzzle()
{
    return puzzle;
}

void Sudoku::set_puzzle(std::vector<std::vector<int>> puzzle)
{
    initialized = false;
    solved = false; //because we don't know if the new puzzle is solved, set it to false.
    this->puzzle = puzzle;
}

void Sudoku::set_puzzle(std::string puzzleString)
{
    initialized = false;
    solved = false; //because we don't know if the new puzzle is solved, set it to false.
    for (int r = 0; r < 9; r++)
    {
        std::vector<int> row;
        for (int c = 0; c < 9; c++)
        {
            puzzle[r][c] = puzzleString[r * 9 + c] - '0'; //converts char to int and puts it in the row
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

bool Sudoku::solve()
{
    //first get all possibilities for each cell. This only happens once
    update_options();
    bool changed;
    do
    {
        changed = false;
        if (check_solved_cells())
        {
            //std::cout << "found solved cells" << std::endl;
            changed = true;
            continue; //if anything is found, go back to the beginning to check again
        }
        if (hidden_singles())
        {
            //std::cout << "found hidden singles" << std::endl;
            changed = true;
            continue;
        }
        if (naked_pairs())
        {
            //std::cout << "found naked pair" << std::endl;
            changed = true;
            continue;
        }
        if (naked_triples())
        {
            //std::cout << "found naked triple" << std::endl;
            changed = true;
            continue;
        }
        if (hidden_pairs())
        {
            //std::cout << "found hidden pair" << std::endl;
            changed = true;
            continue;
        }
        if (hidden_triples())
        {
            //std::cout << "found hidden triple" << std::endl;
            changed = true;
            continue;
        }
        if (pointing_pairs())
        {
            //std::cout << "found pointing pairs" << std::endl;
            changed = true;
            continue;
        }
        if (box_line_reduction())
        {
            //std::cout << "found box line reduction" << std::endl;
            changed = true;
            continue;
        }
        if (x_wing())
        {
            //std::cout << "found X-Wing" << std::endl;
            changed = true;
            continue;
        }
        if (simple_colouring())
        {
            //std::cout << "found Simple Colouring" << std::endl;
            changed = true;
            continue;
        }
        if (y_wing())
        {
            //std::cout << "found Y-wing" << std::endl;
            changed = true;
            continue;
        }
        if (swordfish())
        {
            //std::cout << "found swordfish" << std::endl;
            changed = true;
            continue;
        }
        if (xyz_wing())
        {
            //std::cout << "found XYZ wing" << std::endl;
            changed = true;
            continue;
        }
        if (x_cycles())
        {
            //std::cout << "found X-Cycle" << std::endl;
            changed = true;
            continue;
        }
        if (xy_chain())
        {
            //std::cout << "found XY-Chain" << std::endl;
            changed = true;
            continue;
        }
        if (three_d_medusa())
        {
            //std::cout << "found 3d-medusa" << std::endl;
            changed = true;
            continue;
        }
        if (jellyfish())
        {
            //std::cout << "found jellyfish" << std::endl;
            changed = true;
            continue;
        }
        if (unique_rectangles())
        {
            //std::cout << "found unique rectangles" << std::endl;
            changed = true;
            continue;
        }
        if (hidden_unique_rectangles())
        {
            //std::cout << "found hidden unique rectangles" << std::endl;
            changed = true;
            continue;
        }
        if (alternating_inference_chains())
        {
            //std::cout << "found alternating inference chains" << std::endl;
            changed = true;
            continue;
        }

    } while (changed == true);
    if (!is_solved())
    {
        //std::cout << std::endl;
        //print_puzzle();
        //std::cout << toString() << std::endl;
        //std::cout << "not solved using implemented checks. Resorting to backtracking" << std::endl;
        backtrack();
        return false;
    }
    return true;
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