#include <iostream>
#include <iterator>
#include "boidsManager.hpp"
#include "parameters.hpp"

#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest/doctest.h"
int main()
{
    // Création de la window
    auto ctx = p6::Context{{.title = "Boids Boids Boids"}};
    ctx.maximize_window();

    // Initialisation des paramètres
    Parameters parameters{};
    parameters.triangleRadius          = 0.1f;
    parameters.alignmentRadius         = 0.138f;
    parameters.separationRadius        = 0.286f;
    parameters.cohesionRadius          = -0.494f;
    parameters.maxVelocity             = 1.0f;
    parameters.min_separation_distance = parameters.triangleRadius * 2.f;

    // Initialisation de BoidsManager
    BoidsManager boidsManager(parameters);
    boidsManager.randomInitBoids();

    // Déclaration de la boucle de mise à jour infinie
    ctx.update = [&]() {
        // ImGui
        ImGui::Begin("Paramètres");
        ImGui::SliderFloat("Taille du triangle", &parameters.triangleRadius, 0.f, 1.f);
        ImGui::SliderFloat("Alignement", &parameters.alignmentRadius, -2.f, 2.f);
        ImGui::SliderFloat("Séparation", &parameters.separationRadius, -2.f, 2.f);
        ImGui::SliderFloat("Cohésion", &parameters.cohesionRadius, -2.f, 2.f);
        ImGui::End();

        // Fond
        ctx.background(p6::NamedColor::Blue);

        // Update et Draw
        boidsManager.update(&ctx, parameters);
    };

    // Lancement de la boucle infinie
    ctx.start();

    return 0;
}
