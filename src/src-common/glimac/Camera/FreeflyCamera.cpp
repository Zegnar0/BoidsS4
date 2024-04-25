#include "FreeflyCamera.hpp"

FreeflyCamera::FreeflyCamera()
    : m_Position(glm::vec3(0.0f)), m_Phi(glm::pi<float>()), m_Theta(0.0f)
{
    computeDirectionVectors();
}

void FreeflyCamera::computeDirectionVectors()
{
    m_FrontVector = glm::vec3(
        cos(m_Theta) * sin(m_Phi),
        sin(m_Theta),
        cos(m_Theta) * cos(m_Phi)
    );

    m_LeftVector = glm::vec3(
        sin(m_Phi + glm::pi<float>() / 2.0f),
        0,
        cos(m_Phi + glm::pi<float>() / 2.0f)
    );

    m_UpVector = glm::cross(m_FrontVector, m_LeftVector);
}

void FreeflyCamera::moveLeft(float t)
{
    m_Position += t * m_LeftVector;
}

void FreeflyCamera::moveFront(float t)
{
    m_Position += t * m_FrontVector;
}

void FreeflyCamera::rotateLeft(float degrees)
{
    m_Phi += glm::radians(degrees);
    computeDirectionVectors();
}

void FreeflyCamera::rotateUp(float degrees)
{
    m_Theta += glm::radians(degrees);
    computeDirectionVectors();
}

glm::mat4 FreeflyCamera::getViewMatrix() const
{
    return glm::lookAt(m_Position, m_Position + m_FrontVector, m_UpVector);
}

glm::vec3 FreeflyCamera::getPosition() const
{
    return m_Position;
}

void FreeflyCamera::setPosition(const glm::vec3& position)
{
    m_Position = position;
    computeDirectionVectors();
}
