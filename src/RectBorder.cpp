#include "RectBorder.hpp"

RectBorder::RectBorder(sf::Vector2f position,sf::Vector2f size, sf::Color color) {
    object.setSize(size);
    object.setOrigin(object.getGeometricCenter());
    object.setPosition(position);
    object.setFillColor(color);
}

sf::RectangleShape& RectBorder::getObject() {
    return object;
}
