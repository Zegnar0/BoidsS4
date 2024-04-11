#include "BoidsManager.hpp"
#include <glm/glm.hpp>
#include "boids.hpp"

// Initialisation
BoidsManager::BoidsManager(const Parameters& params)
    : parameters(params) {}

// Destruction
BoidsManager::~BoidsManager() = default;

// Initialisation des boids
void BoidsManager::randomInitBoids()
{
    for (auto& boid : boids)
    {
        boid.randomInitBoids();
    }
}

// Update
void BoidsManager::update(p6::Context* ctx, const Parameters& params)
{
    for (auto& boid : boids)
    {
        boid.drawBoids(params, ctx);

        // Création des vecteurs des boids voisins
        std::vector<Boids> nearby_boids_alignment;
        std::vector<Boids> nearby_boids_separation;
        std::vector<Boids> nearby_boids_cohesion;

        for (const auto& other_boid : boids)
        {
            boid.updateBoids(params, other_boid, nearby_boids_alignment, nearby_boids_separation, nearby_boids_alignment);
        }
    }
}
