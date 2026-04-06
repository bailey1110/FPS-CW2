#pragma once

#include <map>
#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

struct Character
{
    unsigned int TextureID;
    glm::ivec2 Size;
    glm::ivec2 Bearing;
    unsigned int Advance;
};

class TextRenderer
{
public:
    std::map<char, Character> Characters;

    unsigned int VAO, VBO;

    void init(const char* fontPath);
    void renderText(unsigned int shader, std::string text, float x, float y, float scale, glm::vec3 color);
};