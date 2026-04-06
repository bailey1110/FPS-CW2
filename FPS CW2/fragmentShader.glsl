#version 330 core

out vec4 FragColor;

in vec2 TexCoord;
in vec3 FragPos;
in vec3 Normal;

uniform sampler2D texture1;
uniform int useTexture;

uniform vec3 lightDir;
uniform vec3 viewPos;
uniform vec3 objectColor;

void main()
{
    // TRACER / UNTEXTURED OBJECTS (pure colour, no lighting)
    if (useTexture == 0)
    {
        FragColor = vec4(objectColor, 1.0);
        return;
    }

    // TEXTURED OBJECTS (normal lighting)
    vec3 baseColor = texture(texture1, TexCoord).rgb;

    vec3 ambient = 0.15 * baseColor;

    vec3 norm = normalize(Normal);
    vec3 light = normalize(-lightDir);
    float diff = max(dot(norm, light), 0.0);
    vec3 diffuse = diff * baseColor;

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-light, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    vec3 specular = vec3(0.3) * spec;

    vec3 result = ambient + diffuse + specular;

    FragColor = vec4(result, 1.0);
}