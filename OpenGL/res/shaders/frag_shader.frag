#version 330 core

out vec4 FragColor;

in vec4 outColor;
in vec2 outTexcord;

uniform sampler2D ourTexture;

void main()
{
    //FragColor = texture(ourTexture, outTexcord);
    FragColor = outColor*texture(ourTexture, outTexcord);
    //FragColor = outColor;\n"

 }