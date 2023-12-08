#version 440 core

out vec4 FragColor;

in vec2 TexCord;

uniform sampler2D ourTexture;

void main()
{
	FragColor = texture(ourTexture, TexCord);
}