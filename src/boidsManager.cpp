#include "boidsManager.hpp"
#include <glm/glm.hpp>
#include <iostream>
#include "boids.hpp"
#include "tiny_obj_loader.h"

// Initialisation
BoidsManager::BoidsManager(const Parameters& params)
    : parameters(params), markovState() {}

// Destruction
BoidsManager::~BoidsManager() = default;

// Initialisation des boids
void BoidsManager::randomInitBoids()
{
    markovState.randomInitBoids();
    for (auto& boid : boids)
    {
        boid.randomInitBoids();
    }
}

// Update
void BoidsManager::update(p6::Context* ctx, Parameters& params)
{
    if (boucleMarkov == 100)
    {
        BoidState newState = markovState.updateState();
        markovState.updateParams(params, newState);
        boucleMarkov = 0;
    }
    for (auto& boid : boids)
    {
        boid.drawBoids(params, ctx);

        std::vector<Boids> nearby_boids_alignment;
        std::vector<Boids> nearby_boids_separation;
        std::vector<Boids> nearby_boids_cohesion;

        for (const auto& other_boid : boids)
        {
            boid.updateBoids(params, other_boid, nearby_boids_alignment, nearby_boids_separation, nearby_boids_cohesion);
        }
    }
    boucleMarkov++;
}

const std::vector<Boids>& BoidsManager::getBoids() const
{
    return boids;
}