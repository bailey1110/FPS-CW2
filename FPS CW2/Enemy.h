// Enemy.h (UPDATED TO USE MODEL)
#pragma once
#include <glm/glm.hpp>
#include "Model.h"

class Enemy
{
public:
    Model model;
    glm::vec3 position;
    bool isHit;

    void setup();
    void draw(unsigned int shaderProgram, unsigned int modelLoc, unsigned int colorLoc);
    bool checkHit(glm::vec3 rayOrigin, glm::vec3 rayDir);
};