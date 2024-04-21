#include "Mesh.hpp"
#include <cstddef> // For offsetof

Mesh::Mesh(const std::vector<glimac::ShapeVertex>& vertices)
    : _vertexCount(static_cast<GLsizei>(vertices.size()))
{
    glGenVertexArrays(1, &_vao);
    glGenBuffers(1, &_vbo);

    glBindVertexArray(_vao);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glimac::ShapeVertex), vertices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), (void*)offsetof(glimac::ShapeVertex, position));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), (void*)offsetof(glimac::ShapeVertex, normal));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), (void*)offsetof(glimac::ShapeVertex, texCoords));

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

Mesh::~Mesh()
{
    glDeleteBuffers(1, &_vbo);
    glDeleteVertexArrays(1, &_vao);
}

void Mesh::bind() const
{
    glBindVertexArray(_vao);
}

void Mesh::unbind() const
{
    glBindVertexArray(0);
}

GLsizei Mesh::vertexCount() const
{
    return _vertexCount;
}

void Mesh::setupMesh(const std::vector<glimac::ShapeVertex>& vertices)
{
    // Could contain additional setup logic if needed
}
