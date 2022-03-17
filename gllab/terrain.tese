#version 410 core
layout(quads, fractional_odd_spacing, ccw) in;

#extension GL_ARB_explicit_uniform_location : enable

uniform sampler2D hmap;
layout (location = 0) uniform int hmapsize;
layout (location = 1) uniform mat4 model;
layout (location = 2) uniform mat4 view;
layout (location = 3) uniform mat4 projection;

layout (location = 0) in vec2 TextureCoord[];


layout (location = 0) out float Height;
layout (location = 1) out vec2 TexCoord;
layout (location = 2) out vec4 Normal;
layout (location = 3) out vec4 FragPos;



void main()
{
    float u = gl_TessCoord.x;
    float v = gl_TessCoord.y;

    vec2 t00 = TextureCoord[0];
    vec2 t01 = TextureCoord[1];
    vec2 t10 = TextureCoord[2];
    vec2 t11 = TextureCoord[3];

    vec2 t0 = (t01 - t00) * u + t00;
    vec2 t1 = (t11 - t10) * u + t10;
    vec2 TexCoord = (t1 - t0) * v + t0;

    float Height = texture(hmap, vec2(TexCoord.x , TexCoord.y)).r;
    

    vec4 p00 = gl_in[0].gl_Position;
    vec4 p01 = gl_in[1].gl_Position;
    vec4 p10 = gl_in[2].gl_Position;
    vec4 p11 = gl_in[3].gl_Position;

    vec4 uVec = p01 - p00;
    vec4 vVec = p10 - p00;
    Normal = normalize( vec4(cross(vVec.xyz, uVec.xyz), 0.0f));

    vec4 p0 = (p01 - p00) * u + p00;
    vec4 p1 = (p11 - p10) * u + p10;
    vec4 p = (p1 - p0) * v + p0 + Normal * Height;
    
    FragPos = model * p;
    gl_Position =  projection * view * FragPos;
    //gl_Position = projection * view * model * p;
}