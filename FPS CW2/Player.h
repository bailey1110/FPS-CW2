#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

// Handles player movement and camera control
class Player
{
public:
    glm::vec3 position; // Player position in world space
    glm::vec3 front;    // Direction the player is facing
    glm::vec3 up;       // Up direction (usually world up)

    float maxPitch = 30.0f; // Maximum vertical look angle (prevents over-rotation)

    float yaw;   // Horizontal rotation (left/right)
    float pitch; // Vertical rotation (up/down)

    float lastX; // Last mouse X position (for calculating movement delta)
    float lastY; // Last mouse Y position
    bool firstMouse; // Used to prevent large jump on first mouse input

    float speed;       // Movement speed
    float sensitivity; // Mouse sensitivity

    // Constructor: initialise player position and camera values
    Player();

    // Handle keyboard input for movement
    void processInput(GLFWwindow* window, float deltaTime);

    // Handle mouse movement for camera rotation
    void mouseLook(double xpos, double ypos);

    // Generate view matrix for rendering (camera transform)
    glm::mat4 getViewMatrix() const;
};