#include "BulletTracer.h"
#include <glad/glad.h>
#include <glm/glm.hpp>

// Constructor: initialise tracer state and default values
BulletTracer::BulletTracer()
{
    active = false;     // Tracer is initially inactive
    finished = false;   // Has not reached target

    travelDuration = 0.2f; // Default travel time
    persistTime = 0.0f;    // No persistence after impact by default
    life = 0.0f;           // Remaining lifetime

    flashActive = false;   // Flash initially off
    flashTimer = 0.0f;     // Flash timer
    flashDuration = 0.5f;  // Flash duration
    flashSize = 1.0f;      // Flash size scaling
}

// Setup OpenGL buffers
void BulletTracer::setup()
{
    glGenVertexArrays(1, &VAO); // Generate VAO
    glGenBuffers(1, &VBO);      // Generate VBO
}

// Spawn a new tracer from start (s) to end (e)
void BulletTracer::spawn(glm::vec3 s, glm::vec3 e, float travelTime, float stayTime)
{
    start = s;          // Set start position
    targetEnd = e;      // Set target position
    currentEnd = s;     // Initial end = start

    travelDuration = travelTime; // Time to reach target
    persistTime = stayTime;      // Time to remain after reaching target
    life = travelDuration;       // Reset lifetime

    active = true;      // Activate tracer
    finished = false;   // Not yet reached target

    flashActive = true;         // Enable muzzle flash
    flashTimer = flashDuration; // Reset flash timer

    // Initial line vertices (start → currentEnd)
    float verts[] = {
        start.x, start.y, start.z,
        currentEnd.x, currentEnd.y, currentEnd.z
    };

    glBindVertexArray(VAO);                 // Bind VAO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);     // Bind VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_DYNAMIC_DRAW); // Upload data

    // Position attribute (vec3)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

// Update tracer position and lifetime
void BulletTracer::update(float dt)
{
    if (active)
    {
        if (!finished)
        {
            life -= dt; // Decrease travel time

            // Interpolation factor (0 → 1)
            float t = 1.0f - (life / travelDuration);
            if (t < 0.0f) t = 0.0f;
            if (t > 1.0f) t = 1.0f;

            // Move current end along the line
            currentEnd = start + (targetEnd - start) * t;

            // Reached target
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
            // After reaching target, countdown persistence
            life -= dt;
            if (life <= 0.0f)
                active = false;
        }

        // Update line vertex data
        float verts[] = {
            start.x, start.y, start.z,
            currentEnd.x, currentEnd.y, currentEnd.z
        };

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(verts), verts);
    }

    // Update flash timer
    if (flashActive)
    {
        flashTimer -= dt;
        if (flashTimer <= 0.0f)
            flashActive = false;
    }
}

// Render tracer line and muzzle flash
void BulletTracer::draw(int colorLoc)
{
    glDisable(GL_DEPTH_TEST); // Ensure tracer renders on top

    glUniform3f(colorLoc, 1.0f, 1.0f, 1.0f); // Set tracer colour (white)

    // Draw line tracer
    if (active)
    {
        glLineWidth(6.0f);           // Thicker line
        glBindVertexArray(VAO);
        glDrawArrays(GL_LINES, 0, 2); // Draw line (2 vertices)
        glLineWidth(1.0f);           // Reset line width
    }

    // Draw muzzle flash
    if (flashActive)
    {
        float distance = glm::length(start - currentEnd); // Distance from start to current end
        float size = flashSize / (distance + 0.1f);       // Scale flash size

        glPointSize(size * 100.0f); // Set point size

        float flashVerts[] = {
            start.x, start.y, start.z
        };

        // Update buffer to contain single point
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(flashVerts), flashVerts);

        glBindVertexArray(VAO);
        glDrawArrays(GL_POINTS, 0, 1); // Draw flash point
    }

    glEnable(GL_DEPTH_TEST); // Re-enable depth testing
}