#include <cmath>
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
    auto        ctx             = p6::Context{{.title = "Simple-p6-Setup"}};
    auto        triangle_radius = 0.5f;
    float       positionX       = 0.f;
    float       positionY       = 0.f;
    float       vitesseX        = p6::random::number(-2.f, 2.f);
    float       vitesseY        = p6::random::number(-2.f, 2.f);
    int         nb_triangle     = 5;
    p6::Angle   angle           = 1_turn;
    std::string text            = "Hello";
    ctx.maximize_window();

    // Declare your infinite update loop.
    ctx.update = [&]() {
        ImGui::Begin("Param");
        ImGui::SliderFloat("Triangle size", &triangle_radius, 0.f, 1.f);
        ImGui::SliderFloat("Triangle vitesse X", &vitesseX, -2.f, 2.f);
        ImGui::SliderFloat("Triangle vitesse Y", &vitesseY, -2.f, 2.f);
        ImGui::End();
        ctx.background(p6::NamedColor::Blue);
        for (int i = 0; i < nb_triangle; i++)
        {
            if (round((ctx.time() - ctx.delta_time()) / 2) != round(ctx.time() / 2))
            {
                angle = p6::random::angle();
            }
            const auto sg1 = ctx.transform_scope_guard();
            vitesseX       = std::cos(angle.as_radians());
            vitesseY       = std::sin(angle.as_radians());
            positionX += vitesseX * ctx.delta_time();
            positionY += vitesseY * ctx.delta_time();
            // position += vitesse * p6::Rotation(angle).value;
            ctx.translate({positionX, positionY});
            {
                if (positionX > ctx.aspect_ratio())
                {
                    positionX = -ctx.aspect_ratio();
                }
                if (positionX < -ctx.aspect_ratio())
                {
                    positionX = ctx.aspect_ratio();
                }
                if (positionY > 1)
                {
                    positionY = -1;
                }
                if (positionY < -1)
                {
                    positionY = 1;
                }
                ctx.fill = {1, 0, 0, 0.5};
                ctx.equilateral_triangle(p6::Center{}, triangle_radius, angle);
            }
        }
    };

    // Should be done last. It starts the infinite loop.
    ctx.start();
}