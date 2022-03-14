#version 410 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;
  
void main() {

    vec3 A = gl_in[0].gl_Position.xyz;
    vec3 B = gl_in[1].gl_Position.xyz;
    vec3 C = gl_in[2].gl_Position.xyz;


    
    EndPrimitive();
}    