#include "Border.hpp"
#include <SFML/Graphics.hpp>

const float TIME_STEP = 1.0f;  // Simulation time step

Border::Border(sf::Vector2f position, float radius, sf::Color color){
    object.setRadius(radius);
    object.setOrigin(object.getGeometricCenter());
    object.setPosition(position); 
    object.setPointCount(500);
    object.setFillColor(color);  //Change to use sprite later  
}

sf::CircleShape& Border::getObject(){
    return object;
}