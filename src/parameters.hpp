#pragma once
#include <glm/glm.hpp>

struct Parameters {
    float triangleRadius   = 0.1f;
    float alignmentRadius  = 0.138f;
    float separationRadius = 0.286f;
    float cohesionRadius   = -0.494f;
    float maxVelocity      = 1.0f;
    float min_separation_distance;
    int   LOD = 1;

    Parameters()
    {
        min_separation_distance = triangleRadius * 2.f;
    }

    void drawImGui();
};

Parameters getData();
