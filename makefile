#makefile for unix style systems

bin\sudoku: bin\main.o bin\sudoku.o 
	g++ bin\main.o bin\sudoku.o -o bin\sudoku
bin\main.o: src\main.cpp
	g++ -c src\main.cpp -o bin\main.o
bin\sudoku.o: src\sudoku.cpp src\sudoku.h
	g++ -c src\sudoku.cpp -o bin\sudoku.o
clean:
	rm -f bin\*.o bin\sudoku
debug: 
	g++ -g src\main.cpp src\sudoku.cpp -o bin\sudoku