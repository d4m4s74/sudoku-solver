#makefile for mingw32-make in windows 

bin\sudoku.exe: bin\main.o bin\sudoku.o 
	g++ bin\main.o bin\sudoku.o -o bin\sudoku.exe
bin\main.o: src\main.cpp
	g++ -c src\main.cpp -o bin\main.o
bin\sudoku.o: src\sudoku.cpp src\sudoku.h
	g++ -c src\sudoku.cpp -o bin\sudoku.o
clean:
	del  bin\*.o bin\*.exe >nul 2>&1
debug: 
	g++ -g src\main.cpp src\sudoku.cpp -o bin\sudoku.exe