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
    auto        square_radius = 0.5f;
    std::string text          = "Hello";
    ctx.maximize_window();

    // Declare your infinite update loop.
    ctx.update = [&]() {
        ImGui::Begin("Test");
        ImGui::SliderFloat("Square size", &square_radius, 0.f, 1.f);
        ImGui::InputText("Text", &text);
        ImGui::End();
        // Show the official ImGui demo window
        // It is very useful to discover all the widgets available in ImGui
        ImGui::ShowDemoWindow();
        ctx.background(p6::NamedColor::Blue);
        ctx.circle(
            p6::Center{ctx.mouse()},
            p6::Radius{0.2f}
        );
        ctx.square(p6::Center{}, p6::Radius{square_radius});
    };

    // Should be done last. It starts the infinite loop.
    ctx.start();
}