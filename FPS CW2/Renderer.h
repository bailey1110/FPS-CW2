#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "Model.h"
#include "Texture.h"
#include "Enemy.h"
#include "BulletTracer.h"

// Handles all rendering of 3D objects and effects in the scene
class Renderer
{
public:
    unsigned int shaderProgram; // Main shader program used for rendering

    // Cached uniform locations (for efficiency)
    GLint modelLoc;        // Model matrix uniform
    GLint viewLoc;         // View matrix uniform
    GLint projectionLoc;   // Projection matrix uniform
    GLint textureLoc;      // Texture sampler uniform
    GLint useTextureLoc;   // Toggle for using textures
    GLint objectColorLoc;  // Colour for non-textured objects

    // Lighting uniforms
    GLint lightDirLoc;     // Directional light direction
    GLint viewPosLoc;      // Camera position (for lighting calculations)

    // Fog uniforms
    GLint fogColorLoc;     // Fog colour
    GLint fogMinLoc;       // Distance where fog starts
    GLint fogMaxLoc;       // Distance where fog is fully applied

    // Flashlight (player-attached light)
    GLint lightPosLoc;        // Flashlight position (player position)
    GLint lightDirPlayerLoc; // Flashlight direction (player view direction)
    GLint flashlightOnLoc;   // Toggle flashlight on/off

    // Initialise renderer and retrieve all uniform locations
    void init(unsigned int shader);

    // Set camera matrices and lighting values each frame
    void setMatrices(const glm::mat4& view,
        const glm::mat4& projection,
        const glm::vec3& camPos,
        const glm::vec3& camFront,
        bool flashlightOn);

    // Draw the floor model with texture
    void drawFloor(Model& model, Texture& tex);

    // Draw an enemy
    void drawEnemy(Enemy& enemy);

    // Draw the player's gun model
    void drawGun(Model& model, Texture& tex,
        const glm::vec3& pos, const glm::mat4& rot);

    // Draw bullet tracer effect
    void drawTracer(BulletTracer& tracer);
};