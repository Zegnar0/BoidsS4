#ifndef BOIDS_MANAGER_HPP
#define BOIDS_MANAGER_HPP

#include <glm/glm.hpp>
#include <vector>
#include "boids.hpp"
#include "boidsMarkovState.hpp"
#include "p6/p6.h"
#include "parameters.hpp"

class BoidsManager {
public:
    explicit BoidsManager(const Parameters& params);
    ~BoidsManager();

    void randomInitBoids();
    void update(p6::Context* ctx, Parameters& params);

private:
    std::vector<Boids> boids{10};
    // std::vector<BoidsModel> boidsModel;
    Parameters       parameters{};
    BoidsMarkovState markovState{};
    int              boucleMarkov = 0;
};

#endif