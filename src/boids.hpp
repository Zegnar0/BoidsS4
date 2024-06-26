#pragma once
#include <glm/vec2.hpp>
#include <vector>
// #include "boidsModel.hpp"
#include "p6/p6.h"
#include "parameters.hpp"

class Boids {
public:
    Boids();
    ~Boids();

    void randomInitBoids();

    static float     distance(const Boids& boid1, const Boids& boid2);
    static glm::vec3 averageVelocity(const std::vector<glm::vec3>& velocities);
    void             updateBoids(Parameters param, const Boids& other_boid, std::vector<Boids>& nearby_boids_alignment, std::vector<Boids>& nearby_boids_separation, std::vector<Boids>& nearby_boids_cohesion);
    void             drawBoids(const Parameters& param, p6::Context* ctx);
    glm::vec3        getPosition() const;
    // void             drawBoids(const Parameters& param, p6::Context* ctx, const std::vector<BoidsModel>& boidsModels);

private:
    glm::vec3 position;
    glm::vec3 velocity;
};
