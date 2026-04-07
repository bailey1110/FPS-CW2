#include "Player.h"

// Constructor: initialise player position, camera, and controls
Player::Player()
{
    position = glm::vec3(0.0f, 1.0f, 3.0f); // Starting position
    front = glm::vec3(0.0f, 0.0f, -1.0f);   // Initial forward direction
    up = glm::vec3(0.0f, 1.0f, 0.0f);       // World up direction

    yaw = -90.0f;  // Horizontal angle (facing -Z)
    pitch = 0.0f;  // Vertical angle
    lastX = 960.0f; // Initial mouse X (screen centre)
    lastY = 540.0f; // Initial mouse Y
    firstMouse = true; // Prevent jump on first movement

    speed = 4.0f;       // Movement speed
    sensitivity = 0.1f; // Mouse sensitivity
}

// Handle keyboard input for movement
void Player::processInput(GLFWwindow* window, float deltaTime)
{
    float velocity = speed * deltaTime; // Movement scaled by frame time

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true); // Exit game

    // Flatten forward direction to XZ plane (ignore vertical movement)
    glm::vec3 flatFront = glm::vec3(front.x, 0.0f, front.z);

    if (glm::length(flatFront) > 0.0f)
        flatFront = glm::normalize(flatFront);

    // Calculate right direction
    glm::vec3 right = glm::normalize(glm::cross(flatFront, up));

    // Movement controls (WASD)
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        position += flatFront * velocity;

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        position -= flatFront * velocity;

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        position -= right * velocity;

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        position += right * velocity;
}

// Handle mouse movement for camera rotation
void Player::mouseLook(double xpos, double ypos)
{
    // Handle first mouse input to avoid large jump
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    // Calculate mouse movement offsets
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    // Apply sensitivity scaling
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    // Update yaw and pitch
    yaw += xoffset;
    pitch += yoffset;

    // Clamp vertical rotation
    if (pitch > maxPitch)
        pitch = maxPitch;
    if (pitch < -maxPitch)
        pitch = -maxPitch;

    // Convert spherical coordinates to direction vector
    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    front = glm::normalize(direction); // Update forward direction
}

// Generate view matrix for camera
glm::mat4 Player::getViewMatrix() const
{
    return glm::lookAt(position, position + front, up); // Camera transform
}