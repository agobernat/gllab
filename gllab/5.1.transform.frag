#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

// texture samplers
uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
	// linearly interpolate between both textures (80% container, 20% awesomeface)
	vec4 texColor = texture(texture2, TexCoord);
	if (texColor.a < 0.3)
		discard;
	FragColor = texColor;
}