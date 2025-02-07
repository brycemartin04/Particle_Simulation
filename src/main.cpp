#include <SFML/Graphics.hpp>
#include "Particle.hpp"
#include "Border.hpp"
#include "Solver.hpp"
#include <vector>

sf::Color HSVToRGB(float h, float s, float v){
    float hPrime = h/60.0f;
    unsigned int hIndex = (unsigned int)hPrime % 6;
    float chroma = s * v;
    float min = v - chroma;
    float x = chroma * (1.f - std::abs(fmod(hPrime, 2.f) - 1.f));
    float outRGB[6][3] = {
        {chroma, x, 0.f},
        {x, chroma, 0.f},
        {0.f, chroma, x},
        {0.f, x, chroma},
        {x, 0.f, chroma},
        {chroma, 0.f, x}
    };
    float rF = (outRGB[hIndex][0] + min);
    float gF = (outRGB[hIndex][1] + min);
    float bF = (outRGB[hIndex][2] + min);
    rF *= 255;
    gF *= 255;
    bF *= 255;
    std::uint8_t rI = std::uint8_t(rF);
    std::uint8_t gI = std::uint8_t(gF);
    std::uint8_t bI = std::uint8_t(bF);
    return sf::Color(rI, gI, bI);
}

int main() {
    const int width = 1280;
    const int height = 720;

    sf::RenderWindow window(sf::VideoMode({width, height}), "Particle Simulation with Verlet Integration");
    window.setFramerateLimit(60);

    // Create a series of particles.
    std::vector<Particle> particles;
    /*
    for (int i = 300; i < 400; i += 1) {
        particles.emplace_back(sf::Vector2f(static_cast<float>(i), 360.f), 15.f, sf::Vector2f(1.f, 0.f), sf::Color::Red);
    }
    for (int i = 300; i < 400; i += 1) {
        particles.emplace_back(sf::Vector2f(static_cast<float>(i), 360.f), 15.f, sf::Vector2f(0.f, 0.f), sf::Color::Green);
    }
    for (int i = 300; i < 400; i += 1) {
        particles.emplace_back(sf::Vector2f(static_cast<float>(i), 360.f), 5.f, sf::Vector2f(0.f, 0.f), sf::Color::Blue);
    }
    for (int i = 300; i < 400; i += 1) {
        particles.emplace_back(sf::Vector2f(static_cast<float>(i), 360.f),15.f, sf::Vector2f(0.f, 0.f), sf::Color::Yellow);
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
    */
    
 

    // Create a circular border.
    Border border(sf::Vector2f(width/2, height/2), 360.f, sf::Color::Black);
    Solver* solver = new Solver();
    int temp = 0;

    while (window.isOpen()){
        if (temp < 500){
            float hue = temp * (360.f / 500);
            particles.emplace_back(sf::Vector2f(width/2, height/5), 10.f, sf::Vector2f(1.f, 0.f), HSVToRGB(hue, 1.f, 1.f));
        }
        temp ++;
        while (const std::optional event = window.pollEvent()){
            if (event->is<sf::Event::Closed>()){
                window.close();
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Escape)){
                window.close();
            }
        }
        window.clear(sf::Color::White);

        // Draw the border.
        window.draw(border.getObject());

        // Check collisions between particles and update them.
        // To avoid duplicate checks, use a nested loop.
        for (int i = 0; i < 2; i++){
        for (std::size_t i = 0; i < particles.size(); ++i) {
            for (std::size_t j = i + 1; j < particles.size(); ++j) {
                solver->solveParticleCollision(particles[i], particles[j]);
            }
            // Check collision with the border.
            solver->solveBorderCollision(particles[i], border);
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