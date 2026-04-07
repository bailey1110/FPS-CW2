#include "Mesh.h"

// Constructor: initialise OpenGL handles and vertex count
Mesh::Mesh()
{
    VAO = 0;          // Vertex Array Object
    VBO = 0;          // Vertex Buffer Object
    vertexCount = 0;  // Number of floats in vertex data
}

// Initialise mesh with raw vertex data
void Mesh::init(float* vertices, unsigned int count)
{
    vertexCount = count; // Store number of floats

    glGenVertexArrays(1, &VAO); // Generate VAO
    glGenBuffers(1, &VBO);      // Generate VBO

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Upload vertex data to GPU
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * count, vertices, GL_STATIC_DRAW);

    // Position attribute (vec3: x, y, z)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0); // Unbind VAO
}

// Draw the mesh
void Mesh::draw()
{
    glBindVertexArray(VAO);

    // Draw triangles (3 floats per vertex)
    glDrawArrays(GL_TRIANGLES, 0, vertexCount / 3);

    glBindVertexArray(0);
}