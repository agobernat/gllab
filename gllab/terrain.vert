#version 410 core

layout (location = 0) in vec2 aPos;
layout (location = 0) out vec2 TexCoord;
#extension GL_ARB_explicit_uniform_location : enable
layout (location = 0) uniform int hmapsize;





//out vec3 FragPos;

//out vec3 Normal;

//uniform mat4 model;
//uniform mat4 view;
//uniform mat4 projection;

//uniform sampler2D hmap;


void main()
{
    
    // note that we read the multiplication from right to left
    //vec4 hmapAstex = texture(hmap, vec2(aPos.x/hmapsize, aPos.y/hmapsize));
    //FragPos = vec3(model * vec4(aPos, hmapAstex.r, 1.0f));
    //gl_Position = projection * view  * vec4(FragPos, 1.0f);
    
    gl_Position = vec4(aPos, 0.0f, 1.0);
    
    TexCoord = vec2(aPos.x / hmapsize, aPos.y / hmapsize);
    
    
}