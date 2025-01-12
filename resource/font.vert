#version 330 core
layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 texPos;
out vec2 pos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(aPos.x, aPos.y, 0.0, 1.0);
    pos = texPos;
}