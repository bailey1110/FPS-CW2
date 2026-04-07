#pragma once
#include <glad/glad.h>

class Texture
{
public:
    unsigned int ID;

    Texture();

    void load(const char* path);
    void bind(unsigned int unit = 0);
};