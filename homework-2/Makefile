all: Sprites

Sprites: Sprites.o 
	g++ Sprites.o -o Sprites -lsfml-graphics -lsfml-window -lsfml-system

Sprites.o: Sprites.cpp
	g++ -c Sprites.cpp

clean:
	rm -f *.o Sprites
