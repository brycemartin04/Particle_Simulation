#include <SFML/Graphics.hpp>
#include "Border.hpp"
#include <vector>

class Particle {
    private:
        sf::CircleShape object;
        sf::Vector2f velocity;
        void applyGravity();
        
public:
    Particle(sf::Vector2f position, float radius, sf::Vector2f velocity, sf::Color color);

    void updatePosition();
    
    sf::Vector2f getVelocity();
    sf::CircleShape& getObject();

    void checkCollisions(Particle& other);
    void checkCollisions(Border& other);
};