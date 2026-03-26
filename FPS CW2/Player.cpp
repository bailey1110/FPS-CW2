// Player.cpp (FULL - quarter speed)

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

    speed = 1.25f; // quarter of original 5.0
    sensitivity = 0.1f;
}

void Player::processInput(GLFWwindow* window)
{
    float velocity = speed * 0.016f;

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
        lastX = (float)xpos;
        lastY = (float)ypos;
        firstMouse = false;
    }

    float xoffset = (float)xpos - lastX;
    float yoffset = lastY - (float)ypos;

    lastX = (float)xpos;
    lastY = (float)ypos;

    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

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