#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTex;
layout (location = 2) in vec3 aNormal;

out vec2 TexCoord;
out vec3 FragPos;
out vec3 Normal;
out float fogDepth;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    vec4 worldPos = model * vec4(aPos, 1.0);
    FragPos = worldPos.xyz;

    Normal = mat3(transpose(inverse(model))) * aNormal;
    TexCoord = aTex;

    vec4 viewPos = view * worldPos;
    fogDepth = abs(viewPos.z);

    gl_Position = projection * viewPos;
}