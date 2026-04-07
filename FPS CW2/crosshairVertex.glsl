// Vertex shader for rendering the crosshair in screen space

#version 330 core

layout (location = 0) in vec3 aPos; // Input vertex position

void main()
{
    // Directly output position (already in Normalised Device Coordinates)
    gl_Position = vec4(aPos, 1.0);
}