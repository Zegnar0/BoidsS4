#ifndef BOIDS_MANAGER_HPP
#define BOIDS_MANAGER_HPP

#include <glm/glm.hpp>
#include <vector>
#include "boids.hpp"
#include "p6/p6.h"
#include "parameters.hpp"

class BoidsManager {
public:
    explicit BoidsManager(const Parameters& params);
    ~BoidsManager();

    void randomInitBoids();
    void update(p6::Context* ctx, const Parameters& params);

private:
    std::vector<Boids> boids{10};
    Parameters         parameters{};
};

#endif