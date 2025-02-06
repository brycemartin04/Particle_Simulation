#include "Particle.hpp"
#include <SFML/Graphics.hpp>

const float TIME_STEP = .01f;  // Simulation time step

Particle::Particle(sf::Vector2f position, float radius, sf::Vector2f initialVelocity, sf::Color color){
    velocity = initialVelocity;
    object.setRadius(radius);
    object.setOrigin(object.getGeometricCenter());
    object.setPosition(position); 
    
    object.setFillColor(color);  //Change to use sprite later  
}

void Particle::applyGravity(){
    velocity.y += 9.91f;
}

void Particle::updatePosition(){
    applyGravity();
    object.move(velocity * TIME_STEP);
}
 
void Particle::checkCollisions(Particle& other){
    if (object.getGlobalBounds().findIntersection(other.getObject().getGlobalBounds())) {
        sf::Vector2f normal = object.getPosition() - other.getObject().getPosition();
        float length = sqrt(normal.x * normal.x + normal.y * normal.y);
        if (length != 0) {
            normal /= length;
            
            sf::Vector2f relativeVelocity = velocity - other.getVelocity();
            float velocityAlongNormal = relativeVelocity.x * normal.x + relativeVelocity.y * normal.y;

            if (velocityAlongNormal < 0) {
                float restitution = 0.5f;
                float impulse = (2 * velocityAlongNormal) / (1 + 1);

                velocity -= normal * impulse;
                other.velocity += normal * impulse;
            }
        }
    }
}

void Particle::checkCollisions(Border& border) {
    sf::Vector2f borderCenter = border.getObject().getPosition();
    float borderRadius = border.getObject().getRadius();
    sf::Vector2f particlePosition = object.getPosition();
    float particleRadius = object.getRadius();

    sf::Vector2f direction = borderCenter - particlePosition;
    float distance = sqrt(direction.x * direction.x + direction.y * direction.y);

    if (distance > borderRadius - particleRadius) {
        sf::Vector2f normal = direction / distance;

        sf::Vector2f vel = velocity;
        float velocityAlongNormal = vel.x * normal.x + vel.y * normal.y;

        if (velocityAlongNormal < 0) {
            float restitution = 0.5f;
            velocity = vel - (1.0f + restitution) * velocityAlongNormal * normal;
        }
    }
}



sf::Vector2f Particle::getVelocity(){ 
    return velocity; 
    }

sf::CircleShape& Particle::getObject(){
    return object;
}