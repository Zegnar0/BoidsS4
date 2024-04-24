#pragma once
#include "p6/p6.h"
#include "stb_image/stb_image.h"

class Texture {
private:
    GLuint      _textureID;
    std::string _filePath;
    void        loadTexture();

public:
    explicit Texture(const std::string& name);

    GLuint getTextureID() const { return _textureID; }
};