#version 330 core
in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D Texture;// 纹理
uniform float a;// 透明度
void main()
{
    FragColor = texture(Texture, TexCoords) * vec4(1.0, 1.0, 1.0, a);
}