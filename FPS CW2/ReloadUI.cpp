#include "ReloadUI.h"
#include <glad/glad.h>

// Setup VAO/VBO for reload bar
void ReloadUI::setup()
{
    glGenVertexArrays(1, &VAO); // Generate VAO
    glGenBuffers(1, &VBO);      // Generate VBO

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Allocate buffer space (6 vertices × 3 floats), initially empty
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 18, nullptr, GL_DYNAMIC_DRAW);

    // Position attribute (vec3)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0); // Unbind VAO
}

// Update reload bar geometry based on progress (0 → 1)
void ReloadUI::update(float progress)
{
    // Clamp progress between 0 and 1
    if (progress < 0.0f) progress = 0.0f;
    if (progress > 1.0f) progress = 1.0f;

    // Define bar dimensions in screen space (NDC)
    float left = -0.95f;
    float right = left + (0.35f * progress); // Width scales with progress
    float top = -0.80f;
    float bottom = -0.88f;

    // Two triangles forming a rectangle
    float verts[] = {
        left,  top,    0.0f,
        right, top,    0.0f,
        right, bottom, 0.0f,

        right, bottom, 0.0f,
        left,  bottom, 0.0f,
        left,  top,    0.0f
    };

    // Update vertex data
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(verts), verts);
}

// Draw reload bar
void ReloadUI::draw(unsigned int shaderProgram)
{
    glDisable(GL_DEPTH_TEST); // Render UI on top

    glUseProgram(shaderProgram);

    // Set bar colour (green)
    int colorLoc = glGetUniformLocation(shaderProgram, "objectColor");
    glUniform3f(colorLoc, 0.0f, 1.0f, 0.0f);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6); // Draw rectangle (2 triangles)

    glEnable(GL_DEPTH_TEST); // Restore depth testing
}