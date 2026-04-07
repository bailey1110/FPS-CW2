#version 330 core

out vec4 FragColor;

in vec2 TexCoord;
in vec3 FragPos;
in vec3 Normal;
in float fogDepth;

uniform sampler2D texture1;
uniform int useTexture;

uniform vec3 lightPos;
uniform vec3 lightDirPlayer;
uniform float flashlightOn;

uniform float fogMin;
uniform float fogMax;

void main()
{
    vec3 norm = normalize(Normal);

    vec3 baseColor = (useTexture == 1)
        ? texture(texture1, TexCoord).rgb
        : vec3(1.0);

    vec3 toFrag = FragPos - lightPos;
    float dist = length(toFrag);

    vec3 L = normalize(toFrag);

    vec3 forwardXZ = normalize(vec3(lightDirPlayer.x, 0.0, lightDirPlayer.z));
    vec3 dirXZ = normalize(vec3(L.x, 0.0, L.z));

    float angle = dot(forwardXZ, dirXZ);

    float cutoff = 0.2;
    float intensity = smoothstep(cutoff, 1.0, angle);

    float radius = 14.0;
    float falloff = 1.0 - clamp(dist / radius, 0.0, 1.0);
    falloff = pow(falloff, 1.1);

    float diff = max(dot(norm, -lightDirPlayer), 0.0);

    float bloom = 1.0 / (dist * 0.25 + 1.0);

    vec3 diffuse = baseColor * diff * intensity * falloff * flashlightOn * 14.0 * bloom;

    vec3 ambient = baseColor * 0.1;

    vec3 lighting = ambient + diffuse;

    lighting = lighting / (lighting + vec3(1.0));

    float fogFactor = (fogMax - fogDepth) / (fogMax - fogMin);
    fogFactor = clamp(fogFactor, 0.0, 1.0);

    vec3 finalColor = lighting * fogFactor;

    FragColor = vec4(finalColor, 1.0);
}