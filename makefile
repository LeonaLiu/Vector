test : main.o
	g++ -std=c++11 -o test main.o

main.o : main.cpp Vector.h
	g++ -std=c++11 -c main.cpp

clean : 
	rm test main.o
