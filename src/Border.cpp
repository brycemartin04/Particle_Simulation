#include "Border.hpp"

Border::Border(sf::Vector2f position, float radius, sf::Color color) {
    object.setRadius(radius);
    // Center the circle shape.
    object.setOrigin({radius, radius});
    object.setPosition(position);
    // Increase point count for a smooth circle.
    object.setPointCount(500);
    object.setFillColor(color);
}

sf::CircleShape& Border::getObject() {
    return object;
}
