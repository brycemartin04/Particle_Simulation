#ifndef SPAWNER_HPP
#define SPAWNER_HPP

#include <SFML/Graphics.hpp>
#include "Particle.hpp"

class Spawner {
public:
    Spawner(sf::Vector2f pos,float r,sf::Vector2f initialVel,float num, int spawnSpeed);
    void update(std::vector<Particle>& particles);

private:
    int spawnSpeed;
    sf::Vector2f position;
    float numParticles;
    float radius;
    sf::Vector2f initialVelocity;
    int counter = 0;
    int clock = 0;
    sf::Color HSVToRGB(float h, float s, float v);
};

#endif
