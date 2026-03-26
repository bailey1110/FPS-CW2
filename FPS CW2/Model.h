// Model.h
#pragma once
#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>

struct Vertex
{
    glm::vec3 position;
    glm::vec2 texCoords;
};

class Model
{
public:
    unsigned int VAO, VBO;
    std::vector<Vertex> vertices;

    Model();

    void loadSimpleCube();
    void loadPlane();
    void setupMesh();
    void draw();
    void loadCylinder(int segments = 20);
};