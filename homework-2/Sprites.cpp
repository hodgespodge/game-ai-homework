#include <SFML/Graphics.hpp>
// #include <vector>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <list>
#include "SteeringBehavior.h"
#include "VelocityMatching.h"
#include "ArriveAndAlign.h"
// #include "KinematicSprite.h"
#include "Wander.h"
#include "Boid.h"

int main(int argc, char *argv[])
{
    sf::Clock clock;
    sf::RenderWindow window(sf::VideoMode(640, 480), "Demonstrating Steering Behavior");

    sf::Texture texture;
    if(!texture.loadFromFile("images/boid-sm.png")){
        return EXIT_FAILURE;
    }

    std::list<Boid> Boids;

    // initialize a Boid
    Boid sprite;
    int numBoids;
    bool drawBreadcrumbs = false;
    bool fadeBreadcrumbs = false;

    SteeringBehavior* steeringBehavior = NULL;

    // if second argument is "v", use velocity matching
    if(argc > 1 && strcmp(argv[1], "-v") == 0){
        steeringBehavior = new VelocityMatching(window);
        numBoids = 10;
        drawBreadcrumbs = false;
        fadeBreadcrumbs = false;
    } else if(argc > 1 && strcmp(argv[1], "-a") == 0){
        steeringBehavior = new ArriveAndAlign(window);
        numBoids = 10;
        drawBreadcrumbs = true;
        fadeBreadcrumbs = true;
    } else if(argc > 1 && strcmp(argv[1], "-w") == 0){
        steeringBehavior = new Wander(window);
        numBoids = 5;
        drawBreadcrumbs = true;
        fadeBreadcrumbs = true;
    } else {
        printf("Using default steering behavior: VelocityMatching\n");
        steeringBehavior = new VelocityMatching(window);
        numBoids = 10;
        drawBreadcrumbs = false;
        fadeBreadcrumbs = false;
    }

    // Create 10 KinematicBoids with random positions inside the window and add them to the list
    for(int i = 0; i < numBoids; i++){
        sprite.setTexture(texture);
        sprite.setPosition(rand() % (int)window.getSize().x, rand() % (int)window.getSize().y);
        Boids.push_back(sprite);
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
            for(auto &s : Boids){
                steeringBehavior->updateSprite(s, clock.getElapsedTime().asMilliseconds() );
            }

            steeringBehavior->postUpdate();
            
            clock.restart();
        }

        // clear the window with white color
        window.clear(sf::Color::White);

        if (drawBreadcrumbs)
        {
            for(auto &s : Boids){
                s.drawBreadcrumbs(window, fadeBreadcrumbs);
                window.draw(s);
            }
        }
        else{
            for(auto &s : Boids){
                window.draw(s);
            }
        }

        window.display();
    }
    
    return 0;
}

