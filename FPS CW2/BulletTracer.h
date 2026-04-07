#pragma once
#include <glm/glm.hpp>

// Represents a visual tracer for a fired bullet (line + muzzle flash)
class BulletTracer
{
public:
    glm::vec3 start;       // Starting position (usually gun muzzle)
    glm::vec3 targetEnd;   // Final hit position
    glm::vec3 currentEnd;  // Current interpolated position during travel

    float life;            // Remaining time for current state
    float travelDuration;  // Time it takes to reach target
    float persistTime;     // Time to remain visible after reaching target

    bool active;           // Whether tracer is currently active/visible
    bool finished;         // Whether tracer has reached its target

    unsigned int VAO;      // OpenGL Vertex Array Object
    unsigned int VBO;      // OpenGL Vertex Buffer Object

    // Muzzle flash properties
    bool flashActive;      // Whether flash is visible
    float flashTimer;      // Remaining time for flash
    float flashDuration;   // Total duration of flash
    float flashSize;       // Size of flash effect

    // Constructor: initialises tracer state
    BulletTracer();

    // Initialise OpenGL buffers
    void setup();

    // Start a new tracer effect
    // s = start position, e = end position
    // travelTime = how long it takes to reach target
    // stayTime = how long it remains after impact
    void spawn(glm::vec3 s, glm::vec3 e, float travelTime, float stayTime);

    // Update tracer movement and timing
    void update(float deltaTime);

    // Render tracer and flash
    void draw(int colorLoc);
};