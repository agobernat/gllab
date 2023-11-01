#version 410 core
out vec4 FragColor;

in vec2 aTexC;
in vec3 aNormC;
uniform sampler2D tx;
uniform vec4 baseColorFactor;

uniform mat4 view;
uniform vec4 coloruniform;
  


void main()
{
    //FragColor = vec4(1, 0, 0, 0.3);
    FragColor = coloruniform;
} 