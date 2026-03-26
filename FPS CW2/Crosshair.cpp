#include "Crosshair.h"
#include <glad/glad.h>

float crosshairVertices[] = {
    -0.02f,  0.0f, 0.0f,
     0.02f,  0.0f, 0.0f,

     0.0f, -0.02f, 0.0f,
     0.0f,  0.02f, 0.0f
};

void Crosshair::setup()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(crosshairVertices), crosshairVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

void Crosshair::draw(unsigned int shaderProgram)
{
    glDisable(GL_DEPTH_TEST);

    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glDrawArrays(GL_LINES, 0, 4);

    glEnable(GL_DEPTH_TEST);
}