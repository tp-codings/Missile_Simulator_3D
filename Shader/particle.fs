#version 330 core

in vec2 textureCoords1;
in vec2 textureCoords2;
in float blend;

uniform float elapsedTime;
uniform float lifeLength;
uniform sampler2D particleTexture;

out vec4 FragColor;



void main()
{
    // Normalisierte Zeit zwischen 0 und 1 berechnen
    float normalizedTime = clamp(elapsedTime / lifeLength, 0.0, 1.0);

    // Mische die Farben von Rot zu Braun über die normalisierte Zeit
    vec3 startColor = vec3(1.0, 0.65, 0.0);
    vec3 endColor = vec3(0.8);
    vec3 finalColor = endColor;

    if(normalizedTime <0.2){
        finalColor = mix(startColor, endColor, normalizedTime/0.2);
    }


    // Setze die FragColor auf den berechneten Farbwert
    FragColor = vec4(finalColor, 1.0);


    vec4 color1 = texture(particleTexture, textureCoords1);
    vec4 color2 = texture(particleTexture, textureCoords2);

    FragColor = mix(color1, color2, blend);
}