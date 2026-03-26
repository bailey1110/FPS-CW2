#include "BulletTracer.h"
#include <glad/glad.h>

BulletTracer::BulletTracer()
{
    active = false;
    finished = false;

    travelDuration = 0.2f;
    persistTime = 0.0f;
    life = 0.0f;
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
    if (!active) return;

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

void BulletTracer::draw(unsigned int shaderProgram)
{
    if (!active) return;

    glUseProgram(shaderProgram);

    int colorLoc = glGetUniformLocation(shaderProgram, "objectColor");
    glUniform3f(colorLoc, 1.0f, 1.0f, 1.0f);

    glLineWidth(6.0f);   // ← was 3.0f
    glBindVertexArray(VAO);
    glDrawArrays(GL_LINES, 0, 2);
    glLineWidth(1.0f);
}