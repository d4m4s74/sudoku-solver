#include <iostream>
#include <vector>
#include <chrono>
#include <fstream>
#include <regex>
#include "sudoku.h"

int solve_file(std::string inputfile, std::string outputfile)
{
    std::ifstream input(inputfile);
    std::ofstream output(outputfile);
    int cases = 0;
    if (input.is_open() and output.is_open())
    {
        auto begin = std::chrono::high_resolution_clock::now();

        input >> cases;
        Sudoku sudoku;
        for (int i = 0; i < cases; i++)
        {   
            input >> sudoku;
            sudoku.solve();
            output << sudoku << std::endl;
            std::cout << i + 1 << "/" << cases << "\r";
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
        std::cout << std::endl
                  << "Solved " << cases << " puzzles in " << elapsed.count() * 1e-9 << " seconds." << std::endl;
    }
    else
    {
        return -1;
    }
    return cases;
}

int solve_file(std::string inputfile)
{
    std::ifstream input(inputfile);
    int cases = 0;
    if (input.is_open())
    {
        auto begin = std::chrono::high_resolution_clock::now();

        input >> cases;
        Sudoku sudoku;
        for (int i = 0; i < cases; i++)
        {
            input >> sudoku;
            sudoku.solve();
            std::cout << sudoku << std::endl;
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
        std::cout << std::endl
                  << "Solved " << cases << " puzzles in " << elapsed.count() * 1e-9 << " seconds." << std::endl;
    }
    else
    {
        return -1;
    }
    return cases;
}

void solve_string(std::string puzzleString)
{
    Sudoku sudoku(puzzleString);
    sudoku.print_puzzle();
    std::cout << std::endl;
    sudoku.solve();
    std::cout << "solved:" << std::endl
              << std::endl;
    sudoku.print_puzzle();
}

std::string getFileName(const std::string &s)
{

    char sep = '/';

#ifdef _WIN32
    sep = '\\';
#endif

    size_t i = s.rfind(sep, s.length());
    if (i != std::string::npos)
    {
        return (s.substr(i + 1, s.length() - i));
    }

    return ("");
}

void helptext(char **argv)
{
    std::string filename = getFileName(argv[0]);
    std::cout << "usage: " << std::endl;
    std::cout << "\t" << filename << " [options] inputfile [outputfile]" << std::endl;
    std::cout << "\t" << filename << " [options] puzzle" << std::endl;
    std::cout << "options:" << std::endl;
    std::cout << "\t-h,--help\t Display this information" << std::endl;
}

int main(int argc, char **argv)
{
    if (argc == 2)
    {
        if (std::string(argv[1]) == "-h" or std::string(argv[1]) == "--help")
        {
            helptext(argv);
        }
        else if (std::regex_match(argv[1], std::regex("([0-9]{81})")))
        {
            solve_string(argv[1]);
        }
        else
        {
            solve_file(argv[1]);
        }
    }
    else if (argc == 3)
    {
        solve_file(argv[1], argv[2]);
    }
    else
        helptext(argv);
}