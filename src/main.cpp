#include <SFML/Graphics.hpp>
#include "Particle.hpp"
#include "Border.hpp"
#include "Solver.hpp"
#include <vector>

int main() {
    const int width = 720;
    const int height = 720;

    sf::RenderWindow window(sf::VideoMode({width, height}), "Particle Simulation with Verlet Integration");
    window.setFramerateLimit(60);

    // Create a series of particles.
    std::vector<Particle> particles;
    for (int i = 300; i < 400; i += 1) {
        particles.emplace_back(sf::Vector2f(static_cast<float>(i), 360.f), 10.f, sf::Vector2f(1.f, 0.f), sf::Color::Red);
    }
   
    for (int i = 300; i < 400; i += 1) {
        particles.emplace_back(sf::Vector2f(static_cast<float>(i), 360.f), 5.f, sf::Vector2f(0.f, 0.f), sf::Color::Green);
    }
    for (int i = 300; i < 400; i += 1) {
        particles.emplace_back(sf::Vector2f(static_cast<float>(i), 360.f), 5.f, sf::Vector2f(0.f, 0.f), sf::Color::Blue);
    }
    for (int i = 300; i < 400; i += 1) {
        particles.emplace_back(sf::Vector2f(static_cast<float>(i), 360.f), 5.f, sf::Vector2f(0.f, 0.f), sf::Color::Yellow);
    }
    for (int i = 300; i < 400; i += 1) {
        particles.emplace_back(sf::Vector2f(static_cast<float>(i), 360.f), 5.f, sf::Vector2f(0.f, 0.f), sf::Color::Cyan);
    }
    for (int i = 300; i < 400; i += 1) {
        particles.emplace_back(sf::Vector2f(static_cast<float>(i), 360.f), 5.f, sf::Vector2f(0.f, 0.f), sf::Color::Magenta);
    }
    for (int i = 300; i < 400; i += 1) {
        particles.emplace_back(sf::Vector2f(static_cast<float>(i), 360.f), 5.f, sf::Vector2f(0.f, 0.f), sf::Color::Magenta);
    }
    for (int i = 300; i < 400; i += 1) {
        particles.emplace_back(sf::Vector2f(static_cast<float>(i), 360.f), 5.f, sf::Vector2f(0.f, 0.f), sf::Color::Magenta);
    }

 

    // Create a circular border.
    Border border(sf::Vector2f(360.f, 360.f), 360.f, sf::Color::Black);
    Solver* solver = new Solver();

    while (window.isOpen()){
        while (const std::optional event = window.pollEvent()){
            if (event->is<sf::Event::Closed>()){
                window.close();
            }
        }
        window.clear(sf::Color::White);

        // Draw the border.
        window.draw(border.getObject());

        // Check collisions between particles and update them.
        // To avoid duplicate checks, use a nested loop.
        for (std::size_t i = 0; i < particles.size(); ++i) {
            for (std::size_t j = i + 1; j < particles.size(); ++j) {
                solver->solveParticleCollision(particles[i], particles[j]);
            }
            // Check collision with the border.
            particles[i].checkCollisions(border);
            // Update the particle’s position via Verlet integration.
            particles[i].updatePosition();
            window.draw(particles[i].getObject());
        }
        window.display();
    }
    
    return 0;
}