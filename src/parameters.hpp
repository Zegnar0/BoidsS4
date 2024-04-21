#pragma once
#include <glm/glm.hpp>

struct Parameters {
    float min_separation_distance;
    float alignmentRadius;
    float separationRadius;
    float cohesionRadius;
    float maxVelocity;
    float triangleRadius;
    int   LOD;
};