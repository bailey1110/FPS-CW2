#include "Renderer.h"
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

// Initialise renderer and cache uniform locations
void Renderer::init(unsigned int shader)
{
    shaderProgram = shader; // Store shader program

    glUseProgram(shaderProgram);

    // Get uniform locations from shader
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

    // Set static uniforms
    glUniform1i(textureLoc, 0); // Texture unit 0
    glUniform3f(fogColorLoc, 0.0f, 0.0f, 0.0f); // Black fog
    glUniform1f(fogMinLoc, 5.0f);  // Fog start distance
    glUniform1f(fogMaxLoc, 60.0f); // Fog end distance
}

// Update matrices and lighting uniforms each frame
void Renderer::setMatrices(const glm::mat4& view,
    const glm::mat4& projection,
    const glm::vec3& camPos,
    const glm::vec3& camFront,
    bool flashlightOn)
{
    // Send view and projection matrices
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, &projection[0][0]);

    // Directional light (static)
    glm::vec3 lightDir = glm::normalize(glm::vec3(-0.5f, -1.0f, -0.3f));
    glUniform3f(lightDirLoc, lightDir.x, lightDir.y, lightDir.z);

    // Camera position
    glUniform3f(viewPosLoc, camPos.x, camPos.y, camPos.z);

    // Flashlight direction (player view)
    glm::vec3 dir = glm::normalize(camFront);

    // Flashlight position slightly in front of camera
    glm::vec3 lightPos =
        camPos
        + dir * 1.4f
        + glm::vec3(0.0f, 1.0f, 0.0f);

    // Send flashlight data
    glUniform3f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z);
    glUniform3f(lightDirPlayerLoc, dir.x, dir.y, dir.z);
    glUniform1f(flashlightOnLoc, flashlightOn ? 1.0f : 0.0f);

    glUniform3f(viewPosLoc, camPos.x, camPos.y, camPos.z); // (redundant but safe)
}

// Draw tiled floor
void Renderer::drawFloor(Model& model, Texture& tex)
{
    glUniform1i(useTextureLoc, 1); // Enable texture
    tex.bind(0);                  // Bind texture

    const float scale = 0.25f;
    const float tileSize = 64.0f;

    // Draw 3x3 grid of floor tiles
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

// Draw enemy via its own draw function
void Renderer::drawEnemy(Enemy& enemy)
{
    enemy.draw(shaderProgram, modelLoc, useTextureLoc);
}

// Draw player's gun model
void Renderer::drawGun(Model& model, Texture& tex, const glm::vec3& pos, const glm::mat4& rot)
{
    // Combine translation and rotation
    glm::mat4 mat = glm::translate(glm::mat4(1.0f), pos) * rot;
    mat = glm::scale(mat, glm::vec3(0.05f)); // Scale gun down

    glUniform1i(useTextureLoc, 1);
    tex.bind(0);

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mat[0][0]);
    model.draw();
}

// Draw bullet tracer (line + flash)
void Renderer::drawTracer(BulletTracer& tracer)
{
    glm::mat4 mat = glm::mat4(1.0f); // No transform
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mat[0][0]);

    glUniform1i(useTextureLoc, 0); // Disable texture
    glUniform3f(objectColorLoc, 1.0f, 1.0f, 1.0f); // White colour

    glDisable(GL_DEPTH_TEST); // Render on top
    tracer.draw(objectColorLoc);
    glEnable(GL_DEPTH_TEST);

    glUniform1i(useTextureLoc, 1); // Restore texture usage
}