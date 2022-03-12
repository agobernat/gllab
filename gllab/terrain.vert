#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
//layout (location = 2) in int hPos;

out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float heightmap[16*16];
uniform int heightmapsize;

void main()
{
    // note that we read the multiplication from right to left
    gl_Position = projection * view * model * vec4(aPos.xy, heightmap[int(aPos.x + 0.5)*heightmapsize + int(aPos.y + 0.5)], 1.0);
    TexCoord = vec2(aTexCoord.x, aTexCoord.y);
    
}