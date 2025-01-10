#version 330 core
// in vec2 pos;
out vec4 FragColor;

uniform vec4 color;
// uniform sampler2D ourTexture;

void main()
{
    // FragColor = texture(ourTexture, pos);
    // FragColor = FragColor * color;
    FragColor = color;
}