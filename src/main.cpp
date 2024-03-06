#include <cmath>
#include <cstdlib>
#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest/doctest.h"
#include "p6/p6.h"

struct Boids {
    glm::vec2 position;
    glm::vec2 velocity;
    float     angle;
};

Boids boids[5];

void random_init_boids()
{
    for (auto& boid : boids)
    {
        boid.position = {p6::random::number(-2.f, 2.f), p6::random::number(-2.f, 2.f)};
        boid.velocity = {0.f, 0.f};
        boid.angle    = 15;
    }
}

float speed = 0.5f;

int main()
{
    // Run the tests
    if (doctest::Context{}.run() != 0)
        return EXIT_FAILURE;

    // Actual application code
    auto      ctx             = p6::Context{{.title = "Boids Boids Boids"}};
    auto      triangle_radius = 0.5f;
    p6::Angle angle           = 1_turn;
    ctx.maximize_window();
    random_init_boids();

    // Declare your infinite update loop.
    ctx.update = [&]() {
        ImGui::Begin("Param");
        ImGui::SliderFloat("Triangle size", &triangle_radius, 0.f, 1.f);
        // ImGui::SliderFloat("Triangle vitesse X", &vitesseX, -2.f, 2.f);
        // ImGui::SliderFloat("Triangle vitesse Y", &vitesseY, -2.f, 2.f);
        ImGui::End();
        ctx.background(p6::NamedColor::Blue);
        for (auto& boid : boids)
        {
            if (round((ctx.time() - ctx.delta_time()) / 2) != round(ctx.time() / 2))
            {
                angle = p6::random::angle();
            }
            const auto sg1  = ctx.transform_scope_guard();
            boid.velocity.x = std::cos(angle.as_radians());
            boid.velocity.y = std::sin(angle.as_radians());
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
                ctx.equilateral_triangle(p6::Center{}, triangle_radius, angle);
            };
        };
    };
    // Should be done last. It starts the infinite loop.
    ctx.start();
}