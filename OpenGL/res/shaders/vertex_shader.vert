#version 420 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTex;

out vec4 outColor;
out vec2 outTexcord;

void main()
{
   gl_Position = vec4(aPos.x*cos, aPos.y, -1*aPos.x*sin, 1.0);
   outColor = vec4(aColor, 1.0);
   outTexcord = aTex;
}