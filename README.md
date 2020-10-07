# sudoku-solver
A c++ sudoku solver, trying to avoid backtracking  
Currently implemented: hidden singles, naked pairs and naked triples (and backtracking)  
  
Script currently accepts single strings representing the puzzle (ex: 800010000020600050037008400000007260070030009000805300010950000000000103406000000)  
Script also accepts files containing multiple puzzles. (see testcases folder for examples)

usage:  
        sudoku.exe [options] inputfile [outputfile]  
        sudoku.exe [options] puzzle  
options:  
        -h,--help        Display this information  

