// Fragment shader for rendering the crosshair

#version 330 core

out vec4 FragColor; // Final output colour of the fragment

void main()
{
    // Set crosshair colour to white (no lighting or texture)
    FragColor = vec4(1.0, 1.0, 1.0, 1.0);
}