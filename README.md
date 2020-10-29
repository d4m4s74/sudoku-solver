# sudoku-solver
A c++ sudoku solver, trying to avoid backtracking  
Currently implemented: hidden singles, naked pairs, naked triples, hidden pairs, hidden triples, pointing pairs, box/line reduction, X-wing, simple colouring, Y-wing, Swordfish and XYZ wing (and backtracking)  
  
Script currently accepts single strings representing the puzzle (ex: 800010000020600050037008400000007260070030009000805300010950000000000103406000000)  
Script also accepts files containing multiple puzzles. (see testcases folder for examples)

```
usage:  
        sudoku.exe [options] inputfile [outputfile]  
        sudoku.exe [options] puzzle  
options:  
        -h,--help       Display this information  
        -v,--verbose    In case of file, show counter for every puzzle instead of every 100  
        -t,--timed      In case of file,times individual puzzles, or sets of 100 depending on if -v is set  
        -s              In case of puzzle string, only returns solved string  
        -p <threads>    Solve puzzles in parallel
```
