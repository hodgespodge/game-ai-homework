#ifndef BOID_H
#define BOID_H

#include <SFML/Graphics.hpp>
#include "KinematicSprite.h"
#include <list>

class Boid : public KinematicSprite
{
private:
    int maxBreadCrumbs;
    sf::Sprite breadCrumbSprite;
    sf::Texture breadCrumbTexture;
    std::list<sf::Sprite> BreadCrumbs;

    sf::Font font;
    sf::Text text;

    float breadCrumbScale;
    int updateCount;
    int updatesPerBreadCrumb;
    int BoidID;

public:

    Boid(int numBreadCrumbs, float breadCrumbScale){

        maxBreadCrumbs = numBreadCrumbs;
        this->breadCrumbScale = breadCrumbScale;
        breadCrumbTexture.loadFromFile("images/breadcrumb.png");
        
        if (!font.loadFromFile("fonts/arial.ttf")){
            printf("Could not load font\n");
        };
        updateCount = 0;
        updatesPerBreadCrumb = 10;

        text.setFont(font);
        text.setCharacterSize(15);
        text.setColor(sf::Color::Black);
    }

    int getID(){
        return BoidID;
    }

    void setID(int ID){
        BoidID = ID;
        text.setString(std::to_string(BoidID));
    }

    void updateBreadcrumbs(){
        updateCount++;

        if (updateCount % updatesPerBreadCrumb == 0)
        {
            updateCount = 0;

            if (BreadCrumbs.size() > maxBreadCrumbs)
            {
                breadCrumbSprite = BreadCrumbs.back();
                BreadCrumbs.pop_back();
            }
            
            breadCrumbSprite.setTexture(breadCrumbTexture);
            breadCrumbSprite.setPosition(getPosition());
            breadCrumbSprite.setRotation(getRotation());    
            breadCrumbSprite.setColor(sf::Color(0,0,0, 255));
            breadCrumbSprite.setScale(breadCrumbScale, breadCrumbScale);

            BreadCrumbs.push_front(breadCrumbSprite);
        }
    }

    void drawBreadcrumbs(sf::RenderWindow& window, bool fadeBreadcrumbs){

        int i = 0;
        for (std::list<sf::Sprite>::iterator it = BreadCrumbs.begin(); it != BreadCrumbs.end(); ++it)
        {
            if (fadeBreadcrumbs)
            {
                it->setColor(sf::Color(0,0,0, 255 - i * (255/maxBreadCrumbs)));
            }
            window.draw(*it);
            i++;
        }

    }

    // used for debugging boid behavior. Displays the boid ID above the boid
    void drawID(sf::RenderWindow& window){

        text.setPosition(getPosition().x, getPosition().y - 30);
        window.draw(text);
    }
    
};

#endif
