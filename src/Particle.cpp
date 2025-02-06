#include "Particle.hpp"
#include <cmath>

// Time step for the simulation.
const float TIME_STEP = 0.1f;

Particle::Particle(sf::Vector2f position, float radius, sf::Vector2f initialVelocity, sf::Color color) {
    // For Verlet, the current position is the given position.
    currentPos = position;
    // Set the previous position so that the initial velocity is:
    //   velocity = currentPos - previousPos   =>   previousPos = currentPos - velocity * dt
    previousPos = position - initialVelocity * TIME_STEP;
    
    object.setRadius(radius);
    // Center the circle shape.
    object.setOrigin({radius, radius});
    object.setPosition(position);
    object.setFillColor(color);
}

void Particle::updatePosition() {
    // Gravity acceleration (pixels/s²)
    sf::Vector2f acceleration(0.f, 9.91f);
    
    // Verlet integration: newPos = currentPos + (currentPos - previousPos) + acceleration * dt^2
    sf::Vector2f temp = currentPos;
    currentPos = currentPos + (currentPos - previousPos) + acceleration * (TIME_STEP * TIME_STEP);
    previousPos = temp;
    
    // Update the drawn position.
    object.setPosition(currentPos);
}

void Particle::checkCollisions(Particle& other) {
    sf::Vector2f pos1 = currentPos;
    sf::Vector2f pos2 = other.currentPos;
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
        currentPos += normal * (penetration / 2.f);
        other.currentPos -= normal * (penetration / 2.f);
        
        // --- Velocity (Bounce) Correction ---
        // In Verlet, the implicit velocity is (currentPos - previousPos).
        sf::Vector2f v1 = currentPos - previousPos;
        sf::Vector2f v2 = other.currentPos - other.previousPos;
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
            // Update previousPos accordingly.
            previousPos = currentPos - v1;
        }
        if (v2n > 0.f) { // Note: for the other particle, the normal points from it toward the first particle.
            sf::Vector2f v2nVec = normal * v2n;
            sf::Vector2f v2t = v2 - v2nVec;
            v2nVec = -restitution * v2nVec;
            v2 = v2t + v2nVec;
            other.previousPos = other.currentPos - v2;
        }
    }
}

void Particle::checkCollisions(Border& border) {
    sf::Vector2f pos = currentPos;
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
        currentPos -= normal * penetration;
        
        // Reflect the velocity.
        sf::Vector2f v = currentPos - previousPos;
        float vn = v.x * normal.x + v.y * normal.y;
        if (vn > 0.f) {
            float restitution = 0.5f;
            sf::Vector2f v_n = normal * vn;
            sf::Vector2f v_t = v - v_n;
            v = v_t - restitution * v_n;
            previousPos = currentPos - v;
        }
    }
}

sf::Vector2f Particle::getVelocity() const {
    return currentPos - previousPos;
}

sf::CircleShape& Particle::getObject() {
    return object;
}
