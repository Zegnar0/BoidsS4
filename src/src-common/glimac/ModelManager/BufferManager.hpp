#pragma once
#include <vector>
#include "common.hpp"
#include "p6/p6.h"

class BufferManager {
public:
    BufferManager();
    ~BufferManager();

    void createBuffers(const std::vector<glimac::ShapeVertex>& vertices);
    void bindVAO() const;
    void unbindVAO() const;
    void deleteBuffers();

private:
    GLuint _vao;
    GLuint _vbo;

    void setupAttributes() const;
};
