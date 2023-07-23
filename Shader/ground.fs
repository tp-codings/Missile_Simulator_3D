#version 330 core

in vec3 vNormal;
in vec2 vTexCoord;
in vec3 FragPos;


out vec4 FragColor;

vec3 vColor;
vec3 lightPos = vec3(0.0, 5000.0, 0.0);
vec3 lightColor = vec3(1.0);
float shininess = 64.f;

uniform vec3 viewPos;



void main(){

        vColor = vec3(0.85, 0.7, 0.4);

        float brownSpots = sin(FragPos.x * 0.03) * cos(FragPos.z*0.03);
        float strength = 0.2;
        vColor = mix(vColor, vec3(0.7, 0.5, 0.2), brownSpots * strength);
        vColor += vec3(0.8);

        float ambientStrength = 0.2;
    
        // Ambient Light
        vec3 ambient = ambientStrength * vColor;
    
        // Point Light
        vec3 lightDir = normalize(lightPos - FragPos);
    
        float diff = max(dot(vNormal, lightDir), 0.0); 
        vec3 diffuse = diff * lightColor;
    
        float specularStrength = 0.0;
    
        vec3 viewDir = normalize(viewPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, vNormal);
    
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess); 
        vec3 specular = specularStrength * spec * lightColor;
    
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