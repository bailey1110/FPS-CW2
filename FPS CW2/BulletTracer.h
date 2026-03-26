#pragma once
#include <glm/glm.hpp>

class BulletTracer
{
public:
    glm::vec3 start;
    glm::vec3 targetEnd;
    glm::vec3 currentEnd;

    float life;
    float travelDuration;
    float persistTime;

    bool active;
    bool finished;

    unsigned int VAO;
    unsigned int VBO;

    BulletTracer();

    void setup();
    void spawn(glm::vec3 s, glm::vec3 e, float travelTime, float stayTime);
    void update(float deltaTime);
    void draw(unsigned int shaderProgram);
};