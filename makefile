#makefile for unix style systems

bin/sudoku: bin/main.o bin/sudoku.o 
	g++ -pthread bin/main.o bin/sudoku.o -o bin/sudoku
bin/main.o: src/main.cpp
	g++ -c -pthread src/main.cpp -o bin/main.o
bin/sudoku.o: src/sudoku.cpp src/sudoku.h
	g++ -c src/sudoku.cpp -o bin/sudoku.o
clean:
	rm -f bin/*.o bin/sudoku
debug: 
	g++ -g -pthread src/main.cpp src/sudoku.cpp -o debug/sudoku