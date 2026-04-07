#pragma once

#include <glm/glm.hpp>
#include "Model.h"
#include "Texture.h"

// Represents an enemy entity in the game world
class Enemy
{
public:
    glm::vec3 position;   // World position of the enemy
    float radius = 0.5f;  // Collision radius for hit detection

    bool isActive;        // Whether the enemy is alive/active

    Model* model;         // Pointer to the 3D model used for rendering
    Texture* texture;     // Pointer to the texture applied to the model

    // Initialise enemy with a model and texture
    void setup(Model* m, Texture* t);

    // Check if a ray (e.g. bullet) hits the enemy (ray-sphere intersection)
    // origin = ray start position
    // dir = ray direction (should be normalized)
    bool checkHit(const glm::vec3& origin, const glm::vec3& dir);

    // Check collision with another sphere (e.g. player collision)
    bool isColliding(const glm::vec3& otherPos, float otherRadius);

    // Render the enemy
    void draw(unsigned int shader, GLint modelLoc, GLint useTextureLoc);
};