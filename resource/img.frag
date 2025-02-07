#version 330 core
in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D Texture;

void main()
{
    float color = texture(Texture, TexCoords).r;
    FragColor = vec4(color, color, color, 1.0);
}