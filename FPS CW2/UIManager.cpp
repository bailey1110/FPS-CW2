#include "UIManager.h"
#include <glad/glad.h>

// Initialise UI manager with components and screen size
void UIManager::init(Crosshair* c, ReloadUI* r, TextRenderer* t,
    unsigned int crossShader, unsigned int txtShader,
    int width, int height)
{
    crosshair = c;         // Assign crosshair pointer
    reloadUI = r;          // Assign reload UI pointer
    textRenderer = t;      // Assign text renderer pointer
    crosshairShader = crossShader; // Shader for UI elements
    textShader = txtShader;        // Shader for text

    screenWidth = width;   // Store screen width
    screenHeight = height; // Store screen height
}

// Convert pixel coordinates to Normalised Device Coordinates (-1 to 1)
glm::vec2 UIManager::toNDC(float px, float py)
{
    float x = -1.0f + (px / screenWidth) * 2.0f;  // Map X from [0,width] → [-1,1]
    float y = 1.0f - (py / screenHeight) * 2.0f;  // Map Y from [0,height] → [1,-1]
    return glm::vec2(x, y);
}

// Scale UI based on screen height (maintains consistent size)
float UIManager::getScale(float baseSize)
{
    float referenceHeight = 1080.0f; // Reference resolution
    return baseSize * (screenHeight / referenceHeight); // Scale proportionally
}

// Draw all UI elements
void UIManager::draw(int killCount)
{
    crosshair->draw(crosshairShader); // Draw crosshair
    reloadUI->draw(crosshairShader);  // Draw reload bar

    glDisable(GL_DEPTH_TEST); // UI should render on top
    glEnable(GL_BLEND);       // Enable transparency
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Convert screen position to NDC
    glm::vec2 pos = toNDC(10.0f, 50.0f);

    // Build UI text string
    std::string text = "Crates destroyed: " + std::to_string(killCount);

    // Render text
    textRenderer->renderText(
        textShader,
        text,
        pos.x,
        pos.y,
        getScale(0.0015f), // Scale text size
        glm::vec3(1.0f)    // White colour
    );

    glDisable(GL_BLEND);    // Disable blending
    glEnable(GL_DEPTH_TEST); // Restore depth testing
}