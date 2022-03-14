#version 330 core
layout (location = 0) in vec2 aPos;
//layout (location = 1) in vec2 aTexCoord;
//layout (location = 2) in int hPos;

out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform sampler2D hmap;
uniform int hmapsize;

void main()
{
    // note that we read the multiplication from right to left
    vec4 hmapAstex = texture(hmap, vec2(aPos.x/hmapsize, aPos.y/hmapsize));
    gl_Position = projection * view * model * vec4(aPos.xy, hmapAstex.r, 1.0f);
    TexCoord = vec2(int(aPos.x), int(aPos.y));
    
}