#include <SFML/Graphics.hpp>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <list>
#include "SteeringBehavior.h"
#include "VelocityMatching.h"
#include "ArriveAndAlign.h"
#include "Wander.h"
#include "Flocking.h"
#include "Boid.h"

int main(int argc, char *argv[])
{

    // int windowWidth = 640;
    // int windowHeight = 480;

    int windowWidth = 1920;
    int windowHeight = 1080;

    sf::Clock clock;
    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Demonstrating Steering Behavior");

    sf::Texture texture;
    if(!texture.loadFromFile("images/boid-sm.png")){
        return EXIT_FAILURE;
    }

    std::list<Boid*> boids;
    int targetFPS = 40;
    int numBreadCrumbs;
    int numBoids;
    bool drawBreadcrumbs;
    bool fadeBreadcrumbs;
    bool drawID;

    SteeringBehavior* steeringBehavior = NULL;

    // if second argument is "v", use velocity matching
    if(argc > 1 && strcmp(argv[1], "-v") == 0){
        steeringBehavior = new VelocityMatching(window);

    } else if(argc > 1 && strcmp(argv[1], "-a") == 0){
        steeringBehavior = new ArriveAndAlign(window);

    } else if(argc > 1 && strcmp(argv[1], "-w") == 0){
        steeringBehavior = new Wander(window);

    } else if(argc > 1 && strcmp(argv[1], "-f") == 0){
        steeringBehavior = new Flocking(window,boids);
     
    } else {
        printf("Using default steering behavior: VelocityMatching\n");
        // steeringBehavior = new Flocking(window, boids);
        steeringBehavior = new Flocking(window, boids);

    }

    numBoids = steeringBehavior->numBoids;
    numBreadCrumbs = steeringBehavior->numBreadCrumbs;
    drawBreadcrumbs = steeringBehavior->drawBreadcrumbs;
    fadeBreadcrumbs = steeringBehavior->fadeBreadcrumbs;
    drawID = steeringBehavior->drawID;

    for(int i = 0; i < numBoids; i++){

        Boid * sprite = new Boid(numBreadCrumbs);

        sprite->setTexture(texture);
        sprite->setPosition(rand() % (int)window.getSize().x, rand() % (int)window.getSize().y);
        sprite->linearVelocity = 0.05f*sf::Vector2f(rand() % 10 - 5, rand() % 10 - 5);
        sprite->setRotation(rand() % 360);
        sprite->setID(i);
        boids.push_back(sprite);

    }

    int updateFrames = 1000/targetFPS;

    while (window.isOpen())
    {

        sf::Event event;
        while (window.pollEvent(event))
        {

            if (event.type == sf::Event::Closed)
                window.close();

            steeringBehavior->checkEvent(event);
            
        }

        if (clock.getElapsedTime().asMilliseconds() > updateFrames)
        {

            // update each sprite using the steering behavior
            for(auto s : boids){
                
                steeringBehavior->updateSprite(*s, clock.getElapsedTime().asMilliseconds() );
            }

            steeringBehavior->postUpdate();
            
            clock.restart();
        }

        // clear the window with white color
        window.clear(sf::Color::White);

        if (drawBreadcrumbs)
        {
            for(auto s : boids){
                // *s.drawBreadcrumbs(window, fadeBreadcrumbs);
                s->drawBreadcrumbs(window, fadeBreadcrumbs);
                window.draw(*s);
            }
        }

        if (drawID)
        {
            for(auto s : boids){
                // s.drawID(window);
                s->drawID(window);
                window.draw(*s);
            }
        }

        else{
            for(auto s : boids){
                window.draw(*s);
            }
        }

        window.display();
    }
    
    return 0;
}

