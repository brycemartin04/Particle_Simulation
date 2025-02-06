#include <SFML/Graphics.hpp>
#include "Particle.hpp"
#include "Border.hpp"
#include <vector>

int main() {
    const int width = 720;
    const int height = 720;


    sf::RenderWindow* window = new sf::RenderWindow(sf::VideoMode({width,height}),"Planet Simulation");
    window -> setFramerateLimit(60);

    std::vector<Particle> particles;

    for (int i = 100; i < 600; i+=3){
        particles.push_back(Particle({(float)i,360},3,{0,0},sf::Color::Red));
    }
    Border* border = new Border({360,360},200,sf::Color::Black);

    while (window -> isOpen()){
        while (const std::optional event = window -> pollEvent()){
            if (event->is<sf::Event::Closed>()){
                window->close();
            }
        }


        window -> clear(sf::Color::White);
        
        window -> draw(border->getObject());

        for (Particle& i : particles){
            for (Particle& j : particles){
              if (&i != &j) {
                    i.checkCollisions(j);
                }
            }
            i.checkCollisions(*border);
            i.updatePosition(); 
            window -> draw(i.getObject());
        }

        window -> display();
    }
    
    
    return 0;
}