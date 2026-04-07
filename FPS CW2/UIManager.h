#pragma once

#include "Crosshair.h"
#include "ReloadUI.h"
#include "TextRenderer.h"
#include <glm/glm.hpp>
#include <string>

// Manages all UI elements (crosshair, reload bar, and on-screen text)
class UIManager
{
public:
    Crosshair* crosshair;        // Pointer to crosshair UI
    ReloadUI* reloadUI;          // Pointer to reload bar UI
    TextRenderer* textRenderer;  // Pointer to text rendering system

    unsigned int crosshairShader; // Shader used for crosshair and basic UI
    unsigned int textShader;      // Shader used for rendering text

    int screenWidth;  // Screen width in pixels
    int screenHeight; // Screen height in pixels

    // Initialise UI system with references and screen dimensions
    void init(Crosshair* c, ReloadUI* r, TextRenderer* t,
        unsigned int crossShader, unsigned int txtShader,
        int width, int height);

    // Draw all UI elements (crosshair, reload bar, text)
    void draw(int killCount);

    // Convert pixel coordinates to Normalised Device Coordinates (-1 to 1)
    glm::vec2 toNDC(float px, float py);

    // Scale UI elements based on screen size
    float getScale(float baseSize);
};