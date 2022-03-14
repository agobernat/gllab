#version 410 core
layout (location = 0) in vec2 aPos;
//layout (location = 1) in vec2 aTexCoord;
//layout (location = 2) in int hPos;

out vec2 TexCoord;
out vec3 FragPos;
//out vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform sampler2D hmap;
uniform int hmapsize;

void main()
{
    
    // note that we read the multiplication from right to left
    vec4 hmapAstex = texture(hmap, vec2(aPos.x/hmapsize, aPos.y/hmapsize));
    FragPos = vec3(model * vec4(aPos, hmapAstex.r, 1.0f));
    gl_Position = projection * view  * vec4(FragPos, 1.0f);

    
    TexCoord = vec2(int(aPos.x), int(aPos.y));
    
}