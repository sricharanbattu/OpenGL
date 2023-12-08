#version 440 core

layout(location=0) in vec3 aPos;
layout(location=1) in vec2 aTexCord;

out vec2 TexCord;

void main()
{
	gl_Position = vec4(aPos, 1);
	TexCord = aTexCord;
}