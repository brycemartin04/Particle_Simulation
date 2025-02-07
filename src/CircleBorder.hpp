#ifndef CIRCLEBORDER_HPP
#define CIRCLEBORDER_HPP

#include <SFML/Graphics.hpp>

class CircleBorder {
public:
    CircleBorder(sf::Vector2f position, float radius, sf::Color color);
    
    // Return a reference to the underlying SFML circle shape.
    sf::CircleShape& getObject();
    
private:
    sf::CircleShape object;
};

#endif
