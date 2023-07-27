#include "Particle.h"
#define GRAVITY -10

Particle::Particle(ParticleTextureHandler texture, glm::vec3 position, glm::vec3 velocity, float gravityEffect, float lifeLength, float rotation, float scale, std::string type):
	texture(texture), position(position), velocity(velocity), gravityEffect(gravityEffect), lifeLength(lifeLength), rotation(rotation), scale(scale), type(type)
{
	this->elapsedTime = 0.0f;
}

glm::vec3 Particle::getPosition()
{
	return this->position;
}

float Particle::getRotation()
{
	return this->rotation;
}

float Particle::getScale()
{
	return this->scale;
}

float Particle::getElapsedTime()
{
	return this->elapsedTime;
}

float Particle::getLifeLength()
{
	return this->lifeLength;
}

std::string Particle::getType()
{
	return this->type;
}

bool Particle::update(float deltaTime)
{
	this->velocity *= (1 - deltaTime*0.2);
	this->velocity.y += GRAVITY * this->gravityEffect * deltaTime;
	this->position += this->velocity;
	this->elapsedTime += deltaTime;

	return elapsedTime < this->lifeLength;
}

ParticleTextureHandler Particle::getTexture()
{
	return this->texture;
}
