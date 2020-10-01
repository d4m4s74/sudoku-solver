#include <iostream>
#include <vector>
#include <chrono>
#include "sudoku.h"

std::vector<std::vector<int>> mediumPuzzle = {
    {0,8,5,3,1,9,0,0,0},
    {0,0,0,0,5,2,6,0,0},
    {4,0,3,0,0,0,9,0,0},
    {0,0,9,0,0,0,8,0,0},
    {0,0,0,0,2,7,0,0,0},
    {0,3,4,1,0,8,0,0,0},
    {8,0,6,0,0,4,0,3,0},
    {0,0,0,2,0,0,0,0,8},
    {0,9,0,8,3,5,7,0,0}};
std::string mediumPuzzleString = "085319000000052600403000900009000800000027000034108000806004030000200008090835700";
std::string hardPuzzleString =   "400000701000705000030000400920000000100020000047500000003904276000050030000010940";
std::string expertPuzzleString = "000800001600000500000000000401050000030000080000000700000108040020300000500000600";
std::string solvedPuzzleString = "685319427971452683423786915269543871518627349734198256856974132347261598192835764";

int main()
{
    Sudoku puzzle1;
    //std::cin >> puzzle1;
    //Sudoku puzzle2 (mediumPuzzleString);
    //Sudoku puzzle3 (solvedPuzzleString);
    
    
    auto begin = std::chrono::high_resolution_clock::now();
    puzzle1.set_puzzle(hardPuzzleString);
    puzzle1.print_puzzle();
    std::cout << std::endl;
    puzzle1.solve();
    puzzle1.print_puzzle();
    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
    std::cout << elapsed.count() * 1e-9;
    return 0;
}