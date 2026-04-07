#pragma once

#include <vector>
#include <string>
#include <cstddef>
#include <glad/glad.h>
#include <glm/glm.hpp>

// Represents a single vertex with position, texture coordinates, and normal
struct Vertex
{
    glm::vec3 position;   // Vertex position in 3D space
    glm::vec2 texCoords;  // Texture coordinates (UV)
    glm::vec3 normal;     // Normal vector for lighting calculations
};

// Handles loading, storing, and rendering 3D models
class Model
{
public:
    unsigned int VAO;                 // Vertex Array Object
    unsigned int VBO;                 // Vertex Buffer Object
    std::vector<Vertex> vertices;     // List of vertices making up the model

    // Constructor: initialise OpenGL handles
    Model();

    // Load a simple cube (basic geometry)
    void loadSimpleCube();

    // Load a flat plane (used for floor)
    void loadPlane();

    // Generate a cylinder procedurally
    void loadCylinder(int segments = 20);

    // Load a model from an OBJ file
    // Returns true if successful
    bool loadOBJ(const std::string& path);

    // Upload vertex data to GPU and configure attributes
    void setupMesh();

    // Render the model
    void draw();
};