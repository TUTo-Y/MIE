#version 330 core
in vec2 pos;
out vec4 FragColor;

uniform vec4 color;
uniform sampler2D Texture;

void main()
{
    float r=texture(Texture,pos).r;
    if(r < 0.5) discard;
    FragColor=color;

    // FragColor = texture(Texture, pos);
}