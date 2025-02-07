#ifndef RECTBORDER_HPP
#define RECTBORDER_HPP

#include <SFML/Graphics.hpp>

class RectBorder {
public:
    RectBorder(sf::Vector2f position,sf::Vector2f size, sf::Color color);
    
    sf::RectangleShape& getObject();
    
private:
    sf::RectangleShape object;
};

#endif
