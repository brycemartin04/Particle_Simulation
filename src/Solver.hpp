#ifndef SOLVER_HPP
#define SOLVER_HPP

#include <SFML/Graphics.hpp>
#include "Border.hpp"
#include "Particle.hpp"

class Solver {
public:
    void solveParticleCollision(Particle& p1, Particle& p2);

    void solveBorderCollision(Particle& p, Border& b);

private:
    
};

#endif
