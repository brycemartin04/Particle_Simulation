#include <SFML/Graphics.hpp>
#ifndef BORDER_HPP
#define BORDER_HPP

class Border {
    private:
        sf::CircleShape object;
        
public:
    Border(sf::Vector2f position, float radius,sf::Color color);

    sf::CircleShape& getObject();

};

#endif