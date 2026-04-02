// Compass.cpp

#include "Compass.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

void Compass::setup()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
}

void Compass::draw(unsigned int shader, glm::vec3 playerPos, glm::vec3 playerFront, glm::vec3 enemyPos)
{
    glUseProgram(shader);
    glDisable(GL_DEPTH_TEST);

    glm::vec3 flatForward = glm::vec3(playerFront.x, 0.0f, playerFront.z);
    if (glm::length(flatForward) <= 0.0001f)
        flatForward = glm::vec3(0.0f, 0.0f, -1.0f);
    else
        flatForward = glm::normalize(flatForward);

    glm::vec3 toEnemy3 = glm::vec3(enemyPos.x - playerPos.x, 0.0f, enemyPos.z - playerPos.z);
    if (glm::length(toEnemy3) <= 0.0001f)
        toEnemy3 = flatForward;
    else
        toEnemy3 = glm::normalize(toEnemy3);

    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 flatRight = glm::normalize(glm::cross(flatForward, up));

    float sideDot = glm::dot(flatRight, toEnemy3);
    float frontDot = glm::dot(flatForward, toEnemy3);

    float angle = atan2f(sideDot, frontDot);

    const float barHalfWidth = 0.2f;
    const float yBar = 0.90f;

    float screenX = 0.0f;

    if (frontDot <= 0.0f)
    {
        screenX = (sideDot >= 0.0f) ? barHalfWidth : -barHalfWidth;
    }
    else
    {
        float normalized = angle / 1.04719755f; // ~60 degrees
        if (normalized > 1.0f) normalized = 1.0f;
        if (normalized < -1.0f) normalized = -1.0f;
        screenX = normalized * barHalfWidth;
    }

    float barVerts[] = {
        -barHalfWidth, yBar, 0.0f,
         barHalfWidth, yBar, 0.0f
    };

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(barVerts), barVerts, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glDrawArrays(GL_LINES, 0, 2);

    float indicatorVerts[] = {
         screenX,          yBar - 0.02f, 0.0f,
         screenX - 0.012f, yBar + 0.018f, 0.0f,
         screenX + 0.012f, yBar + 0.018f, 0.0f
    };

    glBufferData(GL_ARRAY_BUFFER, sizeof(indicatorVerts), indicatorVerts, GL_DYNAMIC_DRAW);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glEnable(GL_DEPTH_TEST);
}