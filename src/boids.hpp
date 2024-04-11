#ifndef BOIDS_HPP
#define BOIDS_HPP
#include <glm/vec2.hpp>
#include <vector>
#include "p6/p6.h"
#include "parameters.hpp"

class Boids {
public:
    Boids();
    ~Boids();

    void randomInitBoids();

    static float     distance(const Boids& boid1, const Boids& boid2);
    static glm::vec2 averageVelocity(const std::vector<glm::vec2>& velocities);
    void             updateBoids(Parameters param, const Boids& other_boid, std::vector<Boids>& nearby_boids_alignment, std::vector<Boids>& nearby_boids_separation, std::vector<Boids>& nearby_boids_cohesion);
    void             drawBoids(Parameters param, p6::Context* ctx);

private:
    glm::vec2 position;
    glm::vec2 velocity;
};

#endif // BOIDS_HPP