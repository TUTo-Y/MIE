#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoords;

uniform mat4 model;
uniform mat4 PV;

out vec2 TexCoords;

void main()
{
    gl_Position = PV * model * vec4(aPos, 1.0);
    TexCoords = aTexCoords;
}