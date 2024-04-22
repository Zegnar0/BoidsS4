#include "boids.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../src/src-common/glimac/sphere_vertices.hpp"
#include "p6/p6.h"

// Initialisation
Boids::Boids()
{}

// Destruction
Boids::~Boids() = default;

// Initialisation des boids
void Boids::randomInitBoids()
{
    position = {p6::random::number(-2.f, 2.f), p6::random::number(-2.f, 2.f), p6::random::number(-2.f, 2.f)};
    velocity = {p6::random::number(-2.f, 2.f), p6::random::number(-2.f, 2.f), p6::random::number(-2.f, 2.f)};
}

// Distance entre deux boids
float Boids::distance(const Boids& boid1, const Boids& boid2)
{
    return glm::length(boid2.position - boid1.position);
}

// Moyenne des vecteurs de vitesse
glm::vec3 Boids::averageVelocity(const std::vector<glm::vec3>& velocities)
{
    glm::vec3 sum(0.f);
    for (const auto& velocity : velocities)
    {
        sum += velocity;
    }
    sum /= velocities.size();
    return sum;
}

// Update
void Boids::updateBoids(Parameters param, const Boids& other_boid, std::vector<Boids>& nearby_boids_alignment, std::vector<Boids>& nearby_boids_separation, std::vector<Boids>& nearby_boids_cohesion)
{
    if (&other_boid != this)
    {
        float dist = distance(*this, other_boid);
        // Alignment
        if (dist < param.alignmentRadius)
        {
            nearby_boids_alignment.push_back(other_boid);
        }
        // Separation
        if (dist < param.separationRadius)
        {
            nearby_boids_separation.push_back(other_boid);
        }
        // Cohesion
        if (dist < param.cohesionRadius)
        {
            nearby_boids_cohesion.push_back(other_boid);
        }
    }

    // Règle d'alignement
    if (!nearby_boids_alignment.empty())
    {
        std::vector<glm::vec3> velocities;
        velocities.reserve(nearby_boids_alignment.size());
        for (const auto& nearby_b : nearby_boids_alignment)
        {
            velocities.push_back(nearby_b.velocity);
        }
        glm::vec3 avg_velocity = averageVelocity(velocities);
        velocity               = velocity * 0.99f + avg_velocity * 0.01f;
    }

    // Règle de séparation
    if (!nearby_boids_separation.empty())
    {
        for (const auto& nearby_b : nearby_boids_separation)
        {
            glm::vec3 direction = position - nearby_b.position;
            float     distance  = glm::length(direction);
            if (distance < param.min_separation_distance)
            {
                velocity = velocity + (glm::normalize(direction) / distance);
            }
        }
    }

    // Règle de cohésion
    if (!nearby_boids_cohesion.empty())
    {
        glm::vec3 cohesion_force(1.f);
        for (const auto& nearby_b : nearby_boids_cohesion)
        {
            cohesion_force += nearby_b.position;
        }
        cohesion_force /= static_cast<float>(nearby_boids_cohesion.size());
        glm::vec3 direction_to_center = cohesion_force - position;
        velocity                      = velocity + glm::normalize(direction_to_center);
    }

    // Limite de vitesse
    float current_velocity = glm::length(velocity);
    if (current_velocity > param.maxVelocity)
    {
        velocity = glm::normalize(velocity) * param.maxVelocity;
    }
}

// Dessin des boids
void Boids::drawBoids(const Parameters& param, p6::Context* ctx)
{
    const auto sg1 = ctx->transform_scope_guard();

    position = position + velocity * ctx->delta_time();

    if (position.x > 15)
    {
        position = glm::vec3(-15, position.y, position.z);
    }
    else if (position.x < -15)
    {
        position = glm::vec3(15, position.y, position.z);
    }
    if (position.y > 15)
    {
        position = glm::vec3(position.x, -15, position.z);
    }
    else if (position.y < -15)
    {
        position = glm::vec3(position.x, 15, position.z);
    }
    if (position.z > 15)
    {
        position = glm::vec3(position.x, position.y, -15);
    }
    else if (position.z < -15)
    {
        position = glm::vec3(position.x, position.y, 15);
    }

    // ctx->translate({position});
    // ctx->fill = {1, 0, 0, 0.5};
    // ctx->equilateral_triangle(p6::Center{}, param.triangleRadius);

    // const std::vector<glimac::ShapeVertex> vertices = glimac::sphere_vertices(1.f, 32, 16);
}

glm::vec3 Boids::getPosition() const
{
    return position;
}