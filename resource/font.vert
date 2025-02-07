#version 330 core
layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 texPos;
out vec2 pos;

uniform mat4 fix;
uniform mat4 model;
uniform mat4 PV;

void main()
{
    gl_Position = PV * model * fix * vec4(aPos, 0.0, 1.0);
    pos = texPos;
}