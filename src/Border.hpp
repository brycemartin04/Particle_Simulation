#ifndef BORDER_HPP
#define BORDER_HPP

#include <SFML/Graphics.hpp>

class Border {
public:
    Border(sf::Vector2f position, float radius, sf::Color color);
    
    // Return a reference to the underlying SFML circle shape.
    sf::CircleShape& getObject();
    
private:
    sf::CircleShape object;
};

#endif // BORDER_HPP
