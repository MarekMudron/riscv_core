all: instructions.o
	g++ -o main instructions.o -std=c++17 main.cc -I ELFIO -lstdc++fs

instructions.o: instructions.cc instructions.hpp
	g++ -c instructions.cc