#version 330 core

in vec2 position;

out vec2 textureCoords1;
out vec2 textureCoords2;
out float blend;

uniform mat4 projection;
uniform mat4 model;

uniform vec2 texOffset1;
uniform vec2 texOffset2;
uniform vec2 texCoordInfo;

void main(){
	vec2 textureCoords = position + vec2(0.5, 0.5);
	textureCoords.y = 1.0 - textureCoords.y;

	textureCoords /= texCoordInfo.x;

	textureCoords1 = textureCoords + texOffset1;
	textureCoords2 = textureCoords + texOffset2;

	blend = texCoordInfo.y;

	gl_Position = projection * model * vec4(position, 0.0, 1.0);
}