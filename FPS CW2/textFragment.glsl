#version 330 core
in vec2 TexCoords; // Texture coordinates for sampling glyph atlas
out vec4 FragColor; // Final output colour

uniform sampler2D text; // Font texture atlas (single channel)
uniform vec3 textColor; // Colour to apply to text

void main()
{
    // Sample the red channel (grayscale alpha) from the font texture
    float alpha = texture(text, TexCoords).r;

    // Output coloured text using sampled alpha for transparency
    FragColor = vec4(textColor, alpha);
}