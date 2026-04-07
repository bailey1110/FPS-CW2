#pragma once
#include <glad/glad.h>

// Handles loading and binding of 2D textures in OpenGL
class Texture
{
public:
    unsigned int ID; // OpenGL texture ID

    // Constructor: initialises texture ID
    Texture();

    // Load an image file and upload it as a texture to the GPU
    void load(const char* path);

    // Bind the texture to a given texture unit (default = 0)
    void bind(unsigned int unit = 0);
};