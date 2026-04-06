#include "Game.h"
#include <glm/gtc/matrix_transform.hpp>
#include <cstdlib>
#include <windows.h>
#include <mmsystem.h>
#include <string>
#include <thread>
#include <chrono>
#include <atomic>

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

    // NEW TEXT SHADER
    textShader = createShaderProgram("textVertex.glsl", "textFragment.glsl");

    modelLoc = glGetUniformLocation(shaderProgram, "model");
    viewLoc = glGetUniformLocation(shaderProgram, "view");
    projectionLoc = glGetUniformLocation(shaderProgram, "projection");
    textureLoc = glGetUniformLocation(shaderProgram, "texture1");
    useTextureLoc = glGetUniformLocation(shaderProgram, "useTexture");
    objectColorLoc = glGetUniformLocation(shaderProgram, "objectColor");

    projection = glm::perspective(glm::radians(45.0f), 1920.0f / 1080.0f, 0.1f, 100.0f);

    crosshair.setup();
    tracer.setup();
    reloadUI.setup();
    compass.setup();

    // INIT TEXT RENDERER
    textRenderer.init("C:/Windows/Fonts/arial.ttf");

    importedFloorModel.loadOBJ("Models/Floor.obj");
    importedCrateModel.loadOBJ("Models/crate obj.obj");
    importedGunModel.loadOBJ("Models/gun.obj");

    importedFloorTexture.load("Models/floor-diffuse-texture.png");
    importedCrateTexture.load("Models/crate_texture.png");
    importedGunTexture.load("Models/gun_texture.png");

    enemy.setup(&importedCrateModel, &importedCrateTexture);

    lastTime = 0.0f;
    reloadTimer = 0.0f;
    mousePressedLastFrame = false;

    killCount = 0;
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

        if (enemy.checkHit(player.position, dir))
        {
            aimPoint = enemy.position;
            killCount++;
        }

        tracer.spawn(muzzle, aimPoint, 0.15f, 1.0f);

        PlayOverlapSound(L".\\gun.wav");

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

    if (!enemy.isActive)
    {
        enemy.position = glm::vec3(
            (rand() % 5) - 2,
            0.0f,
            -(rand() % 10 + 3)
        );
        enemy.isActive = true;
    }
}

void Game::drawGame()
{
    glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 view = player.getViewMatrix();

    glUseProgram(shaderProgram);

    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, &projection[0][0]);
    glUniform1i(textureLoc, 0);

    glm::vec3 lightDirection = glm::normalize(glm::vec3(-0.5f, -1.0f, -0.3f));

    glUniform3f(glGetUniformLocation(shaderProgram, "lightDir"),
        lightDirection.x, lightDirection.y, lightDirection.z);

    glUniform3f(glGetUniformLocation(shaderProgram, "viewPos"),
        player.position.x, player.position.y, player.position.z);

    drawFloor();
    drawEnemy();
    drawGun();
    drawTracer();

    compass.draw(crosshairShaderProgram, player.position, player.front, enemy.position);

    drawUI();
}

void Game::drawFloor()
{
    glUniform1i(useTextureLoc, 1);

    glm::mat4 floorModelMat = glm::mat4(1.0f);
    floorModelMat = glm::scale(floorModelMat, glm::vec3(0.25f));

    importedFloorTexture.bind(0);
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &floorModelMat[0][0]);
    importedFloorModel.draw();
}

void Game::drawEnemy()
{
    enemy.draw(shaderProgram, modelLoc, useTextureLoc);
}

void Game::drawGun()
{
    glm::vec3 dir = glm::normalize(player.front);
    glm::vec3 right = glm::normalize(glm::cross(dir, player.up));
    glm::vec3 camUp = glm::normalize(glm::cross(right, dir));

    glm::vec3 gunPos =
        player.position
        - dir * 0.5f
        + right * 0.2f
        - camUp * 0.2f
        + dir * 1.25f;

    glm::mat4 gunRotation = glm::mat4(1.0f);
    gunRotation[0] = glm::vec4(right, 0.0f);
    gunRotation[1] = glm::vec4(camUp, 0.0f);
    gunRotation[2] = glm::vec4(dir, 0.0f);

    glm::mat4 gunModelMat = glm::translate(glm::mat4(1.0f), gunPos) * gunRotation;
    gunModelMat = glm::scale(gunModelMat, glm::vec3(0.05f));

    glUniform3f(glGetUniformLocation(shaderProgram, "lightDir"), 0.0f, -1.0f, 0.0f);

    glUniform1i(useTextureLoc, 1);
    importedGunTexture.bind(0);

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &gunModelMat[0][0]);
    importedGunModel.draw();

    glm::vec3 lightDirection = glm::normalize(glm::vec3(-0.5f, -1.0f, -0.3f));
    glUniform3f(glGetUniformLocation(shaderProgram, "lightDir"),
        lightDirection.x, lightDirection.y, lightDirection.z);
}

void Game::drawTracer()
{
    glm::mat4 tracerModel = glm::mat4(1.0f);
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &tracerModel[0][0]);

    glUniform1i(useTextureLoc, 0);
    glUniform3f(objectColorLoc, 1.0f, 1.0f, 1.0f);

    glDisable(GL_DEPTH_TEST);
    tracer.draw(objectColorLoc);
    glEnable(GL_DEPTH_TEST);

    glUniform1i(useTextureLoc, 1);
}

void Game::drawUI()
{
    crosshair.draw(crosshairShaderProgram);
    reloadUI.draw(crosshairShaderProgram);

    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    std::string text = "Crates destroyed: " + std::to_string(killCount);
    textRenderer.renderText(textShader, text, -0.95f, 0.9f, 0.0015f, glm::vec3(1.0f, 1.0f, 1.0f));

    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
}

void Game::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void Game::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    Game* game = static_cast<Game*>(glfwGetWindowUserPointer(window));
    if (game)
        game->player.mouseLook(xpos, ypos);
}