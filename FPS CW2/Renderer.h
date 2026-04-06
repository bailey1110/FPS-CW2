#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "Model.h"
#include "Texture.h"
#include "Enemy.h"
#include "BulletTracer.h"

class Renderer
{
public:
    unsigned int shaderProgram;

    GLint modelLoc;
    GLint viewLoc;
    GLint projectionLoc;
    GLint textureLoc;
    GLint useTextureLoc;
    GLint objectColorLoc;
    GLint lightDirLoc;
    GLint viewPosLoc;

    void init(unsigned int shader);

    void setMatrices(const glm::mat4& view, const glm::mat4& projection, const glm::vec3& camPos);

    void drawFloor(Model& model, Texture& tex);
    void drawEnemy(Enemy& enemy);
    void drawGun(Model& model, Texture& tex, const glm::vec3& pos, const glm::mat4& rot);
    void drawTracer(BulletTracer& tracer);
};