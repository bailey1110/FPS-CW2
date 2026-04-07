#pragma once

// UI element that displays reload progress as a bar on screen
class ReloadUI
{
public:
    unsigned int VAO; // Vertex Array Object (stores vertex layout)
    unsigned int VBO; // Vertex Buffer Object (stores vertex data)

    // Initialise OpenGL buffers for the reload bar
    void setup();

    // Update the reload bar based on progress (0.0 = empty, 1.0 = full)
    void update(float progress);

    // Render the reload bar using the given shader
    void draw(unsigned int shaderProgram);
};