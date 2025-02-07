#include "Spawner.hpp"

Spawner::Spawner(sf::Vector2f pos,float r,sf::Vector2f initialVel,float num, int speed = 1){
    position = pos;
    numParticles = num;
    radius = r;
    initialVelocity = initialVel;
    spawnSpeed = speed;

}




void Spawner::update(std::vector<Particle>& particles){
        if (counter < numParticles && clock % spawnSpeed == 0) { 
                float hue = counter * (360.f / numParticles);
                particles.emplace_back(position, radius, initialVelocity, HSVToRGB(hue, 1.f, 1.f));
                counter++;
            }
        clock++;
}

sf::Color Spawner::HSVToRGB(float h, float s, float v){
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