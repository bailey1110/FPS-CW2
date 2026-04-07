#include "Renderer.h"
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

void Renderer::init(unsigned int shader)
{
    shaderProgram = shader;

    modelLoc = glGetUniformLocation(shaderProgram, "model");
    viewLoc = glGetUniformLocation(shaderProgram, "view");
    projectionLoc = glGetUniformLocation(shaderProgram, "projection");
    textureLoc = glGetUniformLocation(shaderProgram, "texture1");
    useTextureLoc = glGetUniformLocation(shaderProgram, "useTexture");
    objectColorLoc = glGetUniformLocation(shaderProgram, "objectColor");

    lightDirLoc = glGetUniformLocation(shaderProgram, "lightDir");
    viewPosLoc = glGetUniformLocation(shaderProgram, "viewPos");

    fogColorLoc = glGetUniformLocation(shaderProgram, "fogColor");
    fogMinLoc = glGetUniformLocation(shaderProgram, "fogMin");
    fogMaxLoc = glGetUniformLocation(shaderProgram, "fogMax");

    lightPosLoc = glGetUniformLocation(shaderProgram, "lightPos");
    lightDirPlayerLoc = glGetUniformLocation(shaderProgram, "lightDirPlayer");
    flashlightOnLoc = glGetUniformLocation(shaderProgram, "flashlightOn");
}

void Renderer::setMatrices(const glm::mat4& view,
    const glm::mat4& projection,
    const glm::vec3& camPos,
    const glm::vec3& camFront,
    bool flashlightOn)
{
    glUseProgram(shaderProgram);

    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, &projection[0][0]);
    glUniform1i(textureLoc, 0);

    glm::vec3 lightDir = glm::normalize(glm::vec3(-0.5f, -1.0f, -0.3f));
    glUniform3f(lightDirLoc, lightDir.x, lightDir.y, lightDir.z);
    glUniform3f(viewPosLoc, camPos.x, camPos.y, camPos.z);

    glUniform3f(fogColorLoc, 0.0f, 0.0f, 0.0f);
    glUniform1f(fogMinLoc, 5.0f);
    glUniform1f(fogMaxLoc, 60.0f);

    glm::vec3 dir = glm::normalize(camFront);

    glm::vec3 lightPos =
        camPos
        + dir * 0.8f
        + glm::vec3(0.0f, 0.6f, 0.0f);

    glUniform3f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z);
    glUniform3f(lightDirPlayerLoc, camFront.x, camFront.y, camFront.z);
    glUniform1f(flashlightOnLoc, flashlightOn ? 1.0f : 0.0f);
}

void Renderer::drawFloor(Model& model, Texture& tex)
{
    glUniform1i(useTextureLoc, 1);
    tex.bind(0);

    const float scale = 0.25f;
    const float tileSize = 64.0f;

    for (int x = -1; x <= 1; x++)
    {
        for (int z = -1; z <= 1; z++)
        {
            glm::mat4 mat = glm::mat4(1.0f);
            mat = glm::translate(mat, glm::vec3(x * tileSize, 0.0f, z * tileSize));
            mat = glm::scale(mat, glm::vec3(scale));

            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mat[0][0]);
            model.draw();
        }
    }
}

void Renderer::drawEnemy(Enemy& enemy)
{
    enemy.draw(shaderProgram, modelLoc, useTextureLoc);
}

void Renderer::drawGun(Model& model, Texture& tex, const glm::vec3& pos, const glm::mat4& rot)
{
    glm::mat4 mat = glm::translate(glm::mat4(1.0f), pos) * rot;
    mat = glm::scale(mat, glm::vec3(0.05f));

    glUniform1i(useTextureLoc, 1);
    tex.bind(0);

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mat[0][0]);
    model.draw();
}

void Renderer::drawTracer(BulletTracer& tracer)
{
    glm::mat4 mat = glm::mat4(1.0f);
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mat[0][0]);

    glUniform1i(useTextureLoc, 0);
    glUniform3f(objectColorLoc, 1.0f, 1.0f, 1.0f);

    glDisable(GL_DEPTH_TEST);
    tracer.draw(objectColorLoc);
    glEnable(GL_DEPTH_TEST);

    glUniform1i(useTextureLoc, 1);
}