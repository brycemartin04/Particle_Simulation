#ifndef SOLVER_HPP
#define SOLVER_HPP

#include <SFML/Graphics.hpp>
#include "CircleBorder.hpp"
#include "RectBorder.hpp"
#include "Particle.hpp"

class Solver {
public:
    void solveParticleCollision(Particle& p1, Particle& p2);
    void solveBorderCollision(Particle& p, CircleBorder& b);
    void solveBorderCollision(Particle& p, RectBorder& b);

private:
};

#endif
