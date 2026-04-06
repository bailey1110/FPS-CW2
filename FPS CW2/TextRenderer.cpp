#include "TextRenderer.h"
#include <iostream>

void TextRenderer::init(const char* fontPath, unsigned int shader)
{
    textColorLoc = glGetUniformLocation(shader, "textColor");
    textSamplerLoc = glGetUniformLocation(shader, "text");

    FT_Library ft;
    FT_Init_FreeType(&ft);

    FT_Face face;
    FT_New_Face(ft, fontPath, 0, &face);

    FT_Set_Pixel_Sizes(face, 0, 48);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    int atlasWidth = 0;
    int atlasHeight = 0;

    for (unsigned char c = 0; c < 128; c++)
    {
        FT_Load_Char(face, c, FT_LOAD_RENDER);
        atlasWidth += face->glyph->bitmap.width + 1;
        atlasHeight = std::max(atlasHeight, (int)face->glyph->bitmap.rows);
    }

    std::vector<unsigned char> atlasData(atlasWidth * atlasHeight);

    int xOffset = 0;

    for (unsigned char c = 0; c < 128; c++)
    {
        FT_Load_Char(face, c, FT_LOAD_RENDER);

        FT_Bitmap& bmp = face->glyph->bitmap;

        for (int row = 0; row < (int)bmp.rows; row++)
        {
            for (int col = 0; col < (int)bmp.width; col++)
            {
                atlasData[row * atlasWidth + xOffset + col] =
                    bmp.buffer[row * bmp.pitch + col];
            }
        }

        Character ch;
        ch.Size = { bmp.width, bmp.rows };
        ch.Bearing = { face->glyph->bitmap_left, face->glyph->bitmap_top };
        ch.Advance = face->glyph->advance.x;

        ch.UVMin = { (float)xOffset / atlasWidth, 0.0f };
        ch.UVMax = {
            (float)(xOffset + bmp.width) / atlasWidth,
            (float)bmp.rows / atlasHeight
        };

        Characters.insert({ c, ch });

        xOffset += bmp.width + 1;
    }

    glGenTextures(1, &atlasTexture);
    glBindTexture(GL_TEXTURE_2D, atlasTexture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, atlasWidth, atlasHeight, 0,
        GL_RED, GL_UNSIGNED_BYTE, atlasData.data());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4 * 128, nullptr, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);

    glBindVertexArray(0);
}

void TextRenderer::renderText(unsigned int shader, const std::string& text, float x, float y, float scale, glm::vec3 color)
{
    std::vector<float> vertices;
    vertices.reserve(text.size() * 24);

    for (char c : text)
    {
        const Character& ch = Characters[c];

        float xpos = x + ch.Bearing.x * scale;
        float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

        float w = ch.Size.x * scale;
        float h = ch.Size.y * scale;

        float u0 = ch.UVMin.x;
        float v0 = ch.UVMin.y;
        float u1 = ch.UVMax.x;
        float v1 = ch.UVMax.y;

        float quad[] = {
            xpos, ypos + h, u0, v0,
            xpos, ypos,     u0, v1,
            xpos + w, ypos, u1, v1,

            xpos, ypos + h, u0, v0,
            xpos + w, ypos, u1, v1,
            xpos + w, ypos + h, u1, v0
        };

        vertices.insert(vertices.end(), quad, quad + 24);

        x += (ch.Advance >> 6) * scale;
    }

    glUseProgram(shader);
    glUniform3f(textColorLoc, color.x, color.y, color.z);
    glUniform1i(textSamplerLoc, 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, atlasTexture);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_DYNAMIC_DRAW);

    glDrawArrays(GL_TRIANGLES, 0, vertices.size() / 4);

    glBindVertexArray(0);
}