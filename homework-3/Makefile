all: clean Main 

clean:
	rm -f *.o Main GraphSearchTester IndoorMapTester

Main: Main.o 
	g++  Main.o -o Main -lsfml-graphics -lsfml-window -lsfml-system -pthread

Main.o: Main.cpp
	g++ -c Main.cpp

GraphSearchTester: GraphSearchTester.o
	g++  -std=c++11 GraphSearchTester.o -o GraphSearchTester -lsfml-graphics -lsfml-window -lsfml-system -pthread
	
GraphSearchTester.o: GraphSearchTester.cpp
	g++ -c GraphSearchTester.cpp

IndoorMapTester: IndoorMapTester.o
	g++  -std=c++11 IndoorMapTester.o -o IndoorMapTester -lsfml-graphics -lsfml-window -lsfml-system -pthread

IndoorMapTester.o: IndoorMapTester.cpp
	g++ -c IndoorMapTester.cpp