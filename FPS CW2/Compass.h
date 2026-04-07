#pragma once
#include <glm/glm.hpp>

// UI element that shows direction to an enemy relative to player view
class Compass
{
public:
    unsigned int VAO, VBO; // OpenGL buffers for drawing compass elements

    // Initialise OpenGL buffers (VAO/VBO)
    void setup();

    // Draw the compass on screen
    // shader = UI shader program
    // playerPos = player's world position
    // playerFront = direction player is facing
    // enemyPos = enemy world position (target)
    void draw(unsigned int shader, glm::vec3 playerPos, glm::vec3 playerFront, glm::vec3 enemyPos);
};