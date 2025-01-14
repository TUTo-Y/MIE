#version 330 core
in vec2 pos;
out vec4 FragColor;

uniform vec4 color;

void main()
{
    // 检查RED位
    float r=texture(Texture,pos).r;
    if(r < 0.5) discard;

    // 字体颜色
    FragColor=color;
}