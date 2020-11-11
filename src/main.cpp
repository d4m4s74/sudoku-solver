#include <iostream>
#include <vector>
#include <chrono>
#include <fstream>
#include <regex>
#include "sudoku.h"
#include <unistd.h>
#include <future>
#include <mutex>

int cases;
int cur = 0;
std::vector<std::string> puzzles;
std::vector<std::string> solutions;
std::mutex current;
std::mutex cout;
int solve_puzzles(int t)
{
    int solved = 0;
    int solving;
    Sudoku sudoku;
    std::string puzzle;
    while (true)
    {
        current.lock();
        solving = cur++;
        current.unlock();
        puzzle = puzzles[solving];
        if (solving >= cases)
            return solved;
        sudoku.set_puzzle(puzzle);
        solved += sudoku.solve();
        if (sudoku.is_solved()) solutions[solving] = puzzle + "," + sudoku.toString();
        else solutions[solving] = puzzle + ",error                                                                            ";
        if (cout.try_lock())
        {
            std::cout << std::min(cases, cur) << "/" << cases << "\r";
            if (cur%100 == 0) std::cout.flush();
            cout.unlock();
        }
    }
}

int solve_file_parallel(std::string inputfile, std::string outputfile, int threads)
{
    std::ifstream input(inputfile);
    std::ofstream output(outputfile);
    if (input.is_open() and output.is_open())
    {
        auto begin = std::chrono::high_resolution_clock::now();
        auto last = std::chrono::high_resolution_clock::now();
        input >> cases;
        int solved = 0;
        Sudoku sudoku;
        std::string puzzleString;
        output << cases << "\n";
        std::vector<std::future<int>> futures;

        for (int i = 0; i < cases; i++)
        {
            input >> puzzleString;
            puzzles.push_back(puzzleString);
            solutions.push_back("");
        }
        cout.lock();
        for (int i = 0; i < threads; i++)
        {
            futures.push_back(std::async(solve_puzzles, i + 1));
            std::cout << "started thread " << i + 1 << std::endl;
        }
        cout.unlock();
        while (solved < cases)
        {
            if (solutions[solved] != "" and solutions[solved].length() == 163)
            {
                output << solutions[solved] << std::endl;
                solved++;
            }
            else
                usleep(20000);
        }
        for (int i = 0; i < threads; i++)
        {
            solved -= futures[i].get();
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
        std::cout << std::endl
                  << "Solved " << cases << " puzzles in " << elapsed.count() * 1e-9 << " seconds." << std::endl;
        std::cout << solved << " cases required backtracking" << std::endl;
    }
    else
    {
        return -1;
    }
    return cases;
}

int solve_file(std::string inputfile, std::string outputfile, bool verbose = false, bool timed = false)
{
    std::ios::sync_with_stdio(false);
    std::ifstream input(inputfile);
    std::ofstream output(outputfile);
    int cases = 0;
    if (input.is_open() and output.is_open())
    {
        auto begin = std::chrono::high_resolution_clock::now();
        auto last = std::chrono::high_resolution_clock::now();
        input >> cases;
        int solved;
        Sudoku sudoku;
        std::string puzzleString;
        output << cases << "\n";

        for (int i = 0; i < cases; i++)
        {
            input >> puzzleString;
            output << puzzleString << ",";
            sudoku.set_puzzle(puzzleString);
            solved += sudoku.solve();
            if (sudoku.is_solved()) output << sudoku << "\n";
            else output << "error" << "\n";
            if (verbose and timed)
            {
                std::cout << "Solved case " << i + 1 << "/" << cases << " in " << std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - last).count() * 1e-9 << " seconds \n";
                last = std::chrono::high_resolution_clock::now();
                std::cout.flush();
            }
            else if (timed and (i % 100 == 99 or i + 1 == cases))
            {
                std::cout << "Solved cases " << i - 99 << "-" << i + 1 << "/" << cases << " in " << std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - last).count() * 1e-9 << " seconds \n";
                last = std::chrono::high_resolution_clock::now();
                std::cout.flush();
            }
            else if (verbose or i % 100 == 99 or i + 1 == cases)
            {
                std::cout << i + 1 << "/" << cases << '\r';
                //last = std::chrono::high_resolution_clock::now();
                std::cout.flush();
            }
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
        std::cout << std::endl
                  << "Solved " << cases << " puzzles in " << elapsed.count() * 1e-9 << " seconds." << std::endl;
        std::cout << cases - solved << " cases required backtracking" << std::endl;
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
        int solved;
        Sudoku sudoku;
        for (int i = 0; i < cases; i++)
        {
            input >> sudoku;
            std::cout << sudoku << ",";
            solved += sudoku.solve();
            if (sudoku.is_solved()) std::cout << sudoku << std::endl;
            else std::cout << "error" << std::endl;
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
        std::cout << std::endl
                  << "Solved " << cases << " puzzles in " << elapsed.count() * 1e-9 << " seconds." << std::endl;
        std::cout << cases - solved << " cases required backtracking" << std::endl;
    }
    else
    {
        return -1;
    }
    return cases;
}

int solve_cin()
{

    int cases = 0;

    Sudoku sudoku;
    std::string puzzleString;
    while (std::cin >> puzzleString)
    {
        if (puzzleString.length() == 81)
        {
            sudoku.set_puzzle(puzzleString);
            sudoku.solve();
            std::cout << puzzleString << ",";
            if (sudoku.is_solved()) std::cout << sudoku << std::endl;
            else std::cout << "error" << std::endl;
            cases++;
        }
        else if (puzzleString.find_first_not_of("0123456789") == std::string::npos)
            std::cout << puzzleString << std::endl;
        else 
        {
            return -1;
        }
    }

    return cases;
}

void solve_string(std::string puzzleString, bool showPuzzle = true, bool timed = false)
{
    auto begin = std::chrono::high_resolution_clock::now();
    Sudoku sudoku(puzzleString);
    if (showPuzzle)
    {
        sudoku.print_puzzle();
        std::cout << std::endl;
        sudoku.solve();
        std::cout << "solved:" << std::endl
                  << std::endl;
        sudoku.print_puzzle();
    }
    else
    {
        sudoku.solve();
        std::cout << sudoku << std::endl;
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
    if (timed)
        std::cout << "Solved puzzle in " << elapsed.count() * 1e-9 << " seconds." << std::endl;
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
    std::cout << "\t-v,--verbose\t In case of file, show counter for every puzzle instead of every 100" << std::endl;
    std::cout << "\t-t,--timed\t Times individual puzzles, or sets of 100 depending on option -v" << std::endl;
    std::cout << "\t-s\t\t In case of puzzle string, only returns solved string" << std::endl;
    std::cout << "\t-p <threads>\t Solve puzzles in parallel" << std::endl;
}

int main(int argc, char **argv)
{
    bool cin = false;
    int size = std::cin.rdbuf()->in_avail();
    if (!isatty(STDIN_FILENO))
        cin = true;
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
            if (access(argv[argc - 1], R_OK) == 0) //if the last argument is a readable file
            {
                solve_file(argv[argc - 1]); //treat it as a the input file
            }
            else
                std::cout << "Unable to open file " << argv[argc - 1] << std::endl;
        }
    }
    else if (argc >= 3 or cin)
    {
        bool verbose = false;
        bool showPuzzle = true;
        bool timed = false;
        bool parallel = false;
        int threads = 0;
        for (int i = 1; i < argc; i++)
        {
            if (std::string(argv[i]) == "-h" or std::string(argv[i]) == "--help")
            {
                helptext(argv);
                return 0; //exit out of the program
            }
            else if (std::string(argv[i]) == "-v" or std::string(argv[i]) == "--verbose")
            {
                verbose = true;
            }
            else if (std::string(argv[i]) == "-t" or std::string(argv[i]) == "--timed")
            {
                timed = true;
            }
            else if (std::string(argv[i]) == "-s")
            {
                showPuzzle = false;
            }
            else if (std::string(argv[i]) == "-p")
            {
                parallel = true;
                threads = std::stoi(argv[i + 1]);
            }
        }
        if (cin)
        {
            solve_cin();
        }
        else if (access(argv[argc - 2], R_OK) == 0) //if the second to last argument is a readable file
        {
            if (!parallel)
                solve_file(argv[argc - 2], argv[argc - 1], verbose, timed); //treat it as the input file, and the last argument as the output
            else
                solve_file_parallel(argv[argc - 2], argv[argc - 1], threads);
        }
        else if (access(argv[argc - 1], R_OK) == 0) //if the last argument is a readable file
        {
            solve_file(argv[argc - 1]); //treat it as a the input file
        }
        else if (std::regex_match(argv[argc - 1], std::regex("([0-9]{81})")))
        {
            solve_string(argv[argc - 1], showPuzzle, timed);
        }
        else
            std::cout << argv[argc - 1] << " is not a valid file or puzzle string" << std::endl;
    }
    else
        helptext(argv);
}