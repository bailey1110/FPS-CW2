#version 330 core

layout (location = 0) in vec3 aPos;   // Vertex position
layout (location = 1) in vec2 aTex;   // Texture coordinates
layout (location = 2) in vec3 aNormal; // Vertex normal

out vec2 TexCoord; // Pass texture coordinates to fragment shader
out vec3 FragPos;  // Pass world position to fragment shader
out vec3 Normal;   // Pass transformed normal to fragment shader
out float fogDepth; // Depth value used for fog calculation

uniform mat4 model;      // Model transformation matrix
uniform mat4 view;       // View (camera) matrix
uniform mat4 projection; // Projection matrix

void main()
{
    // Transform vertex into world space
    vec4 worldPos = model * vec4(aPos, 1.0);
    FragPos = worldPos.xyz;

    // Transform normal into world space (handles scaling correctly)
    Normal = mat3(transpose(inverse(model))) * aNormal;

    // Pass texture coordinates through
    TexCoord = aTex;

    // Transform to view space
    vec4 viewPos = view * worldPos;

    // Use depth (z value) from view space for fog calculation
    fogDepth = abs(viewPos.z);

    // Final clip-space position
    gl_Position = projection * viewPos;
}