#version 330 core

out vec4 FragColor;

in float Height;
in vec3 vNormal;
in vec3 FragPos;

uniform vec3 vColor ;
uniform float shininess;
uniform float ambientStrength = 0.5;

uniform vec3 pointLightPos;
uniform vec3 pointLightColor;

uniform vec3 dirLightColor;
uniform vec3 dirLightDir;

uniform vec3 viewPos;

void main()
{
    float h = (Height + 16)/32.0f;	// shift and scale the height into a grayscale value
    h *= 0.5;
    vec3 result = vec3(h, h, h);
    if(Height < -10.0){
        result = vec3(0.0, 0.5, 1.0);
    }
    // Ambient Light
    vec3 ambient = ambientStrength * result;
    
    // Point Light
    vec3 lightDir = normalize(pointLightPos - FragPos);
    
    float diff = max(dot(vNormal, lightDir), 0.0); 
    vec3 diffuse = diff * pointLightColor;
    
    float specularStrength = 0.0;
    
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, vNormal);
    
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess); 
    vec3 specular = specularStrength * spec * pointLightColor;
    
    //Directional Light
    vec3 dirLightDirNorm = normalize(dirLightDir);
    float dirDiff = max(dot(vNormal, -dirLightDirNorm), 0.0); 
    vec3 dirDiffuse = dirDiff * dirLightColor;
        
    vec3 dirReflectDir = reflect(dirLightDirNorm, vNormal);
    float dirSpec = pow(max(dot(viewDir, dirReflectDir), 0.0), shininess); 
    vec3 dirSpecular = specularStrength * dirSpec * dirLightColor;
        
    result = (ambient + diffuse + specular + dirDiffuse + dirSpecular) * result; 
    //vec3 result = (ambient+diffuse+specular) * result;
    FragColor = vec4(result, 1.0);
}