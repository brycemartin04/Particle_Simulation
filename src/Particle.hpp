#ifndef PARTICLE_HPP
#define PARTICLE_HPP

#include <SFML/Graphics.hpp>
#include "Border.hpp"

class Particle {
public:
    // The initialVelocity is used to set the previous position so that:
    //   velocity = currentPos - previousPos
    Particle(sf::Vector2f position, float radius, sf::Vector2f initialVelocity, sf::Color color);
    
    // Update the particle’s position using Verlet integration.
    void updatePosition();
    
    // Resolve collisions with another particle.
    void checkCollisions(Particle& other);
    
    // Resolve collisions with a circular border.
    void checkCollisions(Border& border);

    // Compute the implicit velocity (current - previous).
    sf::Vector2f getVelocity() const;
    
    // Return a reference to the underlying SFML circle shape.
    sf::CircleShape& getObject();
    
    // Expose currentPos for internal use (could be made friend or use getters).
    sf::Vector2f currentPos;

private:
    // For Verlet, store the previous position.
    sf::Vector2f previousPos;
    
    // The SFML circle shape for drawing.
    sf::CircleShape object;
};

#endif // PARTICLE_HPP
