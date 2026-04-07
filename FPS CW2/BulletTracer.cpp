#include "BulletTracer.h"
#include <glad/glad.h>
#include <glm/glm.hpp>

BulletTracer::BulletTracer()
{
    active = false;
    finished = false;

    travelDuration = 0.2f;
    persistTime = 0.0f;
    life = 0.0f;

    flashActive = false;
    flashTimer = 0.0f;
    flashDuration = 0.5f;
    flashSize = 1.0f; 
}

void BulletTracer::setup()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
}

void BulletTracer::spawn(glm::vec3 s, glm::vec3 e, float travelTime, float stayTime)
{
    start = s;
    targetEnd = e;
    currentEnd = s;

    travelDuration = travelTime;
    persistTime = stayTime;
    life = travelDuration;

    active = true;
    finished = false;

    flashActive = true;
    flashTimer = flashDuration;

    float verts[] = {
        start.x, start.y, start.z,
        currentEnd.x, currentEnd.y, currentEnd.z
    };

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

void BulletTracer::update(float dt)
{
    if (active)
    {
        if (!finished)
        {
            life -= dt;

            float t = 1.0f - (life / travelDuration);
            if (t < 0.0f) t = 0.0f;
            if (t > 1.0f) t = 1.0f;

            currentEnd = start + (targetEnd - start) * t;

            if (life <= 0.0f)
            {
                currentEnd = targetEnd;
                finished = true;
                life = persistTime;

                if (persistTime <= 0.0f)
                    active = false;
            }
        }
        else
        {
            life -= dt;
            if (life <= 0.0f)
                active = false;
        }

        float verts[] = {
            start.x, start.y, start.z,
            currentEnd.x, currentEnd.y, currentEnd.z
        };

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(verts), verts);
    }

    if (flashActive)
    {
        flashTimer -= dt;
        if (flashTimer <= 0.0f)
            flashActive = false;
    }
}

void BulletTracer::draw(int colorLoc)
{
    glDisable(GL_DEPTH_TEST);

    glUniform3f(colorLoc, 1.0f, 1.0f, 1.0f);

    if (active)
    {
        glLineWidth(6.0f);
        glBindVertexArray(VAO);
        glDrawArrays(GL_LINES, 0, 2);
        glLineWidth(1.0f);
    }

    if (flashActive)
    {
        float distance = glm::length(start - currentEnd);
        float size = flashSize / (distance + 0.1f);

        glPointSize(size * 100.0f);

        float flashVerts[] = {
            start.x, start.y, start.z
        };

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(flashVerts), flashVerts);

        glBindVertexArray(VAO);
        glDrawArrays(GL_POINTS, 0, 1);
    }

    glEnable(GL_DEPTH_TEST);
}