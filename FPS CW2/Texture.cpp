#include "Texture.h"
#include "stb_image.h"
#include <iostream>
#include <string>

// Constructor: initialise texture ID
Texture::Texture()
{
    ID = 0; // No texture yet
}

// Load texture from file into GPU
void Texture::load(const char* path)
{
    glGenTextures(1, &ID); // Generate texture ID
    glBindTexture(GL_TEXTURE_2D, ID); // Bind texture

    // Set wrapping and filtering options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_set_flip_vertically_on_load(true); // Flip image for OpenGL

    int width, height, channels;
    unsigned char* data = nullptr;

    // Try loading texture from given path
    data = stbi_load(path, &width, &height, &channels, 0);

    // Try alternative path (Visual Studio build folder)
    if (!data)
    {
        std::string alt1 = std::string("x64/Debug/") + path;
        data = stbi_load(alt1.c_str(), &width, &height, &channels, 0);
    }

    // Try alternative path (current directory)
    if (!data)
    {
        std::string alt2 = std::string("./") + path;
        data = stbi_load(alt2.c_str(), &width, &height, &channels, 0);
    }

    // If texture loaded successfully
    if (data)
    {
        GLenum format = GL_RGB;

        // Determine format based on number of channels
        if (channels == 1) format = GL_RED;
        if (channels == 3) format = GL_RGB;
        if (channels == 4) format = GL_RGBA;

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Ensure proper alignment

        // Upload texture data to GPU
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D); // Generate mipmaps

        stbi_image_free(data); // Free image memory
    }
    else
    {
        // Print error if loading failed
        std::cout << "Failed to load texture: " << path << std::endl;
    }
}

// Bind texture to a texture unit
void Texture::bind(unsigned int unit)
{
    static unsigned int lastBound[32] = { 0 }; // Cache of last bound textures per unit

    if (unit >= 32)
        unit = 0; // Clamp to valid range

    // Skip binding if already bound to this unit
    if (lastBound[unit] == ID)
        return;

    glActiveTexture(GL_TEXTURE0 + unit); // Activate texture unit
    glBindTexture(GL_TEXTURE_2D, ID);    // Bind texture
    lastBound[unit] = ID;                // Update cache
}