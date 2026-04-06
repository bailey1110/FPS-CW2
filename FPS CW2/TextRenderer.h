#pragma once

#include <map>
#include <string>
#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

struct Character
{
    glm::ivec2 Size;
    glm::ivec2 Bearing;
    unsigned int Advance;
    glm::vec2 UVMin;
    glm::vec2 UVMax;
};

class TextRenderer
{
public:
    std::map<char, Character> Characters;

    unsigned int VAO = 0;
    unsigned int VBO = 0;
    unsigned int atlasTexture = 0;

    GLint textColorLoc;
    GLint textSamplerLoc;

    void init(const char* fontPath, unsigned int shader);
    void renderText(unsigned int shader, const std::string& text, float x, float y, float scale, glm::vec3 color);
};