#pragma once
#include "Model.h"
#include <glm/glm.hpp>

class Gun
{
public:
    Model model;

    void setup();
    void draw(unsigned int modelLoc, glm::vec3 playerPos, glm::vec3 front, glm::vec3 up);
};