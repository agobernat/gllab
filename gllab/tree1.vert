#version 410 core
layout (location = 0) in vec3 aPos; // the position variable has attribute position 0
layout (location = 1) in vec3 aNorm;
layout (location = 2) in vec2 aTex;
  
out vec4 vertexColor; // specify a color output to the fragment shader

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0); // see how we directly give a vec3 to vec4's constructor
    vertexColor = vec4(0.5, 0.0, 0.0, 1.0) * dot(aNorm, vec3(view[0][2], view[1][2], view[2][2])); // set the output variable to a dark-red color
}

