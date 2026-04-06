#pragma once

#include "Crosshair.h"
#include "ReloadUI.h"
#include "TextRenderer.h"
#include <glm/glm.hpp>
#include <string>

class UIManager
{
public:
    Crosshair* crosshair;
    ReloadUI* reloadUI;
    TextRenderer* textRenderer;

    unsigned int crosshairShader;
    unsigned int textShader;

    int screenWidth;
    int screenHeight;

    void init(Crosshair* c, ReloadUI* r, TextRenderer* t,
        unsigned int crossShader, unsigned int txtShader,
        int width, int height);

    void draw(int killCount);

    glm::vec2 toNDC(float px, float py);
    float getScale(float baseSize);
};