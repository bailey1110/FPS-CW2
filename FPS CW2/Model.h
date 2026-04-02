// Model.h

#pragma once

#include <vector>
#include <string>
#include <cstddef>
#include <glad/glad.h>
#include <glm/glm.hpp>

struct Vertex
{
    glm::vec3 position;
    glm::vec2 texCoords;
    glm::vec3 normal;
};

class Model
{
public:
    unsigned int VAO;
    unsigned int VBO;
    std::vector<Vertex> vertices;

    Model();

    void loadSimpleCube();
    void loadPlane();
    void loadCylinder(int segments = 20);
    bool loadOBJ(const std::string& path);

    void setupMesh();
    void draw();
};