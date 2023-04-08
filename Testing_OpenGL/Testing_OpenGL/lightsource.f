#version 330 core
out vec4 FragColor;

uniform vec3 lightSourceColor;

void main()
{
    FragColor = vec4(lightSourceColor, 1.0f);
}