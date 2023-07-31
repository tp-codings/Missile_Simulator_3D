#version 330 core

out vec4 FragColor;

in float Height;

void main()
{
    float h = (Height + 16)/32.0f;	// shift and scale the height into a grayscale value
    h *= 0.5;
    FragColor = vec4(h, h, h, 1.0);
    if(Height < -10.0){
        FragColor = vec4(0.0, 0.5, 1.0, 1.0);
    }
}