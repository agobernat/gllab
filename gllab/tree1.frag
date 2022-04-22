#version 410 core
out vec4 FragColor;

in vec2 TexCoord;

// texture samplers


void main()
{
	vec4 texColor = vec4(1,0,0,1);
	FragColor = texColor;
}