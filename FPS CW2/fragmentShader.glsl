#version 330 core

out vec4 FragColor; // Final output colour of the fragment

in vec2 TexCoord;   // Texture coordinates
in vec3 FragPos;    // Fragment position in world space
in vec3 Normal;     // Surface normal at fragment
in float fogDepth;  // Precomputed depth value for fog

uniform sampler2D texture1; // Texture sampler
uniform int useTexture;     // Toggle: 1 = use texture, 0 = plain colour

uniform vec3 lightPos;        // Flashlight position (player)
uniform vec3 lightDirPlayer;  // Flashlight direction
uniform vec3 viewPos;         // Camera position
uniform float flashlightOn;   // Toggle flashlight (0 or 1)

uniform float fogMin; // Distance where fog starts
uniform float fogMax; // Distance where fog is fully applied

void main()
{
    vec3 norm = normalize(Normal); // Normalise surface normal

    // Choose base colour (texture or white)
    vec3 baseColor = (useTexture == 1)
        ? texture(texture1, TexCoord).rgb
        : vec3(1.0);

    vec3 lightDir = normalize(lightDirPlayer); // Normalised flashlight direction
    vec3 toFrag = FragPos - lightPos;          // Vector from light to fragment
    float dist = length(toFrag);               // Distance from light to fragment
    vec3 toFragDir = normalize(toFrag);        // Direction from light to fragment

    float coneDot = dot(lightDir, toFragDir); // Angle between light direction and fragment

    float innerCutoff = 0.92; // Inner cone angle (full brightness)
    float outerCutoff = 0.55; // Outer cone angle (fade out)

    // Smooth transition between inner and outer cone
    float coneFactor = smoothstep(outerCutoff, innerCutoff, coneDot);

    float radius = 16.0; // Maximum flashlight range
    float distanceFactor = 1.0 - clamp(dist / radius, 0.0, 1.0); // Distance attenuation
    distanceFactor = pow(distanceFactor, 1.35); // Smooth falloff

    // Diffuse lighting (Lambert)
    float diff = max(dot(norm, -lightDir), 0.0);

    vec3 viewDir = normalize(viewPos - FragPos); // Direction from fragment to camera
    vec3 reflectDir = reflect(lightDir, norm);   // Reflection direction

    // Specular highlight (Phong)
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);

    // Combine diffuse and specular lighting with flashlight factors
    vec3 diffuse = baseColor * diff * coneFactor * distanceFactor * flashlightOn * 6.0;
    vec3 specular = vec3(1.0) * spec * coneFactor * distanceFactor * flashlightOn * 2.5;

    vec3 ambient = baseColor * 0.08; // Ambient lighting (constant low light)

    // Beam intensity core
    float beamCore = coneFactor * distanceFactor;
    float beamBloom = pow(beamCore, 1.6) * 2.2; // Bloom effect for beam

    // Reduce beam intensity based on vertical difference
    float heightFade = 1.0 - clamp(abs(FragPos.y - lightPos.y) / 6.0, 0.0, 1.0);
    beamBloom *= mix(0.7, 1.0, heightFade);

    // Slight warm colour for flashlight beam
    vec3 beamColor = vec3(1.0, 0.95, 0.85) * beamBloom * flashlightOn;

    // Final lighting combination
    vec3 lighting = ambient + diffuse + specular + beamColor;

    // Tone mapping to prevent over-bright values
    lighting = lighting / (lighting + vec3(1.0));

    // Fog factor based on depth
    float fogFactor = (fogMax - fogDepth) / (fogMax - fogMin);
    fogFactor = clamp(fogFactor, 0.0, 1.0);

    // Apply fog (fade lighting based on distance)
    vec3 finalColor = lighting * fogFactor;

    FragColor = vec4(finalColor, 1.0); // Output final colour
}