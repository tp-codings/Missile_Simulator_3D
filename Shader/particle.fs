#version 330 core

in vec2 textureCoords1;
in vec2 textureCoords2;
in float blend;

uniform sampler2D particleTexture;

out vec4 FragColor;



void main()
{

    vec4 color1 = texture(particleTexture, textureCoords1);
    vec4 color2 = texture(particleTexture, textureCoords2);

    FragColor = mix(color1, color2, 1.0);

    //FragColor = vec4(1.0);
}