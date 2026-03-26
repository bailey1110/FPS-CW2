#pragma once

class ReloadUI
{
public:
    unsigned int VAO;
    unsigned int VBO;

    void setup();
    void update(float progress);
    void draw(unsigned int shaderProgram);
};