#include "Texture.h"
#include "stb_image.h"
#include <iostream>
#include <string>

Texture::Texture()
{
    ID = 0;
}

void Texture::load(const char* path)
{
    glGenTextures(1, &ID);
    glBindTexture(GL_TEXTURE_2D, ID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_set_flip_vertically_on_load(true);

    int width, height, channels;
    unsigned char* data = nullptr;

    data = stbi_load(path, &width, &height, &channels, 0);

    if (!data)
    {
        std::string alt1 = std::string("x64/Debug/") + path;
        data = stbi_load(alt1.c_str(), &width, &height, &channels, 0);
    }

    if (!data)
    {
        std::string alt2 = std::string("./") + path;
        data = stbi_load(alt2.c_str(), &width, &height, &channels, 0);
    }

    if (data)
    {
        GLenum format = GL_RGB;
        if (channels == 1) format = GL_RED;
        if (channels == 3) format = GL_RGB;
        if (channels == 4) format = GL_RGBA;

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Failed to load texture: " << path << std::endl;
    }
}

void Texture::bind(unsigned int unit)
{
    static unsigned int lastBound[32] = { 0 };

    if (unit >= 32)
        unit = 0;

    if (lastBound[unit] == ID)
        return;

    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, ID);
    lastBound[unit] = ID;
}