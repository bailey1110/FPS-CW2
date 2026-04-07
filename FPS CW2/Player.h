#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

class Player
{
public:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;

    float maxPitch = 30.0f;

    float yaw;
    float pitch;
    float lastX;
    float lastY;
    bool firstMouse;

    float speed;
    float sensitivity;

    Player();

    void processInput(GLFWwindow* window, float deltaTime);
    void mouseLook(double xpos, double ypos);
    glm::mat4 getViewMatrix() const;
};