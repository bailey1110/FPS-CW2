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
uniform vec3 viewPos;
uniform float flashlightOn;

uniform float fogMin;
uniform float fogMax;

void main()
{
    vec3 norm = normalize(Normal);

    vec3 baseColor = (useTexture == 1)
        ? texture(texture1, TexCoord).rgb
        : vec3(1.0);

    vec3 lightDir = normalize(lightDirPlayer);
    vec3 toFrag = FragPos - lightPos;
    float dist = length(toFrag);
    vec3 toFragDir = normalize(toFrag);

    float coneDot = dot(lightDir, toFragDir);

    float innerCutoff = 0.92;
    float outerCutoff = 0.55;

    float coneFactor = smoothstep(outerCutoff, innerCutoff, coneDot);

    float radius = 16.0;
    float distanceFactor = 1.0 - clamp(dist / radius, 0.0, 1.0);
    distanceFactor = pow(distanceFactor, 1.35);

    float diff = max(dot(norm, -lightDir), 0.0);

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(lightDir, norm);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);

    vec3 diffuse = baseColor * diff * coneFactor * distanceFactor * flashlightOn * 6.0;
    vec3 specular = vec3(1.0) * spec * coneFactor * distanceFactor * flashlightOn * 2.5;

    vec3 ambient = baseColor * 0.08;

    float beamCore = coneFactor * distanceFactor;
    float beamBloom = pow(beamCore, 1.6) * 2.2;

    float heightFade = 1.0 - clamp(abs(FragPos.y - lightPos.y) / 6.0, 0.0, 1.0);
    beamBloom *= mix(0.7, 1.0, heightFade);

    vec3 beamColor = vec3(1.0, 0.95, 0.85) * beamBloom * flashlightOn;

    vec3 lighting = ambient + diffuse + specular + beamColor;

    lighting = lighting / (lighting + vec3(1.0));

    float fogFactor = (fogMax - fogDepth) / (fogMax - fogMin);
    fogFactor = clamp(fogFactor, 0.0, 1.0);

    vec3 finalColor = lighting * fogFactor;

    FragColor = vec4(finalColor, 1.0);
}