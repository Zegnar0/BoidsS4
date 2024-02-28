#include <cstdlib>
#include "glm/fwd.hpp"
#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest/doctest.h"
#include "p6/p6.h"

struct Boids {
    glm::vec2 position;
    glm::vec2 velocity;
    float     angle;
};

Boids boids[100];

void random_init_boids()
{
    for (auto& boid : boids)
    {
        boid.position = {p6::random::number(), p6::random::number()};
        boid.velocity = {p6::random::number(), p6::random::number()};
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
    auto      ctx   = p6::Context{{.title = "Boids De fou"}};
    p6::Angle angle = 0.7_turn;

    std::string text = "Hello";
    ctx.maximize_window();
    random_init_boids();

    // Declare your infinite update loop.
    ctx.update = [&]() {
        angle += speed * 0.01_turn;
        ctx.background(p6::NamedColor::Blue);

        for (auto& boid : boids)
        {
            boid.angle += 0.01f;
            ctx.fill               = {1, 0, 0, 0.5};
            float      aspectRatio = ctx.aspect_ratio();
            float      x           = std::fmod(speed * boid.position.x * ctx.time(), 2 * aspectRatio) - aspectRatio;
            float      y           = std::fmod(speed * boid.position.y * ctx.time(), 2.0f) - 1.0f;
            const auto sg1         = ctx.transform_scope_guard();
            ctx.translate({x, y});
            ctx.equilateral_triangle(p6::Center{}, 0.1f, angle);
        }
        // Use speed here
        // ImGui::Begin("Test");
        // ImGui::SliderFloat("Square size", &square_radius, 0.f, 1.f);
        // ImGui::InputText("Text", &text);
        // ImGui::End();
        // Show the official ImGui demo window
        // It is very useful to discover all the widgets available in ImGui
        // ImGui::ShowDemoWindow();
    };

    // Should be done last. It starts the infinite loop.
    ctx.start();
}