#include <cstdlib>
#include "glm/fwd.hpp"
#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest/doctest.h"
#include "p6/p6.h"
float speed = 0.1; // Default speed

// Function to set speed
void set_speed(float new_speed)
{
    speed = new_speed;
}

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

    // Declare your infinite update loop.
    ctx.update = [&]() {
        angle += speed * 0.01_turn; // Use speed here
        // ImGui::Begin("Test");
        // ImGui::SliderFloat("Square size", &square_radius, 0.f, 1.f);
        // ImGui::InputText("Text", &text);
        // ImGui::End();
        // Show the official ImGui demo window
        // It is very useful to discover all the widgets available in ImGui
        // ImGui::ShowDemoWindow();
        ctx.background(p6::NamedColor::Blue);
        ctx.fill          = {1, 0, 0, 0.5};
        float aspectRatio = ctx.aspect_ratio();
        float x           = std::fmod(0.5f * ctx.time(), 2 * aspectRatio) - aspectRatio;
        float y           = std::fmod(0.5f * ctx.time(), 2.0f) - 1.0f;
        x += speed; // Use speed here
        y += speed; // Use speed here
        const auto sg1 = ctx.transform_scope_guard();
        ctx.translate({x, y});
        ctx.equilateral_triangle(p6::Center{}, 0.2f, angle);
    };

    // Should be done last. It starts the infinite loop.
    ctx.start();
}