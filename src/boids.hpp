#pragma once
#include <glm/glm.hpp>

class Boids {
    glm::vec2 coordonnee;

    float acceleration, velocity, position;
    float taille;
    float maxspeed, maxforce;
};
