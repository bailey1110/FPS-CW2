#pragma once

// Simple UI crosshair rendered at the center of the screen
class Crosshair
{
public:
    unsigned int VAO; // Vertex Array Object for storing vertex configuration
    unsigned int VBO; // Vertex Buffer Object for storing vertex data

    // Initialise crosshair geometry and upload to GPU
    void setup();

    // Render the crosshair using the provided shader
    void draw(unsigned int shaderProgram);
};