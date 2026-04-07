#pragma once

#include <map>
#include <string>
#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

// Stores data required to render a single character glyph
struct Character
{
    glm::ivec2 Size;      // Width and height of glyph in pixels
    glm::ivec2 Bearing;   // Offset from baseline to left/top of glyph
    unsigned int Advance; // Horizontal offset to next character

    glm::vec2 UVMin;      // Bottom-left UV coordinate in texture atlas
    glm::vec2 UVMax;      // Top-right UV coordinate in texture atlas
};

// Handles font loading and text rendering
class TextRenderer
{
public:
    std::map<char, Character> Characters; // Character lookup table

    unsigned int VAO = 0;         // Vertex Array Object
    unsigned int VBO = 0;         // Vertex Buffer Object
    unsigned int atlasTexture = 0; // Texture atlas for glyphs

    GLint textColorLoc;   // Shader uniform for text colour
    GLint textSamplerLoc; // Shader uniform for texture sampler

    // Initialise font and create glyph atlas
    void init(const char* fontPath, unsigned int shader);

    // Render text string at position (x, y)
    void renderText(unsigned int shader, const std::string& text, float x, float y, float scale, glm::vec3 color);
};