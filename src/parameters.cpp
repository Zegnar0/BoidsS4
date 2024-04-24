#include "parameters.hpp"
#include <imgui.h>

void Parameters::drawImGui()
{
    ImGui::Begin("Paramètres");

    ImGui::SliderFloat("Alignement", &alignmentRadius, -10.f, 10.f, "Align = %.3f");
    ImGui::SliderFloat("Séparation", &separationRadius, -10.f, 10.f, "Separ = %.3f");
    ImGui::SliderFloat("Cohésion", &cohesionRadius, -10.f, 10.f, "Cohes = %.3f");
    ImGui::SliderInt("LOD", &LOD, 0, 1);

    if (ImGui::Button("Reset Defaults"))
    {
        triangleRadius          = 0.1f;
        alignmentRadius         = 0.138f;
        separationRadius        = 0.286f;
        cohesionRadius          = -0.494f;
        maxVelocity             = 1.0f;
        LOD                     = 1;
        min_separation_distance = triangleRadius * 2.f;
    }

    ImGui::End();
}

Parameters getData()
{
    return Parameters();
}