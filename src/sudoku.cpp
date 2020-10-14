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

bool Sudoku::backtrack()
{
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
    for (int i = 0; i < 9; i++) //iterate(i) from 0 to 9 (all blocks)
    {
        std::vector<std::unordered_set<int>> optionsBlock = get_options_block(i); //get the block
        for (int j = 0; j < 8; j++)//iterate(j) from 0 to 8 (No need to check the last square because it will always be tested with an earlier square)
            {
            int r0 = i / 3 * 3;//calculate r and c for the square.
            int c0 = i % 3 * 3;
            int r = r0 + j / 3;
            int c = c0 + j % 3;
            if (puzzle[r][c] == 0)//if it isn't solved
            {
                for (int o:optionsBlock[j])//iterate through every option
                {
                    //if the option exist in at least one other square in the same row within the block (No need to check the last square in the row, going to use some maths magic)
                    if (j%3 != 2 and (optionsBlock[j+1].count(o) == 1 or optionsBlock[j/3*3+2].count(o) == 1)) //say you are testing square 4. 4%3 = 1, 4+1 == 5, 4/3 = 1, 1*3 = 3, 3+2 = 5
                    {
                        bool ispair = true;//create bool ispair to remember if the option has been found. default true
                        for (int k = 0; k < 9; k++)//iterate(k) through every square within the block
                        {
                            if (k/3 != j/3 and optionsBlock[k].count(o) == 1)//if the square is not in the same row as square j and it contains the number we're checking
                            {
                                ispair = false;//set bool to false
                                break;//no more need to test further.
                            }
                        }
                        if (ispair)//if bool ispair is still true
                        {
                            for (int k = 0; k < 9; k++)//iterate(k) through every square in the row
                            {
                                if (k/3 != c/3)//if it's not in the same block as the square
                                {
                                    if (allOptionsCopy[r][k].erase(o) == 1)//remove the number and check if there are actual changes
                                        found = true;//if so, found = true;
                                }
                            }
                        }
                    }
                    //if the option exist in at least one other square in the same col within the block (No need to check the last square in the col, going to use some maths magic)
                    if (j/3 != 2 and (optionsBlock[j+3].count(o) == 1 or optionsBlock[j%3+6].count(o) == 1)) //say you are testing square 4. 4/3 = 1, 4+3 == 7, 4%3 = 1, 1+6=7
                    {
                        bool ispair = true;//create bool ispair to remember if the option has been found. default true
                        for (int k = 0; k < 9; k++)//iterate(k) through every square within the block
                        {
                            if (k%3 != j%3 and optionsBlock[k].count(o) == 1)//if the square is not in the same col as square j and it contains the number we're checking
                            {
                                ispair = false;//set bool to false
                                break;//no more need to test further.
                            }
                        }
                        if (ispair)//if bool ispair is still true
                        {
                            for (int k = 0; k < 9; k++)//iterate(k) through every square in the col
                            {
                                if (k/3 != r/3)//if it's not in the same block as the square
                                {
                                    if (allOptionsCopy[k][c].erase(o) == 1)//remove the number and check if there are actual changes
                                        found = true;//if so, found = true;
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
        if (naked_pairs())
        {
            //            std::cout << "found naked pair" << std::endl;
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
            //std::cout << "found hidden triple" << std::endl;
            changed = true;
            continue;
        }

    } while (changed == true);
    if (!is_solved())
    {
        //std::cout << std::endl;
        //print_puzzle();
        //std::cout << "not solved using implemented checks. Resorting to backtracking" << std::endl;
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