#pragma once
#include "Model.h"
#include "Texture.h"

class Environment
{
public:
    Model model;
    Texture texture;

    void setup();
    void draw(unsigned int modelLoc);
};