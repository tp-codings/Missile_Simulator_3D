#version 330 core

layout (location = 0) in vec3 aPos; 
layout (location = 1) in vec3 aNormal; 
layout (location = 2) in vec2 aTexCoord; 

out vec3 vNormal; 
out vec2 vTexCoord;
out vec3 FragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0); 
	vNormal = normalize(mat3(transpose(inverse(model))) * aNormal);
	vTexCoord = aTexCoord;
	FragPos = vec3(model * vec4(aPos, 1.0));
}