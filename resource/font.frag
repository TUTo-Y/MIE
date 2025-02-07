#version 330 core
in vec2 pos;
out vec4 FragColor;

uniform vec4 color;         // 字体颜色
uniform sampler2D Texture;  // 字体纹理

void main()
{
    // 检查RED位
    if(texture(Texture, pos).r < 0.5)discard;
    
    // 字体颜色
    FragColor=color;
}