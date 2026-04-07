// Player.cpp

#include "Player.h"

Player::Player()
{
    position = glm::vec3(0.0f, 1.0f, 3.0f);
    front = glm::vec3(0.0f, 0.0f, -1.0f);
    up = glm::vec3(0.0f, 1.0f, 0.0f);

    yaw = -90.0f;
    pitch = 0.0f;
    lastX = 960.0f;
    lastY = 540.0f;
    firstMouse = true;

    speed = 4.0f; // 20% slower than 5.0
    sensitivity = 0.1f;
}

void Player::processInput(GLFWwindow* window, float deltaTime)
{
    float velocity = speed * deltaTime;

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    glm::vec3 flatFront = glm::vec3(front.x, 0.0f, front.z);

    if (glm::length(flatFront) > 0.0f)
        flatFront = glm::normalize(flatFront);

    glm::vec3 right = glm::normalize(glm::cross(flatFront, up));

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        position += flatFront * velocity;

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        position -= flatFront * velocity;

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        position -= right * velocity;

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        position += right * velocity;
}

void Player::mouseLook(double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > maxPitch)
        pitch = maxPitch;
    if (pitch < -maxPitch)
        pitch = -maxPitch;

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    front = glm::normalize(direction);
}

glm::mat4 Player::getViewMatrix() const
{
    return glm::lookAt(position, position + front, up);
}