#pragma once

#include <glad/glad.h>

// Represents a simple mesh using raw vertex data
class Mesh
{
public:
    unsigned int VAO;        // Vertex Array Object (stores vertex attribute configuration)
    unsigned int VBO;        // Vertex Buffer Object (stores vertex data on GPU)
    unsigned int vertexCount; // Total number of float values in the vertex buffer

    // Constructor: initialise mesh with default values
    Mesh();

    // Upload vertex data to GPU
    // vertices = pointer to array of floats (positions)
    // count = number of floats in the array
    void init(float* vertices, unsigned int count);

    // Render the mesh
    void draw();
};