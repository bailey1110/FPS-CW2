// Compass.h

#pragma once
#include <glm/glm.hpp>

class Compass
{
public:
    unsigned int VAO, VBO;

    void setup();
    void draw(unsigned int shader, glm::vec3 playerPos, glm::vec3 playerFront, glm::vec3 enemyPos);
};