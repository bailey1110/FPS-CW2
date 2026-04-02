// Enemy.h

#pragma once
#include <glm/glm.hpp>
#include "Model.h"
#include "Texture.h"

class Enemy
{
public:
    Model* model;
    Texture* texture;

    glm::vec3 position;
    bool isActive;

    float radius;

    void setup(Model* m, Texture* t);
    void draw(unsigned int shaderProgram, unsigned int modelLoc, unsigned int useTextureLoc);
    bool checkHit(glm::vec3 rayOrigin, glm::vec3 rayDir);
};