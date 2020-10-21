# sudoku-solver
A c++ sudoku solver, trying to avoid backtracking  
Currently implemented: hidden singles, naked pairs, naked triples, hidden pairs, hidden triples, pointing pairs, box/line reduction and x-wing (and backtracking)  
  
Script currently accepts single strings representing the puzzle (ex: 800010000020600050037008400000007260070030009000805300010950000000000103406000000)  
Script also accepts files containing multiple puzzles. (see testcases folder for examples)

usage:
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;sudoku.exe [options] inputfile [outputfile]
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;sudoku.exe [options] puzzle
options:
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-h,--help&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Display this information
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-v,--verbose&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;In case of file, show counter for every puzzle instead of every 100
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-t,--timed&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;In case of file,times individual puzzles, or sets of 100 depending on if -v is set
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-s&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;In case of puzzle string, only returns solved string

