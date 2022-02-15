#ifndef BOID_H
#define BOID_H

#include <SFML/Graphics.hpp>
#include "KinematicSprite.h"

class Boid : public KinematicSprite
{
private:
    int maxBreadCrumbs;
    sf::Sprite breadCrumbSprite;
    sf::Texture breadCrumbTexture;
    std::list<sf::Sprite> BreadCrumbs;

    int updateCount;
    int updatesPerBreadCrumb;
    int BoidID;

public:

    Boid(int numBreadCrumbs){
        maxBreadCrumbs = numBreadCrumbs;
        breadCrumbTexture.loadFromFile("images/breadcrumb.png");
        updateCount = 0;
        updatesPerBreadCrumb = 10;
    }

    int getID(){
        return BoidID;
    }

    void setID(int ID){
        BoidID = ID;
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
};

#endif
