#version 410 core
out vec4 FragColor;

in vec2 aTexC;
in vec3 aNormC;
uniform sampler2D tx;
uniform vec4 baseColorFactor;

uniform mat4 view;
  
//in vec4 vertexColor; // the input variable from the vertex shader (same name and same type)  

void main()
{
    discard;
    FragColor = vec4(0, 0, 0, 0);
    //vec4(1.0f, 0.0f, 0.0f, 1.0f);//texture(tx, aTexC); //baseColorFactor * (0.4f + 0.6f * dot(normalize(aNormC), normalize(vec3(view[0][2], view[1][2], view[2][2]))));
} 