#version 330 core

in vec3 vNormal;
in vec2 vTexCoord;
in vec3 FragPos;


out vec4 FragColor;

uniform vec3 vColor ;
uniform vec3 dirLightPos;
uniform vec3 dirLightColor;
uniform float shininess;
uniform float ambientStrength;

uniform vec3 viewPos;


void main(){
    
        // Ambient Light
        vec3 ambient = ambientStrength * vColor;
    
        // Point Light
        vec3 lightDir = normalize(dirLightPos - FragPos);
    
        float diff = max(dot(vNormal, lightDir), 0.0); 
        vec3 diffuse = diff * dirLightColor;
    
        float specularStrength = 0.0;
    
        vec3 viewDir = normalize(viewPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, vNormal);
    
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess); 
        vec3 specular = specularStrength * spec * dirLightColor;
    
        // Directional Light
        //vec3 dirLightDirNorm = normalize(dirLightDir);
        //float dirDiff = max(dot(vNormal, -dirLightDirNorm), 0.0); 
        //vec3 dirDiffuse = dirDiff * dirLightColor;
        //
        //vec3 dirReflectDir = reflect(dirLightDirNorm, vNormal);
        //float dirSpec = pow(max(dot(viewDir, dirReflectDir), 0.0), shininess); 
        //vec3 dirSpecular = specularStrength * dirSpec * dirLightColor;
        //
        //vec3 result = (ambient + diffuse + specular + dirDiffuse + dirSpecular) * vColor; 
        //vec3 result = (ambient+dirSpecular+dirDiffuse) * vColor; 
        vec3 result = (ambient+diffuse+specular) * vColor;
        FragColor = vec4(result, 1.0);

}