#include <SFML/Graphics.hpp>
// #include <vector>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <list>
#include "SteeringBehavior.h"
#include "VelocityMatching.h"
#include "ArriveAndAlign.h"
#include "KinematicSprite.h"
#include "Wander.h"

int main(int argc, char *argv[])
{
    sf::Clock clock;
    sf::RenderWindow window(sf::VideoMode(640, 480), "Demonstrating Steering Behavior");

    sf::Texture texture;
    if(!texture.loadFromFile("images/boid-sm.png")){
        return EXIT_FAILURE;
    }

    std::list<KinematicSprite> kinematicSprites;

    // initialize a KinematicSprite
    KinematicSprite sprite;
    int numKinematicSprites = 10;

    SteeringBehavior* steeringBehavior = NULL;

    // if second argument is "v", use velocity matching
    if(argc > 1 && strcmp(argv[1], "-v") == 0){
        steeringBehavior = new VelocityMatching(window);
    } else if(argc > 1 && strcmp(argv[1], "-a") == 0){
        steeringBehavior = new ArriveAndAlign(window);
        numKinematicSprites = 1;
    } else if(argc > 1 && strcmp(argv[1], "-w") == 0){
        steeringBehavior = new Wander(window);
        numKinematicSprites = 1;
    } else {
        printf("Using default steering behavior: VelocityMatching\n");
        steeringBehavior = new VelocityMatching(window);
    }

    // Create 10 KinematickinematicSprites with random positions inside the window and add them to the list
    for(int i = 0; i < numKinematicSprites; i++){
        sprite.setTexture(texture);
        sprite.setPosition(rand() % (int)window.getSize().x, rand() % (int)window.getSize().y);
        kinematicSprites.push_back(sprite);
    }

    while (window.isOpen())
    {

        sf::Event event;
        while (window.pollEvent(event))
        {

            if (event.type == sf::Event::Closed)
                window.close();

            steeringBehavior->checkEvent(event);
            
        }

        // steeringBehavior->eventPoll();

        if (clock.getElapsedTime().asMilliseconds() > 10)
        {

            // update each sprite using the steering behavior
            for(auto &s : kinematicSprites){
                steeringBehavior->updateSprite(s, clock.getElapsedTime().asMilliseconds() );
            }

            steeringBehavior->postUpdate();
            
            clock.restart();
        }

        // clear the window with white color
        window.clear(sf::Color::White);

        steeringBehavior->drawExtra(window);

        for(auto &s : kinematicSprites){
            window.draw(s);
        }

        // Detect here for mouse click event?

        window.display();
    }
    
    return 0;
}

