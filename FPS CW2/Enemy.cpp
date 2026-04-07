#include "Enemy.h"
#include <glm/gtc/matrix_transform.hpp>

void Enemy::setup(Model* m, Texture* t)
{
    model = m;
    texture = t;
    isActive = true;
}

bool Enemy::checkHit(const glm::vec3& origin, const glm::vec3& dir)
{
    glm::vec3 oc = origin - position;

    float a = glm::dot(dir, dir);
    float b = 2.0f * glm::dot(oc, dir);
    float c = glm::dot(oc, oc) - radius * radius;

    float discriminant = b * b - 4 * a * c;

    return discriminant > 0.0f;
}

bool Enemy::isColliding(const glm::vec3& otherPos, float otherRadius)
{
    float dist = glm::length(position - otherPos);
    return dist < (radius + otherRadius);
}

void Enemy::draw(unsigned int shader, GLint modelLoc, GLint useTextureLoc)
{
    if (!isActive) return;

    glm::mat4 mat = glm::translate(glm::mat4(1.0f), position);

    glUniform1i(useTextureLoc, 1);
    texture->bind(0);

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mat[0][0]);
    model->draw();
}