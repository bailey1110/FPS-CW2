#include "Crosshair.h"
#include <glad/glad.h>

// Vertex data for crosshair (two lines: horizontal and vertical)
float crosshairVertices[] = {
    -0.02f,  0.0f, 0.0f, // left point
     0.02f,  0.0f, 0.0f, // right point

     0.0f, -0.02f, 0.0f, // bottom point
     0.0f,  0.02f, 0.0f  // top point
};

// Setup crosshair geometry and upload to GPU
void Crosshair::setup()
{
    glGenVertexArrays(1, &VAO); // Generate VAO
    glGenBuffers(1, &VBO);      // Generate VBO

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Upload static vertex data
    glBufferData(GL_ARRAY_BUFFER, sizeof(crosshairVertices), crosshairVertices, GL_STATIC_DRAW);

    // Position attribute (vec3)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0); // Unbind VAO
}

// Draw the crosshair
void Crosshair::draw(unsigned int shaderProgram)
{
    glDisable(GL_DEPTH_TEST); // Ensure crosshair renders on top

    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);

    // Draw 2 lines (4 vertices)
    glDrawArrays(GL_LINES, 0, 4);

    glEnable(GL_DEPTH_TEST); // Restore depth testing
}