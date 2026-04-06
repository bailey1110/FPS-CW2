#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "Player.h"
#include "Enemy.h"
#include "Crosshair.h"
#include "BulletTracer.h"
#include "ReloadUI.h"
#include "Shader.h"
#include "Model.h"
#include "Texture.h"
#include "Compass.h"

class Game
{
public:
    GLFWwindow* window;

    Player player;
    Enemy enemy;
    Crosshair crosshair;
    BulletTracer tracer;
    ReloadUI reloadUI;
    Compass compass;

    Model importedFloorModel;
    Model importedCrateModel;
    Model importedGunModel;

    Texture importedFloorTexture;
    Texture importedCrateTexture;
    Texture importedGunTexture;

    unsigned int shaderProgram;
    unsigned int crosshairShaderProgram;

    GLint modelLoc;
    GLint viewLoc;
    GLint projectionLoc;
    GLint textureLoc;
    GLint useTextureLoc;
    GLint objectColorLoc;

    glm::mat4 projection;

    float lastTime;
    float reloadTimer;
    const float reloadDuration = 1.0f;

    bool mousePressedLastFrame;

    int killCount;

    // NEW (added, nothing removed)
    float enemyRespawnTimer;

    void run();

private:
    void initSystems();
    void gameLoop();
    void processInput(float deltaTime);
    void updateGame(float deltaTime);
    void drawGame();

    void drawFloor();
    void drawEnemy();
    void drawGun();
    void drawTracer();
    void drawUI();

    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
};