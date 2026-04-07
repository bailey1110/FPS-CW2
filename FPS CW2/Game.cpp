#define NOMINMAX
#include <windows.h>

#include "Game.h"
#include <glm/gtc/matrix_transform.hpp>
#include <cstdlib>
#include <ctime>
#include <string>
#include <thread>
#include <chrono>
#include <atomic>
#include <algorithm>
#include <limits>
#include <cmath>
#include <mmsystem.h>

#pragma comment(lib, "winmm.lib")

static void PlayOverlapSound(const std::wstring& filePath)
{
    static std::atomic<int> soundCounter = 0;
    std::wstring alias = L"snd_" + std::to_wstring(soundCounter++);

    std::wstring openCmd = L"open \"" + filePath + L"\" type waveaudio alias " + alias;
    mciSendStringW(openCmd.c_str(), NULL, 0, NULL);

    std::wstring playCmd = L"play " + alias;
    mciSendStringW(playCmd.c_str(), NULL, 0, NULL);

    std::thread([alias]()
        {
            std::this_thread::sleep_for(std::chrono::seconds(5));
            std::wstring closeCmd = L"close " + alias;
            mciSendStringW(closeCmd.c_str(), NULL, 0, NULL);
        }).detach();
}

void Game::run()
{
    initSystems();
    gameLoop();
}

void Game::initSystems()
{
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(1920, 1080, "FPS CW2", NULL, NULL);
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);

    glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    glEnable(GL_DEPTH_TEST);

    shaderProgram = createShaderProgram("vertexShader.glsl", "fragmentShader.glsl");
    crosshairShaderProgram = createShaderProgram("crosshairVertex.glsl", "crosshairFragment.glsl");
    textShader = createShaderProgram("textVertex.glsl", "textFragment.glsl");

    renderer.init(shaderProgram);

    projection = glm::perspective(glm::radians(45.0f), 1920.0f / 1080.0f, 0.1f, 100.0f);

    crosshair.setup();
    tracer.setup();
    reloadUI.setup();
    compass.setup();

    textRenderer.init("C:/Windows/Fonts/arial.ttf", textShader);
    uiManager.init(&crosshair, &reloadUI, &textRenderer, crosshairShaderProgram, textShader, 1920, 1080);

    importedFloorModel.loadOBJ("Models/Floor.obj");
    importedCrateModel.loadOBJ("Models/crate obj.obj");
    importedGunModel.loadOBJ("Models/gun.obj");

    importedFloorTexture.load("Models/floor-diffuse-texture.png");
    importedCrateTexture.load("Models/crate_texture.png");
    importedGunTexture.load("Models/gun_texture.png");

    enemies.clear();
    spawnEnemies(1);

    lastTime = 0.0f;
    reloadTimer = 0.0f;
    mousePressedLastFrame = false;
    killCount = 0;

    player.position.y = 1.5f;
}

void Game::gameLoop()
{
    while (!glfwWindowShouldClose(window))
    {
        float currentTime = static_cast<float>(glfwGetTime());
        float deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        processInput(deltaTime);
        updateGame(deltaTime);
        drawGame();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void Game::processInput(float deltaTime)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    player.processInput(window, deltaTime);

    bool mousePressed = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;

    if (mousePressed && !mousePressedLastFrame && reloadTimer <= 0.0f)
    {
        glm::vec3 dir = glm::normalize(player.front);
        glm::vec3 right = glm::normalize(glm::cross(dir, player.up));
        glm::vec3 camUp = glm::normalize(glm::cross(right, dir));

        glm::vec3 gunBase =
            player.position
            - dir * 0.5f
            + right * 0.2f
            - camUp * 0.2f;

        glm::vec3 muzzle = gunBase + dir * 2.0f;
        glm::vec3 aimPoint = player.position + dir * 50.0f;

        PlayOverlapSound(L".\\gun.wav");

        int hitIndex = -1;
        float closestDist2 = std::numeric_limits<float>::max();

        for (int i = 0; i < (int)enemies.size(); i++)
        {
            Enemy& enemy = enemies[i];
            if (!enemy.isActive)
                continue;

            glm::vec3 offset = enemy.position - player.position;
            float nearDist2 = glm::dot(offset, offset);
            if (nearDist2 > 2500.0f)
                continue;

            if (enemy.checkHit(player.position, dir))
            {
                if (nearDist2 < closestDist2)
                {
                    closestDist2 = nearDist2;
                    hitIndex = i;
                }
            }
        }

        if (hitIndex != -1)
        {
            aimPoint = enemies[hitIndex].position;
            enemies[hitIndex].isActive = false;
            killCount++;
            PlayOverlapSound(L".\\crate.wav");
        }

        tracer.spawn(muzzle, aimPoint, 0.15f, 1.0f);
        reloadTimer = reloadDuration;
    }

    mousePressedLastFrame = mousePressed;
}

void Game::updateGame(float deltaTime)
{
    tracer.update(deltaTime);

    if (reloadTimer > 0.0f)
        reloadTimer -= deltaTime;

    if (reloadTimer < 0.0f)
        reloadTimer = 0.0f;

    float reloadProgress = 1.0f - (reloadTimer / reloadDuration);
    reloadUI.update(reloadProgress);

    for (Enemy& enemy : enemies)
    {
        if (!enemy.isActive)
            continue;

        glm::vec3 enemyDelta = enemy.position - player.position;
        if (glm::dot(enemyDelta, enemyDelta) > 100.0f)
            continue;

        const float minDistXZ = 1.5f;

        glm::vec2 playerXZ(player.position.x, player.position.z);
        glm::vec2 enemyXZ(enemy.position.x, enemy.position.z);

        glm::vec2 diff = playerXZ - enemyXZ;
        float dist = glm::length(diff);

        if (dist < minDistXZ)
        {
            if (dist < 0.0001f)
                diff = glm::vec2(0.0f, 1.0f);

            glm::vec2 pushDir = glm::normalize(diff);
            glm::vec2 corrected = enemyXZ + pushDir * minDistXZ;

            player.position.x = corrected.x;
            player.position.z = corrected.y;
        }
    }

    for (int i = 0; i < (int)enemies.size();)
    {
        if (!enemies[i].isActive)
        {
            enemies[i] = enemies.back();
            enemies.pop_back();
        }
        else
        {
            i++;
        }
    }

    int targetCount = 1;
    if (killCount >= 3)
        targetCount = 5;
    else if (killCount >= 1)
        targetCount = 3;

    if ((int)enemies.size() < targetCount)
        spawnEnemies(targetCount - (int)enemies.size());
}

void Game::drawGame()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 view = player.getViewMatrix();
    bool flashlight = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS;

    glUseProgram(shaderProgram);
    renderer.setMatrices(view, projection, player.position, player.front, flashlight);

    renderer.drawFloor(importedFloorModel, importedFloorTexture);

    for (Enemy& enemy : enemies)
        renderer.drawEnemy(enemy);

    glm::vec3 dir = player.front;
    glm::vec3 right = glm::cross(dir, player.up);
    glm::vec3 up = glm::cross(right, dir);

    dir = glm::normalize(dir);
    right = glm::normalize(right);
    up = glm::normalize(up);

    glm::vec3 gunPos =
        player.position
        - dir * 0.5f
        + right * 0.2f
        - up * 0.2f
        + dir * 1.25f;

    glm::mat4 rot = glm::mat4(1.0f);
    rot[0] = glm::vec4(right, 0.0f);
    rot[1] = glm::vec4(up, 0.0f);
    rot[2] = glm::vec4(dir, 0.0f);

    renderer.drawGun(importedGunModel, importedGunTexture, gunPos, rot);
    renderer.drawTracer(tracer);

    bool found = false;
    glm::vec3 target;
    float closest = std::numeric_limits<float>::max();

    for (const Enemy& enemy : enemies)
    {
        glm::vec3 d = enemy.position - player.position;
        float dist2 = glm::dot(d, d);

        if (dist2 < closest)
        {
            closest = dist2;
            target = enemy.position;
            found = true;
        }
    }

    if (found)
        compass.draw(crosshairShaderProgram, player.position, player.front, target);

    uiManager.draw(killCount);
}

void Game::spawnEnemies(int count)
{
    const float spawnRadius = 50.0f;
    const float minPlayerDist = 3.0f;
    const float minEnemyDist = 3.0f;
    const int maxEnemies = 5;

    int available = maxEnemies - (int)enemies.size();
    if (available <= 0) return;
    if (count > available) count = available;

    for (int i = 0; i < count; i++)
    {
        Enemy enemy;
        enemy.setup(&importedCrateModel, &importedCrateTexture);

        bool valid = false;
        int attempts = 0;

        while (!valid && attempts < 500)
        {
            attempts++;

            float angle = (float)rand() / RAND_MAX * 6.2831853f;
            float radius = (float)rand() / RAND_MAX * spawnRadius;

            glm::vec3 pos(
                player.position.x + cos(angle) * radius,
                0.0f,
                player.position.z + sin(angle) * radius
            );

            valid = true;

            glm::vec2 p(player.position.x, player.position.z);
            glm::vec2 n(pos.x, pos.z);

            if (glm::length(n - p) < minPlayerDist)
                valid = false;

            for (const Enemy& other : enemies)
            {
                glm::vec2 o(other.position.x, other.position.z);
                if (glm::length(n - o) < minEnemyDist)
                {
                    valid = false;
                    break;
                }
            }

            if (valid)
                enemy.position = pos;
        }

        enemies.push_back(enemy);
    }
}

void Game::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void Game::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    Game* game = (Game*)glfwGetWindowUserPointer(window);
    if (game)
        game->player.mouseLook(xpos, ypos);
}