#include "ReloadUI.h"
#include <glad/glad.h>

void ReloadUI::setup()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 18, nullptr, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

void ReloadUI::update(float progress)
{
    if (progress < 0.0f) progress = 0.0f;
    if (progress > 1.0f) progress = 1.0f;

    // bottom-left corner with slight offset
    float left = -0.95f;
    float right = left + (0.35f * progress);
    float top = -0.80f;
    float bottom = -0.88f;

    float verts[] = {
        left,  top,    0.0f,
        right, top,    0.0f,
        right, bottom, 0.0f,

        right, bottom, 0.0f,
        left,  bottom, 0.0f,
        left,  top,    0.0f
    };

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(verts), verts);
}

void ReloadUI::draw(unsigned int shaderProgram)
{
    glDisable(GL_DEPTH_TEST);

    glUseProgram(shaderProgram);

    int colorLoc = glGetUniformLocation(shaderProgram, "objectColor");
    glUniform3f(colorLoc, 0.0f, 1.0f, 0.0f);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glEnable(GL_DEPTH_TEST);
}