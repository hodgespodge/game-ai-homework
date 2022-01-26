SingleSprite: SingleSprite.o 
	g++ SingleSprite.o -o SingleSprite -lsfml-graphics -lsfml-window -lsfml-system


SingleSprite.o: SingleSprite.cpp
	g++ -c SingleSprite.cpp

MultiSprite: MultiSprite.o 
	g++ MultiSprite.o -o MultiSprite -lsfml-graphics -lsfml-window -lsfml-system

MultiSprite.o: MultiSprite.cpp
	g++ -c MultiSprite.cpp