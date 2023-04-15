#version 330 core

in vec2 TexCoords;

uniform vec4 color;
uniform sampler2D smokeTexture;

out vec4 FragColor;

void main()
{
	FragColor = color;
}