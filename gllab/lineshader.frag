#version 410 core
out vec4 FragColor;

uniform vec4 dbgColor;

void main()
{
    FragColor = dbgColor;
} 