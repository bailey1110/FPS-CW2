#pragma once

class Crosshair
{
public:
    unsigned int VAO;
    unsigned int VBO;

    void setup();
    void draw(unsigned int shaderProgram);
};