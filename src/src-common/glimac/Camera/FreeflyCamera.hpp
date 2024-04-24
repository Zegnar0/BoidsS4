#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

class FreeflyCamera {
private:
    glm::vec3 m_Position;
    float     m_Phi;
    float     m_Theta;
    glm::vec3 m_FrontVector;
    glm::vec3 m_LeftVector;
    glm::vec3 m_UpVector;

    void computeDirectionVectors();

public:
    FreeflyCamera();

    void      moveLeft(float t);
    void      moveFront(float t);
    void      rotateLeft(float degrees);
    void      rotateUp(float degrees);
    glm::vec3 getPosition() const;
    glm::mat4 getViewMatrix() const;
};
