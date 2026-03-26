#include "Game.h"
#include <glm/gtc/matrix_transform.hpp>

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

    glfwSetWindowUserPointer(window, this);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    glEnable(GL_DEPTH_TEST);

    shaderProgram = createShaderProgram("vertexShader.glsl", "fragmentShader.glsl");
    crosshairShaderProgram = createShaderProgram("crosshairVertex.glsl", "crosshairFragment.glsl");

    projection = glm::perspective(glm::radians(45.0f), 1920.0f / 1080.0f, 0.1f, 100.0f);

    enemy.setup();
    environment.setup();
    crosshair.setup();
    tracer.setup();
    reloadUI.setup();
    gun.setup();

    lastTime = 0.0f;
    reloadTimer = 0.0f;
    mousePressedLastFrame = false;
}

void Game::gameLoop()
{
    while (!glfwWindowShouldClose(window))
    {
        float currentTime = glfwGetTime();
        float deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        processInput();
        updateGame(deltaTime);
        drawGame();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void Game::processInput()
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    player.processInput(window);

    bool mousePressed = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;

    if (mousePressed && !mousePressedLastFrame && reloadTimer <= 0.0f)
    {
        glm::vec3 dir = glm::normalize(player.front);
        glm::vec3 right = glm::normalize(glm::cross(dir, player.up));
        glm::vec3 camUp = glm::normalize(glm::cross(right, dir));

        // same start as gun
        glm::vec3 gunBase =
            player.position
            - dir * 0.5f
            + right * 0.2f
            - camUp * 0.2f;

        // end of barrel
        glm::vec3 muzzle = gunBase + dir * 2.0f;

        // where player is aiming from center of screen at click time
        glm::vec3 aimPoint = player.position + dir * 50.0f;

        if (enemy.checkHit(player.position, dir))
        {
            enemy.isHit = true;
            aimPoint = enemy.position;
        }

        // fire from barrel end TOWARDS crosshair aim point
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
}

void Game::drawGame()
{
    glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 view = player.getViewMatrix();

    glUseProgram(shaderProgram);

    unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
    unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
    unsigned int projectionLoc = glGetUniformLocation(shaderProgram, "projection");
    unsigned int textureLoc = glGetUniformLocation(shaderProgram, "texture1");
    unsigned int colorLoc = glGetUniformLocation(shaderProgram, "objectColor");
    unsigned int useTextureLoc = glGetUniformLocation(shaderProgram, "useTexture");

    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, &projection[0][0]);
    glUniform1i(textureLoc, 0);

    glUniform1i(useTextureLoc, 0);
    glUniform3f(colorLoc, 1.0f, 0.0f, 0.0f);
    enemy.draw(shaderProgram, modelLoc, colorLoc);

    glUniform1i(useTextureLoc, 1);
    environment.draw(modelLoc);

    glUniform1i(useTextureLoc, 0);
    glUniform3f(colorLoc, 0.2f, 0.2f, 0.2f);
    gun.draw(modelLoc, player.position, player.front, player.up);

    glm::mat4 tracerModel = glm::mat4(1.0f);
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &tracerModel[0][0]);

    glDisable(GL_DEPTH_TEST);
    tracer.draw(shaderProgram);
    glEnable(GL_DEPTH_TEST);

    crosshair.draw(crosshairShaderProgram);
    reloadUI.draw(crosshairShaderProgram);
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