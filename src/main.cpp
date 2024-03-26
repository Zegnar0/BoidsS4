#include <cmath>
#include <cstdlib>
#define DOCTEST_CONFIG_IMPLEMENT
#include <vector>
#include "doctest/doctest.h"
#include "p6/p6.h"

struct Boids {
    glm::vec2 position;
    glm::vec2 velocity;
};

Boids boids[100];

// Définition des constantes pour les règles de Boids
float       alignment_radius  = 0.5f; // Rayon pour le calcul de l'alignement
float       separation_radius = 0.2f; // Rayon pour le calcul de la séparation
float       cohesion_radius   = 0.7f; // Rayon pour le calcul de la cohésion
const float max_velocity      = 1.0f;
auto        triangle_radius   = 0.1f;

void random_init_boids()
{
    for (auto& boid : boids)
    {
        boid.position = {p6::random::number(-2.f, 2.f), p6::random::number(-2.f, 2.f)};
        boid.velocity = {p6::random::number(-2.f, 2.f), p6::random::number(-2.f, 2.f)};
    }
}

// Fonction pour calculer la distance entre deux boids
float distance(const Boids& boid1, const Boids& boid2)
{
    return glm::length(boid2.position - boid1.position);
}

// Fonction pour calculer la moyenne des angles
glm::vec2 average_velocity(const std::vector<glm::vec2>& velocities)
{
    glm::vec2 sum(0.f);
    for (const auto& velocity : velocities)
    {
        sum += velocity;
    }
    sum /= velocities.size();
    return sum;
}

// Mise à jour des vélocités des boids selon les règles
void update_boids()
{
    for (auto& boid : boids)
    {
        std::vector<Boids> nearby_boids_alignment;
        std::vector<Boids> nearby_boids_separation;
        std::vector<Boids> nearby_boids_cohesion;
        float              min_separation_distance = triangle_radius * 2.f;
        // Calcul des boids voisins
        for (const auto& other_boid : boids)
        {
            if (&other_boid != &boid)
            {
                float dist = distance(boid, other_boid);
                // Alignment
                if (dist < alignment_radius)
                {
                    nearby_boids_alignment.push_back(other_boid);
                }
                // Separation
                if (dist < separation_radius)
                {
                    nearby_boids_separation.push_back(other_boid);
                }
                // Cohesion
                if (dist < cohesion_radius)
                {
                    nearby_boids_cohesion.push_back(other_boid);
                }
            }
        }

        // Règle d'alignement
        if (!nearby_boids_alignment.empty())
        {
            std::vector<glm::vec2> velocities;
            velocities.reserve(nearby_boids_alignment.size());
            for (const auto& nearby_boid : nearby_boids_alignment)
            {
                velocities.push_back(nearby_boid.velocity);
            }
            glm::vec2 avg_velocity = average_velocity(velocities);
            boid.velocity          = boid.velocity * 0.99f + avg_velocity * 0.01f;
        }

        // Règle de séparation
        if (!nearby_boids_separation.empty())
        {
            for (const auto& nearby_boid : nearby_boids_separation)
            {
                glm::vec2 direction = boid.position - nearby_boid.position;
                float     distance  = glm::length(direction);
                if (distance < min_separation_distance)
                {
                    // Appliquer une répulsion en ajustant la vélocité
                    boid.velocity += glm::normalize(direction) / distance;
                }
            }
        }

        // Règle de cohésion
        if (!nearby_boids_cohesion.empty())
        {
            glm::vec2 cohesion_force(1.f);
            for (const auto& nearby_boid : nearby_boids_cohesion)
            {
                cohesion_force += nearby_boid.position;
            }
            cohesion_force /= static_cast<float>(nearby_boids_cohesion.size());
            glm::vec2 direction_to_center = cohesion_force - boid.position;
            boid.velocity += glm::normalize(direction_to_center);
        }

        float current_velocity = glm::length(boid.velocity);
        if (current_velocity > max_velocity)
        {
            boid.velocity = glm::normalize(boid.velocity) * max_velocity;
        }
    }
}

int main()
{
    // Run the tests
    if (doctest::Context{}.run() != 0)
        return EXIT_FAILURE;

    // Actual application code
    auto ctx = p6::Context{{.title = "Boids Boids Boids"}};

    ctx.maximize_window();
    random_init_boids();

    // Declare your infinite update loop.
    ctx.update = [&]() {
        ImGui::Begin("Param");
        ImGui::SliderFloat("Triangle size", &triangle_radius, 0.f, 1.f);
        ImGui::SliderFloat("Alignement", &alignment_radius, -2.f, 2.f);
        ImGui::SliderFloat("Séparation", &separation_radius, -2.f, 2.f);
        ImGui::SliderFloat("Cohésion", &cohesion_radius, -2.f, 2.f);
        ImGui::End();
        ctx.background(p6::NamedColor::Blue);
        for (auto& boid : boids)
        {
            /*if (round((ctx.time() - ctx.delta_time()) / 2) != round(ctx.time() / 2))
            {
                boid.angle = boid.angle + p6::random::angle();
            }*/
            const auto sg1 = ctx.transform_scope_guard();
            boid.position.x += boid.velocity.x * ctx.delta_time();
            boid.position.y += boid.velocity.y * ctx.delta_time();

            // float x = std::fmod(boid.position.x, 2 * ctx.aspect_ratio()) - ctx.aspect_ratio();
            // float y = std::fmod(boid.position.y, 2.0f) - 1.0f;
            ctx.translate({boid.position.x, boid.position.y});
            {
                if (boid.position.x > ctx.aspect_ratio())
                {
                    boid.position.x = -ctx.aspect_ratio();
                }
                if (boid.position.x < -ctx.aspect_ratio())
                {
                    boid.position.x = ctx.aspect_ratio();
                }
                if (boid.position.y > 1)
                {
                    boid.position.y = -1;
                }
                if (boid.position.y < -1)
                {
                    boid.position.y = 1;
                }
                ctx.fill = {1, 0, 0, 0.5};
                ctx.equilateral_triangle(p6::Center{}, triangle_radius);
            };
        };
        update_boids();
    };
    // Should be done last. It starts the infinite loop.
    ctx.start();
}