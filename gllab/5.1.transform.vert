#version 410 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;


uniform mat4 transform;
uniform float heightmap[64*64];
uniform int heightmapsize;


void main()
{
	gl_Position = transform * vec4(aPos.xy, heightmap[int(aPos.x + 0.5)*16 + int(aPos.y + 0.5)], 1.0);
	TexCoord = vec2(aTexCoord.x, aTexCoord.y);
}