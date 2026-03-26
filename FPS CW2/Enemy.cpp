// Enemy.cpp (UPDATED TO USE MODEL)
#include "Enemy.h"
#include <glm/gtc/matrix_transform.hpp>

void Enemy::setup()
{
    model.loadSimpleCube();

    position = glm::vec3(0.0f, 0.5f, -5.0f);
    isHit = false;
}

void Enemy::draw(unsigned int shaderProgram, unsigned int modelLoc, unsigned int colorLoc)
{
    glm::mat4 modelMat = glm::translate(glm::mat4(1.0f), position);
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &modelMat[0][0]);

    if (isHit)
        glUniform3f(colorLoc, 0.0f, 1.0f, 0.0f);
    else
        glUniform3f(colorLoc, 1.0f, 0.0f, 0.0f);

    model.draw();
}

bool Enemy::checkHit(glm::vec3 rayOrigin, glm::vec3 rayDir)
{
    float t = (position.z - rayOrigin.z) / rayDir.z;
    glm::vec3 hitPoint = rayOrigin + rayDir * t;

    return (hitPoint.x > position.x - 0.5f && hitPoint.x < position.x + 0.5f &&
        hitPoint.y > position.y - 0.5f && hitPoint.y < position.y + 0.5f);
}