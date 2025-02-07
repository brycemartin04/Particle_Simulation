#include "Particle.hpp"
#include <cmath>

// Time step for the simulation.
const float deltaTime = 0.1f;

Particle::Particle(sf::Vector2f position, float radius, sf::Vector2f initialVelocity, sf::Color color) {
    // For Verlet, the current position is the given position.
    currentPosition = position;
    // Set the previous position so that the initial velocity is:
    //   velocity = currentPosition - previousPosition   =>   previousPosition = currentPosition - velocity * dt
    previousPosition = position - initialVelocity * deltaTime;
    
    object.setRadius(radius);
    object.setOrigin({radius, radius});
    object.setPosition(position);
    object.setFillColor(color);
}

void Particle::updatePosition() {
    // Gravity acceleration (pixels/s²)
    const float GRAVITY = 9.81f;
    sf::Vector2f acceleration(0.f, GRAVITY);
    
    // Verlet integration: newPos = currentPosition + (currentPosition - previousPosition) + acceleration * dt^2
    sf::Vector2f temp = currentPosition;
    currentPosition = currentPosition + (currentPosition - previousPosition) + acceleration * (deltaTime * deltaTime);
    previousPosition = temp;
    
    // Update the drawn position.
    object.setPosition(currentPosition);
    
    sf::Vector2f v = currentPosition - previousPosition;
    float speedSquared = v.x * v.x + v.y * v.y;
    const float sleepThresholdSq = 0.001f; // Adjust as needed

    if (speedSquared < sleepThresholdSq) {
        // Particle is essentially at rest, so "sleep" it.
        previousPosition = currentPosition;
}
}

sf::Vector2f Particle::getVelocity() const {
    return currentPosition - previousPosition;
}

sf::CircleShape& Particle::getObject() {
    return object;
}
