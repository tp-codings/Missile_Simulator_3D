#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>

#include "ParticleTextureHandler.h"

class Particle
{
public:
	Particle(ParticleTextureHandler particleTexture, glm::vec3 position, glm::vec3 velocity, float gravityEffect, float lifeLength, float rotation, float scale, std::string type);
	glm::vec3 getPosition();
	float getRotation();
	float getScale();
	float getElapsedTime();
	float getLifeLength();
	std::string getType();
	bool update(float deltaTime);

	ParticleTextureHandler getTexture();



private:
	glm::vec3 position;
	glm::vec3 velocity;
	float gravityEffect;
	float lifeLength;
	float rotation;
	float scale;
	float elapsedTime;
	std::string type;

	ParticleTextureHandler texture;

	glm::vec2 texOffset1;
	glm::vec2 texOffset2;

	float blendFactor;
};

