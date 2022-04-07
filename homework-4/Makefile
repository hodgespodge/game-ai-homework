all: clean Main 

clean:
	rm -f *.o Main

Main: Main.o 
	g++ Main.o -o Main -lsfml-graphics -lsfml-window -lsfml-system -pthread

Main.o: Main.cpp
	g++ -c -std=c++17 Main.cpp