SingleSprite: SingleSprite.o 
	g++ SingleSprite.o -o SingleSprite -lsfml-graphics -lsfml-window -lsfml-system


SingleSprite.o: SingleSprite.cpp
	g++ -c SingleSprite.cpp

