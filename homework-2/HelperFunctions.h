#ifndef HELPERFUNCTIONS_H
#define HELPERFUNCTIONS_H

#include <SFML/Graphics.hpp>
#include <math.h>

static float magnitude(sf::Vector2f& vector)
{
    return sqrt(vector.x * vector.x + vector.y * vector.y);
}

static sf::Vector2f unitVector(sf::Vector2f& vector)
{
    return vector / magnitude(vector);
}

static float dotProduct(sf::Vector2f& vector1, sf::Vector2f& vector2)
{
    return vector1.x * vector2.x + vector1.y * vector2.y;
}

static float angleBetween(sf::Vector2f& vector1, sf::Vector2f& vector2)
{
    return acos(dotProduct(vector1, vector2) / (magnitude(vector1) * magnitude(vector2)));
}




#endif