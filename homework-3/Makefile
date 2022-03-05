all: clean Tester

clean:
	rm -f *.o Main Tester

# Main: Main.o 
# 	g++  Main.o -o Main -lsfml-graphics -lsfml-window -lsfml-system -pthread

# Main.o: Main.cpp
# 	g++ -c Main.cpp

Tester: Tester.o
	g++ Tester.o -o Tester -lsfml-graphics -lsfml-window -lsfml-system -pthread
	
Tester.o: Tester.cpp
	g++ -c Tester.cpp