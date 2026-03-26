// Mesh.h
#pragma once
#include <glad/glad.h>

class Mesh
{
public:
    unsigned int VAO;
    unsigned int VBO;
    unsigned int vertexCount;

    Mesh();
    void init(float* vertices, unsigned int count);
    void draw();
};