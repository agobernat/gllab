#version 410 core

#extension GL_ARB_explicit_uniform_location : enable

layout (location = 0) in float Height;
layout (location = 1) in vec2 TexCoord;
layout (location = 2) in vec4 Normal;
layout (location = 3) in vec4 FragPos;

layout (location = 0) out vec4 FragColor;




layout (location = 5) uniform vec3 lightPos;




// texture samplers
uniform sampler2D texture1;

void main()
{
	vec3 lightColor = vec3(0.3f, 0.5f, 0.3f);
	//vec3 lightPos = vec3(0.0f, 0.0f, 50.0f);
	vec3 norm = normalize(Normal.xyz);
	
	// ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;
  	
    
    vec3 lightDir = normalize(lightPos - FragPos.xyz);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
	
	vec3 result = (ambient + diffuse);
    FragColor = vec4(result, 1.0);
	
	
	//vec4 texColor = texture(texture1, TexCoord);
	////if (texColor.a < 0.3)
	//	//discard;
	////FragColor = texColor;
	//float h = (Height + 16)/64.0f;
    //FragColor = vec4(h, h, h, 1.0);
}