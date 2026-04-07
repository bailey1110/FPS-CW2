#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>

#include "Player.h"
#include "Enemy.h"
#include "Crosshair.h"
#include "BulletTracer.h"
#include "ReloadUI.h"
#include "Shader.h"
#include "Model.h"
#include "Texture.h"
#include "Compass.h"
#include "TextRenderer.h"
#include "UIManager.h"
#include "Renderer.h"

// Main game class that controls the entire application
class Game
{
public:
    GLFWwindow* window; // Pointer to the GLFW window

    Player player; // Player object (camera + movement)

    std::vector<Enemy> enemies; // List of active enemies in the game

    Crosshair crosshair;     // UI crosshair
    BulletTracer tracer;     // Bullet tracer effect
    ReloadUI reloadUI;       // Reload progress UI
    Compass compass;         // Direction indicator for enemies

    TextRenderer textRenderer; // Handles text rendering (UI text)
    UIManager uiManager;       // Manages UI drawing
    Renderer renderer;         // Handles all 3D rendering

    Model importedFloorModel; // Floor model
    Model importedCrateModel; // Enemy model (crate)
    Model importedGunModel;   // Gun model

    Texture importedFloorTexture; // Floor texture
    Texture importedCrateTexture; // Enemy texture
    Texture importedGunTexture;   // Gun texture

    unsigned int shaderProgram;          // Main 3D shader
    unsigned int crosshairShaderProgram; // UI shader (crosshair, compass, etc.)
    unsigned int textShader;             // Shader used for text rendering

    glm::mat4 projection; // Projection matrix (perspective)

    float lastTime;     // Time of previous frame (for delta time)
    float reloadTimer;  // Current reload timer
    const float reloadDuration = 1.0f; // Time required to reload

    bool mousePressedLastFrame; // Tracks mouse state for single-click detection

    int killCount; // Number of enemies eliminated

    // Starts the game (entry point after main)
    void run();

private:
    // Initialise window, OpenGL, assets, UI, etc.
    void initSystems();

    // Main loop (input → update → render)
    void gameLoop();

    // Handle keyboard and mouse input
    void processInput(float deltaTime);

    // Update all game logic
    void updateGame(float deltaTime);

    // Render the scene and UI
    void drawGame();

    // Spawn a given number of enemies
    void spawnEnemies(int count);

    // GLFW callback: window resize
    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);

    // GLFW callback: mouse movement
    static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
};