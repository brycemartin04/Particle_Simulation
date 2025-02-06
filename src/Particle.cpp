#include "Particle.hpp"
#include <cmath>

// Time step for the simulation.
const float TIME_STEP = .1f;

Particle::Particle(sf::Vector2f position, float radius, sf::Vector2f initialVelocity, sf::Color color) {
    velocity = initialVelocity;
    object.setRadius(radius);
    // Set the origin to the center of the circle.
    object.setOrigin({radius, radius});
    object.setPosition(position);
    object.setFillColor(color);
}

void Particle::applyGravity() {
    // Gravity acceleration (scaled by TIME_STEP).
    velocity.y += 9.91f * TIME_STEP;
}

void Particle::updatePosition() {
    applyGravity();
    object.move(velocity * TIME_STEP);
}

void Particle::checkCollisions(Particle& other) {
    sf::Vector2f pos1 = object.getPosition();
    sf::Vector2f pos2 = other.getObject().getPosition();
    float r1 = object.getRadius();
    float r2 = other.getObject().getRadius();

    sf::Vector2f diff = pos1 - pos2;
    float dist = std::sqrt(diff.x * diff.x + diff.y * diff.y);

    // If the distance is less than the sum of radii, a collision has occurred.
    if (dist < (r1 + r2)) {
        // Avoid division by zero.
        sf::Vector2f normal = (dist != 0.f) ? diff / dist : sf::Vector2f(1.f, 0.f);

        // --- Positional Correction ---
        float penetration = (r1 + r2) - dist;
        // Move each particle half the penetration distance along the normal.
        object.move(normal * (penetration / 2.f));
        other.getObject().move(-normal * (penetration / 2.f));

        // --- Velocity Correction ---
        sf::Vector2f relativeVelocity = velocity - other.getVelocity();
        float velocityAlongNormal = relativeVelocity.x * normal.x + relativeVelocity.y * normal.y;

        // Only resolve if particles are moving toward each other.
        if (velocityAlongNormal < 0.f) {
            float restitution = 0.5f; // Change this for more/less bounciness.
            // For equal masses, impulse scalar is computed as follows:
            float impulse = -(1.f + restitution) * velocityAlongNormal / 2.f;
            sf::Vector2f impulseVec = impulse * normal;
            // Update velocities (assuming equal masses).
            velocity += impulseVec;
            // Direct access to other’s velocity (since it’s a friend in our simple design).
            other.velocity -= impulseVec;
        }
    }
}

void Particle::checkCollisions(Border& border) {
    sf::Vector2f pos = object.getPosition();
    sf::Vector2f borderPos = border.getObject().getPosition();
    float particleRadius = object.getRadius();
    float borderRadius = border.getObject().getRadius();

    sf::Vector2f diff = pos - borderPos;
    float dist = std::sqrt(diff.x * diff.x + diff.y * diff.y);

    // Collision with a circular border: if the particle lies outside the border.
    if (dist + particleRadius > borderRadius) {
        sf::Vector2f normal = (dist != 0.f) ? diff / dist : sf::Vector2f(1.f, 0.f);
        float penetration = (dist + particleRadius) - borderRadius;
        // Move the particle back inside the border.
        object.move(-normal * penetration);

        // Reflect the velocity if the particle is moving outward.
        float velocityAlongNormal = velocity.x * normal.x + velocity.y * normal.y;
        if (velocityAlongNormal > 0.f) {
            float restitution = 0.5f;
            velocity -= (1.f + restitution) * velocityAlongNormal * normal;
        }
    }
}

sf::Vector2f Particle::getVelocity(){
    return velocity;
}

sf::CircleShape& Particle::getObject() {
    return object;
}
