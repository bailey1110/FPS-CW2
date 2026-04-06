#include "UIManager.h"
#include <glad/glad.h>

void UIManager::init(Crosshair* c, ReloadUI* r, TextRenderer* t,
    unsigned int crossShader, unsigned int txtShader,
    int width, int height)
{
    crosshair = c;
    reloadUI = r;
    textRenderer = t;
    crosshairShader = crossShader;
    textShader = txtShader;

    screenWidth = width;
    screenHeight = height;
}

glm::vec2 UIManager::toNDC(float px, float py)
{
    float x = -1.0f + (px / screenWidth) * 2.0f;
    float y = 1.0f - (py / screenHeight) * 2.0f;
    return glm::vec2(x, y);
}

float UIManager::getScale(float baseSize)
{
    float referenceHeight = 1080.0f;
    return baseSize * (screenHeight / referenceHeight);
}

void UIManager::draw(int killCount)
{
    crosshair->draw(crosshairShader);
    reloadUI->draw(crosshairShader);

    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glm::vec2 pos = toNDC(10.0f, 50.0f);

    std::string text = "Crates destroyed: " + std::to_string(killCount);

    textRenderer->renderText(
        textShader,
        text,
        pos.x,
        pos.y,
        getScale(0.0015f),
        glm::vec3(1.0f)
    );

    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
}