#include <SFML/Graphics.hpp>
#include "Particle.hpp"
#include "CircleBorder.hpp"
#include "RectBorder.hpp"
#include "Solver.hpp"
#include "Spawner.hpp"
#include <vector>


int main() {
    const int width = 1280;
    const int height = 720;

    sf::RenderWindow window(sf::VideoMode({width, height}), "Particle Simulation");
    window.setFramerateLimit(60);

    std::vector<Particle> particles;
    
    CircleBorder cBorder(sf::Vector2f(width/2, height/2), 360.f, sf::Color::Black);
    RectBorder rBorder(sf::Vector2f(width/2, height/2),sf::Vector2f(width - 20, height - 20), sf::Color::Black);
    Solver* solver = new Solver();
    Spawner spawner({width/4,height/3},5.f, sf::Vector2f(100.f, 0.f),1500,2);
    //Spawner spawner2({width/3,height/2},10.f, sf::Vector2f(30.f, -50.f),500,10);
    int temp = 0;

    while (window.isOpen()){
        while (const std::optional event = window.pollEvent()){
            if (event->is<sf::Event::Closed>()){
                window.close();
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Escape)){
                window.close();
            }
        }

        spawner.update(particles);
        //spawner2.update(particles);
        window.clear(sf::Color::White);

        // Draw the border.
        window.draw(rBorder.getObject());

        // Check collisions between particles and update them.
        // To avoid duplicate checks, use a nested loop.
        for (int i = 0; i < 2; i++){
        for (std::size_t i = 0; i < particles.size(); ++i) {
            for (std::size_t j = i + 1; j < particles.size(); ++j) {
                solver->solveParticleCollision(particles[i], particles[j]);
            }
            // Check collision with the border.
            solver->solveBorderCollision(particles[i], rBorder);
            // Update the particle’s position via Verlet integration.
            
        }
        
        }

        for (auto& particle : particles) {
            particle.updatePosition();
            window.draw(particle.getObject());
        }

        window.display();
    }
    
    return 0;
}