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

    sf::RenderWindow statsWindow(sf::VideoMode({300, 100}), "Stats");
    
    // Load font for displaying text
    const sf::Font font("../Montserrat-BoldItalic.otf");

    sf::Clock clock;
    float fps = 0.f;

    sf::Text fpsText(font);
    fpsText.setCharacterSize(20);
    fpsText.setFillColor(sf::Color::Black);
    fpsText.setPosition({10, 10});

    sf::Text particleCountText(font);
    particleCountText.setCharacterSize(20);
    particleCountText.setFillColor(sf::Color::Black);
    particleCountText.setPosition({10, 40});

    std::vector<Particle> particles;
    
    CircleBorder cBorder(sf::Vector2f(width/2, height/2), 360.f, sf::Color::Black);
    RectBorder rBorder(sf::Vector2f(width/2, height/2),sf::Vector2f(width - 20, height - 20), sf::Color::Black);
    Solver* solver = new Solver();
    Spawner spawner({width/4,height/3},10.f, sf::Vector2f(100.f, 0.f),1000,2);
    //Spawner spawner2({width/3,height/2},10.f, sf::Vector2f(0.1f, 20.f),500,10);
    int temp = 0;
    sf::Vector2f mousePos;

    

    while (window.isOpen()){
        sf::Time elapsed = clock.restart();
        fps = 1.f / elapsed.asSeconds();


        while (const std::optional event = window.pollEvent()){
            if (event->is<sf::Event::Closed>()){
                window.close();
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Escape)){
                window.close();
            }
        }
        
        if (window.hasFocus()){
            if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
                mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                    sf::Vector2i pos = sf::Mouse::getPosition(window);
                for (auto& particle : particles) {
                    sf::Vector2f direction = mousePos - particle.currentPosition;
                    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);
                    
                    if (distance > 1.f) { // Avoid division by zero or extreme acceleration
                        direction /= distance; // Normalize direction
                        float attractionForce = 0.2f; // Strength of attraction
                        particle.currentPosition += direction * attractionForce;
                    }
                }
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
            particles[i].updatePosition();
            window.draw(particles[i].getObject());
            
        }
        
        }

       

        window.display();

        fpsText.setString("FPS: " + std::to_string(static_cast<int>(fps)));
        particleCountText.setString("Particles: " + std::to_string(particles.size()));

        statsWindow.clear(sf::Color::White);
        statsWindow.draw(fpsText);
        statsWindow.draw(particleCountText);
        statsWindow.display();
    }
    
    return 0;
}