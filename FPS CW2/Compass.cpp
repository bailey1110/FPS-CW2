#include "Compass.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Setup OpenGL buffers
void Compass::setup()
{
    glGenVertexArrays(1, &VAO); // Generate VAO
    glGenBuffers(1, &VBO);      // Generate VBO
}

// Draw compass UI pointing toward enemy
void Compass::draw(unsigned int shader, glm::vec3 playerPos, glm::vec3 playerFront, glm::vec3 enemyPos)
{
    glUseProgram(shader);        // Use UI shader
    glDisable(GL_DEPTH_TEST);    // Disable depth so UI draws on top

    // Flatten player forward direction to XZ plane (ignore vertical)
    glm::vec3 flatForward = glm::vec3(playerFront.x, 0.0f, playerFront.z);
    if (glm::length(flatForward) <= 0.0001f)
        flatForward = glm::vec3(0.0f, 0.0f, -1.0f); // Default forward if too small
    else
        flatForward = glm::normalize(flatForward);

    // Direction from player to enemy (XZ plane)
    glm::vec3 toEnemy3 = glm::vec3(enemyPos.x - playerPos.x, 0.0f, enemyPos.z - playerPos.z);
    if (glm::length(toEnemy3) <= 0.0001f)
        toEnemy3 = flatForward; // Fallback if too close
    else
        toEnemy3 = glm::normalize(toEnemy3);

    // Calculate right vector relative to player
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 flatRight = glm::normalize(glm::cross(flatForward, up));

    // Dot products to determine relative direction
    float sideDot = glm::dot(flatRight, toEnemy3);   // Left/right
    float frontDot = glm::dot(flatForward, toEnemy3); // Front/back

    // Angle between forward direction and enemy direction
    float angle = atan2f(sideDot, frontDot);

    // Compass bar settings (screen space)
    const float barHalfWidth = 0.2f;
    const float yBar = 0.90f;

    float screenX = 0.0f;

    // If enemy is behind, clamp indicator to edges
    if (frontDot <= 0.0f)
    {
        screenX = (sideDot >= 0.0f) ? barHalfWidth : -barHalfWidth;
    }
    else
    {
        // Map angle to screen position
        float normalized = angle / 1.04719755f; // ~60 degrees range
        if (normalized > 1.0f) normalized = 1.0f;
        if (normalized < -1.0f) normalized = -1.0f;
        screenX = normalized * barHalfWidth;
    }

    // Compass bar line vertices
    float barVerts[] = {
        -barHalfWidth, yBar, 0.0f,
         barHalfWidth, yBar, 0.0f
    };

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Upload bar geometry
    glBufferData(GL_ARRAY_BUFFER, sizeof(barVerts), barVerts, GL_DYNAMIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Draw compass bar
    glDrawArrays(GL_LINES, 0, 2);

    // Triangle indicator pointing to enemy direction
    float indicatorVerts[] = {
         screenX,          yBar - 0.02f, 0.0f,
         screenX - 0.012f, yBar + 0.018f, 0.0f,
         screenX + 0.012f, yBar + 0.018f, 0.0f
    };

    // Upload indicator geometry
    glBufferData(GL_ARRAY_BUFFER, sizeof(indicatorVerts), indicatorVerts, GL_DYNAMIC_DRAW);

    // Draw indicator triangle
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glEnable(GL_DEPTH_TEST); // Re-enable depth testing
}