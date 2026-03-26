// Game.h
#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "Player.h"
#include "Enemy.h"
#include "Environment.h"
#include "Crosshair.h"
#include "BulletTracer.h"
#include "ReloadUI.h"
#include "Shader.h"
#include "Gun.h"

class Game
{
public:
    GLFWwindow* window;

    Player player;
    Enemy enemy;
    Environment environment;
    Crosshair crosshair;
    BulletTracer tracer;
    ReloadUI reloadUI;
    Gun gun;

    unsigned int shaderProgram;
    unsigned int crosshairShaderProgram;

    glm::mat4 projection;

    float lastTime;
    float reloadTimer;
    const float reloadDuration = 1.0f;

    bool mousePressedLastFrame;

    void run();

private:
    void initSystems();
    void gameLoop();
    void processInput();
    void updateGame(float deltaTime);
    void drawGame();

    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
};