#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture_diffuse1;

uniform vec3 pointLightColor;


void main()
{    
    FragColor = vec4(pointLightColor*1.5, 1.0);

}