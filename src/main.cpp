#include <cstdlib>
#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest/doctest.h"
#include "p6/p6.h"

int main()
{
    // Run the tests
    if (doctest::Context{}.run() != 0)
        return EXIT_FAILURE;

    // Actual application code
    auto ctx = p6::Context{{.title = "Simple-p6-Setup"}};
    const p6::Angle angle  = 0.5_turn;
    
    std::string text          = "Hello";
    ctx.maximize_window();

    // Declare your infinite update loop.
    ctx.update = [&]() {
        // ImGui::Begin("Test");
        // ImGui::SliderFloat("Square size", &square_radius, 0.f, 1.f);
        // ImGui::InputText("Text", &text);
        // ImGui::End();
        // Show the official ImGui demo window
        // It is very useful to discover all the widgets available in ImGui
        // ImGui::ShowDemoWindow();
        ctx.background(p6::NamedColor::Blue);
        ctx.fill = {1, 0, 0, 0.5};
        ctx.equilateral_triangle(p6::Center{ctx.mouse()}, 0.1f, angle);

    };

    // Should be done last. It starts the infinite loop.
    ctx.start();
}