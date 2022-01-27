#include <SFML/Graphics.hpp>
// #include <vector>
#include <math.h>
#include <list>

int main()
{
    sf::Clock clock;
    sf::RenderWindow window(sf::VideoMode(640, 480), "Moving multiple sprites");


    sf::Texture texture;
    if(!texture.loadFromFile("images/boid-sm.png")){
        return EXIT_FAILURE;
    }

    std::list<sf::Sprite> sprites;    

    sf::Sprite sprite;

    float offset = 20.f;
    float speed = 500.f;
    bool addingSprites = true;

    sprite.setTexture(texture);
    
    sprite.setPosition(sf::Vector2f(offset, offset));

    sprites.push_back(sprite);
    

    while (window.isOpen())
    {
        if (clock.getElapsedTime().asMilliseconds() > 5)
        {

            int num_sprites = sprites.size();

            if (num_sprites == 0){
                sf::Sprite sprite;
                sprite.setTexture(texture);
                sprite.setPosition(sf::Vector2f(offset, offset));
                sprites.push_back(sprite);

                addingSprites = true;
            } else if (num_sprites >= 4){
                addingSprites = false;
            }

            // Remove a sprite if it reaches the top right of the screen
            sprites.remove_if([](const sf::Sprite& s){ 
                return s.getPosition().y < 20.f;
            });

            for(auto &s : sprites){
                
                int sX = s.getPosition().x;
                int sY = s.getPosition().y;

                // rotate the sprite when it reaches the top right of the screen
                if(sX > window.getSize().x - offset){
                    s.setRotation(s.getRotation() + 90);
                    s.setPosition(sf::Vector2f(window.getSize().x - offset, sY));

                    // add a sprite whenever a sprite reachces the top right corner
                    if(addingSprites){
                        sf::Sprite sprite;
                        sprite.setTexture(texture);
                        sprite.setPosition(sf::Vector2f(offset, offset));
                        sprites.push_back(sprite);
                    }
                }

                // rotate the sprite when it reaches the bottom right of the screen
                if(sY > window.getSize().y - offset){
                    s.setRotation(s.getRotation() + 90);
                    s.setPosition(sf::Vector2f(sX, window.getSize().y - offset));
                }

                // rotate the sprite when it reaches the bottom left of the screen
                if(sX < offset){
                    s.setRotation(s.getRotation() + 90);
                    s.setPosition(sf::Vector2f(offset, sY));
                }

                float angle = s.getRotation();

                // calculate the x,y move based on the current angle of rotation
                s.move(speed* cos(angle * 3.14159265359 / 180.f) / window.getSize().y, speed*sin(angle * 3.14159265359 / 180.f) / window.getSize().x);

            }

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

        // window.draw(sprite);
        window.display();
    }
    
    return 0;
}

