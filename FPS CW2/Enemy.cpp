// Enemy.cpp

#include "Enemy.h"
#include <glm/gtc/matrix_transform.hpp>

void Enemy::setup(Model* m, Texture* t)
{
    model = m;
    texture = t;

    position = glm::vec3(0.0f, 0.0f, -5.0f);
    isActive = true;

    radius = 0.75f;
}

void Enemy::draw(unsigned int shaderProgram, unsigned int modelLoc, unsigned int useTextureLoc)
{
    if (!isActive) return;

    glm::mat4 modelMat = glm::translate(glm::mat4(1.0f), position);

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &modelMat[0][0]);

    glUniform1i(useTextureLoc, 1);
    texture->bind(0);

    model->draw();
}

bool Enemy::checkHit(glm::vec3 rayOrigin, glm::vec3 rayDir)
{
    if (!isActive) return false;

    glm::vec3 oc = rayOrigin - position;

    float a = glm::dot(rayDir, rayDir);
    float b = 2.0f * glm::dot(oc, rayDir);
    float c = glm::dot(oc, oc) - radius * radius;

    float discriminant = b * b - 4 * a * c;

    if (discriminant < 0.0f)
        return false;

    isActive = false;
    return true;
}