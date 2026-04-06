#pragma once

#include <glm/glm.hpp>
#include "Model.h"
#include "Texture.h"

class Enemy
{
public:
    glm::vec3 position;
    float radius = 0.5f;

    bool isActive;

    Model* model;
    Texture* texture;

    void setup(Model* m, Texture* t);

    bool checkHit(const glm::vec3& origin, const glm::vec3& dir);

    bool isColliding(const glm::vec3& otherPos, float otherRadius);

    void draw(unsigned int shader, GLint modelLoc, GLint useTextureLoc);
};