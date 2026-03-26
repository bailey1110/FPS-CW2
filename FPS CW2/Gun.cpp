// Gun.cpp
#include "Gun.h"
#include <glm/gtc/matrix_transform.hpp>

void Gun::setup()
{
    model.loadCylinder(20);
}

void Gun::draw(unsigned int modelLoc, glm::vec3 playerPos, glm::vec3 front, glm::vec3 up)
{
    glm::vec3 f = glm::normalize(front);
    glm::vec3 r = glm::normalize(glm::cross(f, up));
    glm::vec3 u = glm::normalize(glm::cross(r, f));

    glm::vec3 startPos =
        playerPos
        - f * 0.5f
        + r * 0.2f
        - u * 0.2f;

    glm::mat4 rotation = glm::mat4(1.0f);
    rotation[0] = glm::vec4(r, 0.0f);
    rotation[1] = glm::vec4(u, 0.0f);
    rotation[2] = glm::vec4(f, 0.0f);

    glm::mat4 modelMat = glm::translate(glm::mat4(1.0f), startPos) * rotation;

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &modelMat[0][0]);

    model.draw();
}