#version 330 core
layout (location = 0) in vec4 vertex; // <pos.xy, tex.xy> (position in xy, texture coords in zw)

out vec2 TexCoords; // Pass texture coordinates to fragment shader

void main()
{
    // Set final vertex position (already in screen space / NDC)
    gl_Position = vec4(vertex.xy, 0.0, 1.0);

    // Extract texture coordinates from input
    TexCoords = vertex.zw;
}