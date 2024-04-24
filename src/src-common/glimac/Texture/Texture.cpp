#include "Texture.hpp"

Texture::Texture(const std::string& name)
    : _filePath("Textures/" + name + ".png")
{
    loadTexture();
}
void Texture::loadTexture()
{
    int            width, height, channels;
    unsigned char* data = stbi_load(_filePath.c_str(), &width, &height, &channels, 0);
    if (!data)
    {
        std::cerr << "Failed to load texture from file: " << _filePath << std::endl;
        return;
    }

    glGenTextures(1, &_textureID);
    glBindTexture(GL_TEXTURE_2D, _textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, (channels == 4 ? GL_RGBA : GL_RGB), width, height, 0, (channels == 4 ? GL_RGBA : GL_RGB), GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(data);
}