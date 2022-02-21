all: clean Sprites multiSprites

Sprites: Sprites.o 
	g++ Sprites.o -o Sprites -lsfml-graphics -lsfml-window -lsfml-system

Sprites.o: Sprites.cpp
	g++ -c Sprites.cpp

clean:
	rm -f *.o Sprites multiSprites

multiSprites: multiSprites.o 
	g++  multiSprites.o -o multiSprites -lsfml-graphics -lsfml-window -lsfml-system -pthread

multiSprites.o: multiSprites.cpp
	g++ -c multiSprites.cpp