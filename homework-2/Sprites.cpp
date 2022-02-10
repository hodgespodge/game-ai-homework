#include <SFML/Graphics.hpp>
// #include <vector>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <list>
#include "SteeringBehavior.h"
#include "VelocityMatching.h"
#include "KinematicSprite.h"

int main(int argc, char *argv[])
{
    sf::Clock clock;
    sf::RenderWindow window(sf::VideoMode(640, 480), "Demonstrating Steering Behavior");

    sf::Texture texture;
    if(!texture.loadFromFile("images/boid-sm.png")){
        return EXIT_FAILURE;
    }

    std::list<KinematicSprite> sprites;    

    // initialize a KinematicSprite
    KinematicSprite sprite;

    // Create 10 KinematicSprites with random positions inside the window and add them to the list
    for(int i = 0; i < 10; i++){
        sprite.setTexture(texture);
        sprite.setPosition(rand() % (int)window.getSize().x, rand() % (int)window.getSize().y);
        sprites.push_back(sprite);
    }

    SteeringBehavior* steeringBehavior = NULL;

    // if second argument is "v", use velocity matching
    if(argc > 1 && strcmp(argv[1], "-v") == 0){
        steeringBehavior = new VelocityMatching(window);
    } else {
        printf("Using default steering behavior: VelocityMatching\n");
        steeringBehavior = new VelocityMatching(window);
    }
    

    while (window.isOpen())
    {
        if (clock.getElapsedTime().asMilliseconds() > 5)
        {

            // update each sprite using the steering behavior
            for(auto &s : sprites){
                steeringBehavior->updateSprite(s, clock.getElapsedTime().asMilliseconds() );
            }

            steeringBehavior->postUpdate();
            
            clock.restart();
        }

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // clear the window with white color
        window.clear(sf::Color::White);

        for(auto &s : sprites){
            window.draw(s);
        }

        // Detect here for mouse click event?

        // window.draw(sprite);
        window.display();
    }
    
    return 0;
}

