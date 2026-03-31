// Enemy.cpp

#include "Enemy.h"
#include <glm/gtc/matrix_transform.hpp>

void Enemy::setup(Model* m, Texture* t)
{
    model = m;
    texture = t;

    position = glm::vec3(0.0f, 0.0f, -5.0f);
    isActive = true;
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

    float t = (position.z - rayOrigin.z) / rayDir.z;
    glm::vec3 hitPoint = rayOrigin + rayDir * t;

    bool hit =
        (hitPoint.x > position.x - 0.5f && hitPoint.x < position.x + 0.5f &&
            hitPoint.y > position.y - 0.5f && hitPoint.y < position.y + 0.5f);

    if (hit)
        isActive = false;

    return hit;
}