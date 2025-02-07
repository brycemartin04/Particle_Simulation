#include "Solver.hpp"

void Solver::solveParticleCollision(Particle& p1, Particle& p2) {
    sf::Vector2f pos1 = p1.currentPosition;
    sf::Vector2f pos2 = p2.currentPosition;
    float r1 = p1.getObject().getRadius();
    float r2 = p2.getObject().getRadius();
    
    
    sf::Vector2f diff = pos1 - pos2;
    float dist = std::sqrt(diff.x * diff.x + diff.y * diff.y);
    
    // Check if particles overlap.
    if (dist < (r1 + r2) && dist > 0.f) {
        // Calculate penetration depth.
        float penetration = (r1 + r2) - dist;
        // Compute the collision normal.
        sf::Vector2f normal = diff / dist;
        
        // --- Positional Correction ---
        // Move each particle by half the penetration along the normal.
        p1.currentPosition += normal * (penetration / 2.f);
        p2.currentPosition -= normal * (penetration / 2.f);
        
        float damping = .99f; // 1% energy loss on collision

        // For Particle 1:
        sf::Vector2f v1 = p1.currentPosition - p1.previousPosition;
        p1.previousPosition = p1.currentPosition - v1 * damping;

        // For Particle 2:
        sf::Vector2f v2 = p2.currentPosition - p2.previousPosition;
        p2.previousPosition = p2.currentPosition - v2 * damping;

        // --- Velocity (Bounce) Correction ---
        // In Verlet, the implicit velocity is (currentPosition - previousPosition).
        v1 = p1.currentPosition - p1.previousPosition;
         v2 = p2.currentPosition - p2.previousPosition;
        float v1n = v1.x * normal.x + v1.y * normal.y;
        float v2n = v2.x * normal.x + v2.y * normal.y;
        
        
        // Compute the impulse.
        float impulse = 0.1f * (v1n - v2n) / (1.f / 1.f + 1.f / 1.f);
        
        // Apply the impulse to the particles.
        if (impulse < .5f) {
      
        p1.currentPosition += normal * impulse / 1.f;   
        p2.currentPosition -= normal * impulse / 1.f;
        }
    }
}

void Solver::solveBorderCollision(Particle& p, CircleBorder& b) {
    sf::Vector2f pos = p.currentPosition;
    sf::Vector2f borderPos = b.getObject().getPosition();
    float particleRadius = p.getObject().getRadius();
    float borderRadius = b.getObject().getRadius();
    
    sf::Vector2f diff = pos - borderPos;
    float dist = std::sqrt(diff.x * diff.x + diff.y * diff.y);
    
    // If the particle is outside (or overlapping) the border.
    if (dist + particleRadius > borderRadius && dist > 0.f) {
        float penetration = (dist + particleRadius) - borderRadius;
        sf::Vector2f normal = diff / dist;
        
        // Push the particle back inside.
        p.currentPosition -= normal * penetration;
        
        // Reflect the velocity.
        sf::Vector2f v = p.currentPosition - p.previousPosition;
        float vn = v.x * normal.x + v.y * normal.y;
        if (vn > 0.f) {
            float restitution = 0.9f;
            sf::Vector2f v_n = normal * vn;
            sf::Vector2f v_t = v - v_n;
            v = v_t - restitution * v_n;
            p.previousPosition = p.currentPosition - v;

            float damping = 1.f; // 1% energy loss on collision

            // For Particle 1:
            sf::Vector2f v1 = p.currentPosition - p.previousPosition;
            p.previousPosition = p.currentPosition - v1 * damping;
        }
    }
}

void Solver::solveBorderCollision(Particle& p, RectBorder& b) {
    sf::Vector2f pos = p.currentPosition;
    sf::Vector2f prevPos = p.previousPosition;
    float radius = p.getObject().getRadius();

    // Get rectangle position and size directly (SFML 3 changes)
    sf::Vector2f rectPos = b.getObject().getPosition();
    sf::Vector2f rectSize = b.getObject().getSize();

    float left = rectPos.x - rectSize.x / 2;
    float right = rectPos.x + rectSize.x / 2;
    float top = rectPos.y - rectSize.y / 2;
    float bottom = rectPos.y + rectSize.y / 2;

    // Compute velocity
    sf::Vector2f velocity = pos - prevPos;

    float damping = 0.5f;

    // Check collision with left and right edges
    if (pos.x - radius < left) {
        pos.x = left + radius; // Move particle inside the boundary
        velocity.x = -velocity.x * damping; // Reflect and apply some damping
    } else if (pos.x + radius > right) {
        pos.x = right - radius;
        velocity.x = -velocity.x * damping;
    }

    // Check collision with top and bottom edges
    if (pos.y - radius < top) {
        pos.y = top + radius;
        velocity.y = -velocity.y * damping;
    } else if (pos.y + radius > bottom) {
        pos.y = bottom - radius;
        velocity.y = -velocity.y * damping;
    }

    // Update positions
    p.currentPosition = pos;
    p.previousPosition = pos - velocity;
}