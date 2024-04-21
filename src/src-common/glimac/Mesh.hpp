#pragma once
#include <cstddef>
#include <glm/gtc/random.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "p6/p6.h"
#include "src-common/glimac/FreeflyCamera.hpp"
#include "src-common/glimac/ModelLoader.hpp"
#include "src-common/glimac/common.hpp"

class Mesh {
public:
    Mesh(const std::vector<glimac::ShapeVertex>& vertices);
    ~Mesh();
    Mesh(const Mesh&)              = delete;
    Mesh&   operator=(const Mesh&) = delete;
    void    bind() const;
    void    unbind() const;
    GLsizei vertexCount() const;

private:
    GLuint  _vbo;
    GLuint  _vao;
    GLsizei _vertexCount;

    void setupMesh(const std::vector<glimac::ShapeVertex>& vertices);
};
