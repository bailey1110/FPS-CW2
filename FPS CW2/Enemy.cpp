#include "Enemy.h"
#include <glm/gtc/matrix_transform.hpp>

// Initialise enemy with model and texture
void Enemy::setup(Model* m, Texture* t)
{
    model = m;      // Assign model
    texture = t;    // Assign texture
    isActive = true; // Enemy starts active
}

// Ray-sphere intersection test (used for bullet hit detection)
bool Enemy::checkHit(const glm::vec3& origin, const glm::vec3& dir)
{
    glm::vec3 oc = origin - position; // Vector from ray origin to sphere center

    float a = glm::dot(dir, dir); // Quadratic coefficient A
    float b = 2.0f * glm::dot(oc, dir); // Quadratic coefficient B
    float c = glm::dot(oc, oc) - radius * radius; // Quadratic coefficient C

    float discriminant = b * b - 4 * a * c; // Discriminant determines intersection

    return discriminant > 0.0f; // True if ray intersects sphere
}

// Sphere-sphere collision test (e.g. player vs enemy)
bool Enemy::isColliding(const glm::vec3& otherPos, float otherRadius)
{
    float dist = glm::length(position - otherPos); // Distance between centers
    return dist < (radius + otherRadius); // Collision if distance < sum of radii
}

// Draw the enemy
void Enemy::draw(unsigned int shader, GLint modelLoc, GLint useTextureLoc)
{
    if (!isActive) return; // Skip if enemy is inactive

    // Create model transformation (translate to position)
    glm::mat4 mat = glm::translate(glm::mat4(1.0f), position);

    glUniform1i(useTextureLoc, 1); // Enable texture usage
    texture->bind(0);              // Bind texture to unit 0

    // Send model matrix to shader
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mat[0][0]);

    model->draw(); // Render model
}