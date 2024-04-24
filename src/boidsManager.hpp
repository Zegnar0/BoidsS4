#pragma once
#include <glm/glm.hpp>
#include <vector>
#include "BoidsMarkovState.hpp"
#include "boids.hpp"
#include "p6/p6.h"
#include "parameters.hpp"

class BoidsManager {
public:
    explicit BoidsManager(const Parameters& params);
    ~BoidsManager();

    void                      randomInitBoids();
    void update(p6::Context* ctx, Parameters& params);
    void                      loadBoidsModel();
    const std::vector<Boids>& getBoids() const;

private:
    std::vector<Boids> boids{10};
    // std::vector<BoidsModel> boidsModel;
    Parameters       parameters{};
    BoidsMarkovState markovState{};
    int              boucleMarkov = 0;
};
