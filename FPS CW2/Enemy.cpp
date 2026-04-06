#include "Enemy.h"
#include <glm/gtc/matrix_transform.hpp>
#include <windows.h>
#include <mmsystem.h>
#include <string>
#include <thread>
#include <chrono>
#include <atomic>

#pragma comment(lib, "winmm.lib")

static void PlayOverlapSound(const std::wstring& filePath)
{
    static std::atomic<int> soundCounter = 10000;
    std::wstring alias = L"snd_" + std::to_wstring(soundCounter++);

    std::wstring openCmd = L"open \"" + filePath + L"\" type waveaudio alias " + alias;
    mciSendStringW(openCmd.c_str(), NULL, 0, NULL);

    std::wstring playCmd = L"play " + alias;
    mciSendStringW(playCmd.c_str(), NULL, 0, NULL);

    std::thread([alias]()
        {
            std::this_thread::sleep_for(std::chrono::seconds(5));
            std::wstring closeCmd = L"close " + alias;
            mciSendStringW(closeCmd.c_str(), NULL, 0, NULL);
        }).detach();
}

void Enemy::setup(Model* m, Texture* t)
{
    model = m;
    texture = t;

    position = glm::vec3(0.0f, 0.0f, -5.0f);
    isActive = true;

    radius = 0.75f;
}

void Enemy::draw(unsigned int shaderProgram, unsigned int modelLoc, unsigned int useTextureLoc)
{
    if (!isActive) return;

    glm::mat4 modelMat = glm::translate(glm::mat4(1.0f), position);

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &modelMat[0][0]);

    glUniform1i(useTextureLoc, 1);
    texture->bind(0);

    model->draw();
}

bool Enemy::checkHit(glm::vec3 rayOrigin, glm::vec3 rayDir)
{
    if (!isActive) return false;

    glm::vec3 oc = rayOrigin - position;

    float a = glm::dot(rayDir, rayDir);
    float b = 2.0f * glm::dot(oc, rayDir);
    float c = glm::dot(oc, oc) - radius * radius;

    float discriminant = b * b - 4 * a * c;

    if (discriminant < 0.0f)
        return false;

    isActive = false;

    // PLAY IMMEDIATELY (no delay)
    PlayOverlapSound(L".\\crate.wav");

    return true;
}