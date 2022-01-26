#include <SFML/Graphics.hpp>

int main()
{
    sf::Clock clock;
    sf::RenderWindow window(sf::VideoMode(640, 480), "Moving a sprite");


    sf::Texture texture;
    if(!texture.loadFromFile("images/boid-sm.png")){
        return EXIT_FAILURE;
    }

    sf::Sprite sprite;
    sprite.setTexture(texture);
    sprite.setPosition(sf::Vector2f(0, 100.f));

    while (window.isOpen())
    {
        if (clock.getElapsedTime().asMilliseconds() > 5)
        {
            if(sprite.getPosition().x > window.getSize().x){
                sprite.setPosition(sf::Vector2f(0, 100.f));
            }
            else{
                sprite.move(1.f, 0.f);
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

        window.draw(sprite);
        window.display();
    }
    
    return 0;
}

