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
}

void Particle::checkCollisions(Particle& other) {
    sf::Vector2f pos1 = currentPosition;
    sf::Vector2f pos2 = other.currentPosition;
    float r1 = object.getRadius();
    float r2 = other.object.getRadius();
    
    sf::Vector2f diff = pos1 - pos2;
    float dist = std::sqrt(diff.x * diff.x + diff.y * diff.y);
    
    // Check if particles overlap.
    if (dist < (r1 + r2) && dist > 0.f) {
        // Calculate penetration depth.
        float penetration = (r1 + r2) - dist;
        // Compute the collision normal.
        sf::Vector2f normal = diff / dist;
        
        // --- Positional Correction ---
        // Move each particle by half the penetration along the normal.
        currentPosition += normal * (penetration / 2.f);
        other.currentPosition -= normal * (penetration / 2.f);
        
        // --- Velocity (Bounce) Correction ---
        // In Verlet, the implicit velocity is (currentPosition - previousPosition).
        sf::Vector2f v1 = currentPosition - previousPosition;
        sf::Vector2f v2 = other.currentPosition - other.previousPosition;
        float v1n = v1.x * normal.x + v1.y * normal.y;
        float v2n = v2.x * normal.x + v2.y * normal.y;
        float restitution = 0.5f;
        
        // Reflect velocity if moving toward each other.
        if (v1n < 0.f) {
            // Decompose v1 into normal and tangent components.
            sf::Vector2f v1nVec = normal * v1n;
            sf::Vector2f v1t = v1 - v1nVec;
            // Reflect the normal component.
            v1nVec = -restitution * v1nVec;
            v1 = v1t + v1nVec;
            // Update previousPosition accordingly.
            previousPosition = currentPosition - v1;
        }
        if (v2n > 0.f) { // Note: for the other particle, the normal points from it toward the first particle.
            sf::Vector2f v2nVec = normal * v2n;
            sf::Vector2f v2t = v2 - v2nVec;
            v2nVec = -restitution * v2nVec;
            v2 = v2t + v2nVec;
            other.previousPosition = other.currentPosition - v2;
        }
    }
}

void Particle::checkCollisions(Border& border) {
    sf::Vector2f pos = currentPosition;
    sf::Vector2f borderPos = border.getObject().getPosition();
    float particleRadius = object.getRadius();
    float borderRadius = border.getObject().getRadius();
    
    sf::Vector2f diff = pos - borderPos;
    float dist = std::sqrt(diff.x * diff.x + diff.y * diff.y);
    
    // If the particle is outside (or overlapping) the border.
    if (dist + particleRadius > borderRadius && dist > 0.f) {
        float penetration = (dist + particleRadius) - borderRadius;
        sf::Vector2f normal = diff / dist;
        
        // Push the particle back inside.
        currentPosition -= normal * penetration;
        
        // Reflect the velocity.
        sf::Vector2f v = currentPosition - previousPosition;
        float vn = v.x * normal.x + v.y * normal.y;
        if (vn > 0.f) {
            float restitution = 0.5f;
            sf::Vector2f v_n = normal * vn;
            sf::Vector2f v_t = v - v_n;
            v = v_t - restitution * v_n;
            previousPosition = currentPosition - v;
        }
    }
}

sf::Vector2f Particle::getVelocity() const {
    return currentPosition - previousPosition;
}

sf::CircleShape& Particle::getObject() {
    return object;
}
